#ifndef TIMER
#define TIMER
#include "ITimer.h"
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#define DOUBLE_MAX 1.79769e+308
using namespace std;

class TimerObje{
    private:
        Timepoint timer_point;
        Timepoint next_timer_point;
        TPredicate p_func;
        Millisecs period;
        TTimerCallback call_back;
        int type;

    public:
        TimerObje(const Timepoint &next_timer_point_, const Timepoint &timer_point_, const TTimerCallback &call_back_);
        TimerObje(const Timepoint &next_timer_point_, const Millisecs & period, const TTimerCallback &call_back_);
        TimerObje(const Timepoint &next_timer_point_, const Timepoint &timer_point_, const Millisecs & period, const TTimerCallback &call_back_);
        TimerObje(const Timepoint &next_timer_point_, const TPredicate &pred, const Millisecs & period, const TTimerCallback &call_back_);
        Timepoint getTimePoint();
        Timepoint getNextPoint();
        TPredicate getPredicateFunc();
        Millisecs getPeriodms();
        TTimerCallback getCallback();
        int getTimerType();
        void setTimePoint(const Timepoint &timer_point_);
        void setNextPoint(const Timepoint &next_timer_point_);
        void setPredicateFunc(const TPredicate &pred);
        void setPeriodms(const Millisecs & period);
        void setCallback(const TTimerCallback &call_back_);
        void setTimerType(int type);
};

class Timer : public ITimer{
    private:
        vector<TimerObje> timerVector;
        thread timerThread;
        atomic<int> currentTime{0};
        atomic<bool> running{true};
        bool cvFlag = false;
        chrono::duration<double> min_time();
        mutex mutex1;
        condition_variable cv;

    public:
        Timer();
        ~Timer();
        void registerTimer(const Timepoint &tp, const TTimerCallback &cb);
        void registerTimer(const Millisecs & period, const TTimerCallback &cb);
        void registerTimer(const Timepoint &tp, const Millisecs & period, const TTimerCallback &cb);
        void registerTimer(const TPredicate &pred, const Millisecs & period, const TTimerCallback &cb);
};





#endif
