#include <iostream>
#include "Timer.h"
#include <string>
using namespace std;
using CLOCK = chrono::high_resolution_clock;
using TTimerCallback = function<void()>;

static CLOCK::time_point T0;

void logCallback(int id, const string &logstr) {
    auto dt = CLOCK::now() - T0;
    cout << "[" << chrono::duration_cast<chrono::milliseconds>(dt).count() << "] (cb " << id << "): " << logstr << endl;
}

int main(){
    cout << "GIVEN TEST" << endl;
    Timer timer;
    this_thread::sleep_for(chrono::seconds(1));
    T0 = CLOCK::now();
    logCallback(-1, "main starting.");
    auto t1 = CLOCK::now() + chrono::seconds(1);
    auto t2 = t1 + chrono::seconds(1);
    timer.registerTimer(t2, [&]() {logCallback(1, "callback str"); });
    timer.registerTimer(t1, [&]() {logCallback(2, "callback str"); });
    timer.registerTimer(Millisecs(700), [&]() { logCallback(3, "callback str"); });
    timer.registerTimer(t1 + Millisecs(300), Millisecs(500), [&]() { logCallback(4, "callback str"); });
    timer.registerTimer([&]() {
        static int count = 0;
        return ++count < 3;
    }, Millisecs(500), [&]() { logCallback(5, "callback str"); });
    this_thread::sleep_for(chrono::seconds(5));
    logCallback(-1, "main terminating.");
    cout << "MY TEST" << endl;
    this_thread::sleep_for(chrono::seconds(1));
    T0 = CLOCK::now();
    logCallback(-1, "main starting.");
    t1 = CLOCK::now() + chrono::seconds(1);
    t2 = t1 + chrono::seconds(1);
    timer.registerTimer(t2, [&]() {logCallback(1, "callback str (type 1 timer)"); });
    timer.registerTimer(t1, [&]() {logCallback(2, "callback str (type 1 timer)"); });
    timer.registerTimer(Millisecs(200), [&]() { logCallback(3, "callback str (200ms period - type 2 timer)"); });
    timer.registerTimer(Millisecs(1000), [&]() { logCallback(4, "callback str (1000ms period - type 2 timer)"); });
    timer.registerTimer(t1 + Millisecs(3000), Millisecs(1000), [&]() { logCallback(5, "callback str (1000ms period - type 3 timer)"); });
    timer.registerTimer(t1 + Millisecs(2200), Millisecs(1100), [&]() { logCallback(6, "callback str (1100ms period - type 3 timer)"); });
    timer.registerTimer([&]() {
        static int count = 0;
        return ++count < 3;
    }, Millisecs(3000), [&]() { logCallback(7, "callback str (3000ms period - type 4 timer)"); });
    timer.registerTimer([&]() {
        static int count = 0;
        return ++count < 5;
    }, Millisecs(800), [&]() { logCallback(8, "callback str (800ms period - type 4 timer)"); });
    this_thread::sleep_for(chrono::seconds(10));
    logCallback(-1, "main terminating.");
}
