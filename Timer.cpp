#include "Timer.h"

TimerObje::TimerObje(const Timepoint &next_timer_point_, const Timepoint &timer_point_, const TTimerCallback &call_back_){
    this->next_timer_point = next_timer_point_;
    this->timer_point = timer_point_;
    this->call_back = call_back_;
    this->type = 1;
}

TimerObje::TimerObje(const Timepoint &next_timer_point_, const Millisecs & period, const TTimerCallback &call_back_){
    this->next_timer_point = next_timer_point_;
    this->period = period;
    this->call_back = call_back_;
    this->type = 2;
}

TimerObje::TimerObje(const Timepoint &next_timer_point_, const Timepoint &timer_point_, const Millisecs & period, const TTimerCallback &call_back_){
    this->next_timer_point = next_timer_point_;
    this->timer_point = timer_point_;
    this->period = period;
    this->call_back = call_back_;
    this->type = 3;
}

TimerObje::TimerObje(const Timepoint &next_timer_point_, const TPredicate &pred, const Millisecs & period, const TTimerCallback &call_back_){
    this->next_timer_point = next_timer_point_;
    this->p_func = pred;
    this->period = period;
    this->call_back = call_back_;
    this->type = 4;
}

Timepoint TimerObje::getTimePoint(){
    return timer_point;
}

Timepoint TimerObje::getNextPoint(){
    return next_timer_point;
}

TPredicate TimerObje::getPredicateFunc(){
    return p_func;
}

Millisecs TimerObje::getPeriodms(){
    return period;
}

TTimerCallback TimerObje::getCallback(){
    return call_back;
}

int TimerObje::getTimerType(){
    return type;
}

void TimerObje::setTimePoint(const Timepoint &timer_point_){
    this->timer_point = timer_point_;
}

void TimerObje::setNextPoint(const Timepoint &next_timer_point_){
    this->next_timer_point = next_timer_point_;
}

void TimerObje::setPredicateFunc(const TPredicate &p_func){
    this->p_func = p_func;
}

void TimerObje::setPeriodms(const Millisecs & period){
    this->period = period;
}

void TimerObje::setCallback(const TTimerCallback &call_back_){
    this->call_back = call_back_;
}

void TimerObje::setTimerType(int type){
    this->type = type;
}


Timer::Timer() {
    cout<<"Timer::thread function starting"<<endl;

    timerThread = thread([=](){
        while (true){
            if ((timerVector.size()!=0) && running){
                auto sleeping_time = min_time();
                this_thread::sleep_for(sleeping_time);
                if(!running)
                    break;
                chrono::duration<double> time;
                unique_lock<mutex> ulock(mutex1);
                if(timerVector[currentTime].getTimerType() == 1){
                    timerVector[currentTime].getCallback()();
                    time = CLOCK::now() - timerVector[currentTime].getTimePoint();
                    timerVector.erase(timerVector.begin()+currentTime);
                }
                else if(timerVector[currentTime].getTimerType() == 2){
                    timerVector[currentTime].getCallback()();
                    time = (timerVector[currentTime].getNextPoint() + timerVector[currentTime].getPeriodms()) - CLOCK::now();
                    timerVector[currentTime].setNextPoint (CLOCK::now());
                }
                else if(timerVector[currentTime].getTimerType() == 3){
                    time = (timerVector[currentTime].getNextPoint() + timerVector[currentTime].getPeriodms()) - CLOCK::now();
                    timerVector[currentTime].setNextPoint (CLOCK::now());
                    if (CLOCK::now() >= timerVector[currentTime].getTimePoint())
                        timerVector.erase(timerVector.begin() + currentTime);
                    else
                        timerVector[currentTime].getCallback()();
                }
                else{
                    timerVector[currentTime].getCallback()();
                    time = (timerVector[currentTime].getNextPoint()  + timerVector[currentTime].getPeriodms()) - CLOCK::now();
                    if (timerVector[currentTime].getPredicateFunc()())
                        timerVector[currentTime].setNextPoint(CLOCK::now());
                    else
                        timerVector.erase(timerVector.begin() + currentTime);
                }
                if ((int)(time.count()*1000)>10)
                    cerr<<"Deadline miss"<<endl;
            }
            else{
                if(!running)
                    break;
                else{
                    unique_lock<mutex> ulock(mutex1);
                    cv.wait(ulock,[&]{return cvFlag;});
                    cvFlag = false;
                }
            }
        }
    });
}

Timer::~Timer(){
    cout<<"Timer::thread function terminating"<<endl;{
        unique_lock<mutex> ulock(mutex1);
        cvFlag = true;
        running = false;
        cv.notify_one();
        ulock.unlock();
    }
    timerThread.join();
    
}

void Timer::registerTimer(const Timepoint &tp, const TTimerCallback &cb){
        unique_lock<mutex> ulock(mutex1);
        auto last = CLOCK::now();
        timerVector.push_back(TimerObje(last, tp, cb));
        cvFlag = true;
        cv.notify_one();
}

void Timer::registerTimer(const TPredicate &pred, const Millisecs & period, const TTimerCallback &cb){
        unique_lock<mutex> ulock(mutex1);
        auto last = CLOCK::now();
        timerVector.push_back(TimerObje(last, pred, period, cb));
        cvFlag = true;
        cv.notify_one();
}

void Timer::registerTimer(const Timepoint &tp, const Millisecs & period, const TTimerCallback &cb){
        unique_lock<mutex> ulock(mutex1);
        auto last = CLOCK::now();
        timerVector.push_back(TimerObje(last, tp, period, cb));
        cvFlag = true;
        cv.notify_one();
}

void Timer::registerTimer(const Millisecs & period, const TTimerCallback &cb){
        unique_lock<mutex> ulock(mutex1);
        auto last = CLOCK::now();
        timerVector.push_back(TimerObje(last, period, cb));
        cvFlag = true;
        cv.notify_one();
}

chrono::duration<double> Timer::min_time(){
    auto now = CLOCK::now();
    chrono::duration<double> time;
    double min = DOUBLE_MAX;
    for (int i = 0; i < timerVector.size(); i++){
        if(timerVector[i].getTimerType() == 1)
            time  = timerVector[i].getTimePoint() - now;
        else
            time  = (timerVector[i].getNextPoint() + timerVector[i].getPeriodms()) - now;
        if(min > time.count()){
            currentTime = i;
            min = time.count();
        }
    }
    return chrono::duration<double>(min);
}


