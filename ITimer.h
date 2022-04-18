#ifndef ITIMER
#define ITIMER

#include <iostream>
#include <chrono>
#include <functional>
using namespace std;

using CLOCK = chrono::high_resolution_clock;
using TTimerCallback = function<void()>;
using Millisecs = chrono::milliseconds;
using Timepoint = CLOCK::time_point;
using TPredicate = function<bool()>;

class ITimer {
    public:
    virtual void registerTimer(const Timepoint& tp, const TTimerCallback& cb) = 0;
    virtual void registerTimer(const Millisecs& period, const TTimerCallback& cb) = 0;
    virtual void registerTimer(const Timepoint& tp, const Millisecs& period, const TTimerCallback& cb) = 0;
    virtual void registerTimer(const TPredicate& pred, const Millisecs& period, const TTimerCallback& cb) = 0;
};

#endif
