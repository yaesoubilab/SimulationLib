#pragma once

#include <queue>
#include <vector>
#include <map>
#include <utility>
#include <functional>

using namespace std;

namespace SimulationLib {

// TimeT is the type representing time in the EventQueue, and ResultT is
//   the type returned from an EventFunc after being run.
template <typename TimeT = double, typename ResultT = bool>
class EventQueue {
public:

    // --- Alias types ---

    // Function accepting no arguments which runs the simulation, returning
    //   a ResultT
    using EventFuncRunner = function<ResultT(void)>;

    // Object to pass to the queue: contains the time 't' at which the EventFunc
    //   will be run via execution of 'run'
    using ScheduledEvent = struct SE {
        TimeT t;
        EventFuncRunner run;
    };

    // Type of scheduler passed to an EventFunc
    using SchedulerT = function<void(ScheduledEvent *)>;

    // An EventFunc takes a time and a scheduler and outputs a ResultT
    using EventFunc = function<ResultT(TimeT, SchedulerT)>;

    // Package an EventFunc 'ef' to into a ScheduledEvent to be run at time 't'.
    //   This method should be called to produce ScheduledEvents to feed to the
    //   'Schedule' function.
    ScheduledEvent *MakeScheduledEvent(TimeT t, EventFunc ef) {
        ScheduledEvent *se = new ScheduledEvent;

        // Bind 'this' to the Schedule function
        auto boundScheduler = bind(&EventQueue::Schedule, this, placeholders::_1);

        // Wrap 'ef' into an EventFuncRunner
        EventFuncRunner efr = [this, t, ef, boundScheduler] (void) {
            return ef(t, boundScheduler);
        };

        // Set 't' and 'run' fields of EventRunner struct
        se->t   = t;
        se->run = efr;

        return se;
    }

    // The following three methods forward to the underlying data structure
    //   to allow access to the EventFuncs
    bool                  Empty(void) { return pq->empty(); };
    const ScheduledEvent   *Top(void) { return pq->top(); };
    void                    Pop(void) { return pq->pop(); };

    // Schedule 'e' for execution
    void Schedule(ScheduledEvent *e)   { return pq->push(e); }

    void QuickSchedule(TimeT t, EventFunc ef) {
        return Schedule(MakeScheduledEvent(t, ef));
    }

    // Constructor
    EventQueue(void) {
        auto boundScheduledEventCmp =
          bind(&EventQueue::scheduledEventCmp, this, placeholders::_1, placeholders::_2);

        pq = new ScheduledEventPQ(boundScheduledEventCmp);
    }

    // Destructor
    ~EventQueue(void) {
        delete pq;
    }

private:
    // Allows comparison of ScheduledEvents for insertion into priority queue
    int scheduledEventCmp(const ScheduledEvent *se1, const ScheduledEvent *se2)
        { return se1->t > se2->t; };

    // Specialized priority_queue for storing 'ScheudledEvent's
    using ScheduledEventPQ =
      priority_queue<ScheduledEvent *,
                     vector<ScheduledEvent *>,
                     function<int(const ScheduledEvent*, const ScheduledEvent*)>>;

    ScheduledEventPQ *pq;
};
}
