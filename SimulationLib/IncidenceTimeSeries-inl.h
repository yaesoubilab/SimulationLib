#pragma once

#include "IncidenceTimeSeries.h"

namespace SimulationLib {
    using namespace std;

    // Initializes a new IncidenceTimeSeries.
    // 'name': name of the time series
    // 'time0': first time point in simulation (usually 0)
    // 'timeMax': max time point in simulation
    // 'periodLength': how often recordings are aggregated
    //
    // Optional parameters:
    // [ 'recordPeriodLength': the number of periods to aggregate before
    //     recording a statistic. Must be a positive integer (1 or greater),
    //     or, if 'RECORD_ON_ALL' is passed, the ::Record method will be
    //     called on every value input.
    //   'stats': an instance of a TimeStatistic class (for now,
    //     DiscreteTimeStatistic or ContinuousTimeStatistic). Its ::Record
    //     method will be called in accordance with 'recordPeriodLength'. ]
    template <typename T>
    IncidenceTimeSeries<T>::IncidenceTimeSeries
      (string _name, double _time0, double _timeMax, double _periodLength,
        int _recordPeriod, TimeStatistic *_stats) {

        // Number of periods which will be recorded
        int numPeriods;

        if (_time0 < 0)
            printf("Error: time0 must be non-negative\n");
        if (_periodLength <= 0)
            printf("Error: observationPeriodLength must be a positive integer\n");
        if (_periodLength <= 0)
            printf("Error: periodLength must be non-negative\n");
        if (_timeMax < _periodLength)
            printf("Error: timeMax must be >= periodLength\n");
        if (_time0 > _timeMax)
            printf("Error: timeMax must be >= time0\n");
        if (_stats && !(_recordPeriod != RECORD_ON_ALL || _recordPeriod > 0))
            printf("Error: recordPeriod must be RECORD_ON_ALL or a positive int\n");

        name                  = _name;

        time0                 = _time0;
        lastTime              = _time0;
        timeMax               = _timeMax;

        lastPeriod            = 0;
        periodLength          = _periodLength;
        numPeriods            = (int)ceil(timeMax / periodLength);

        observations          = new vector<T>(numPeriods, (T)0);
        aggregatedObservation = (T)0;

        writable              = true;

        recordPeriod          = _recordPeriod;
        stats                 = _stats;
    }

    template <typename T>
    IncidenceTimeSeries<T>::~IncidenceTimeSeries(void) {
        delete observations;
    }

    // Records a new value at time 'time' and adds it to the current
    //   aggregation.
    // For successive calls, 'time' must monotonically increase. In other words,
    //   the function will issue an error if called with a 'time' of lower value
    //   than one which was seen in any previous invocation of the function.
    // template<typename T> void IncidenceTimeSeries::Record(int time, T value) {
    template <typename T>
    bool IncidenceTimeSeries<T>::Record(double time, T value) {

        int currentPeriod; // Vector index of period corresponding to val of 'time'

        if (!writable) {
            printf("Error: Close() has already been called\n");
            return false;
        }

        // Is 'time' too low?
        if (time < time0) {
            printf("Error: Time specified is earlier than 'time0'\n");
            return false;
        }

        // Is 'time' previous to the latest time yet seen?
        if (time < lastTime) {
            printf("Error: successive entries must be monotonically non-decreasing\n");
            return false;
        }

        currentPeriod = (int)floor(time / periodLength);

        // Call Record if:
        // 1. recordPeriod is set to 'RECORD_ON_ALL': we always call Record,
        //    and pass in the time associated with the new value.
        //
        // 2. We have just completed a period, and and the period completed is
        //    was the 'recordPeriod'-th period after the last period after which
        //    Record was called. In this case, we pass the period number, rather
        //    than the time.
        if (stats && recordPeriod == RECORD_ON_ALL)
            stats->Record(time, (double)value);
        else if (stats &&
                 currentPeriod > lastPeriod &&
                 (lastPeriod % recordPeriod) == 0)
            stats->Record(lastPeriod, (double)(*observations)[lastPeriod]);

        aggregatedObservation       += value;
        (*observations)[currentPeriod] += value;

        lastTime   = time;
        lastPeriod = currentPeriod;

        return true;
    }

    template <typename T>
    void IncidenceTimeSeries<T>::Close(void) {
        if (stats && recordPeriod > 0)
            stats->Record(lastPeriod, (double)aggregatedObservation);
        writable = false;
        return;
    }

    // Returns a vector containing all complete aggregations, and the current
    //   incomplete aggregation.
    template<typename T>
    vector<T> *IncidenceTimeSeries<T>::GetVector() {
        if (writable)
            printf("Warning: IncidenceTimeSeries is still writable.\n");

        return observations;
    }

    template <typename T>
    double IncidenceTimeSeries<T>::GetTime0(void) {
        return time0;
    }

    template <typename T>
    string IncidenceTimeSeries<T>::GetName(void) {
        return name;
    }

    template <typename T>
    bool IncidenceTimeSeries<T>::IsWritable(void) {
        return writable;
    }

    // Returns a value of type 'T' containing the sum of the incomplete
    //   aggregation. If no points have yet been added to this aggregation,
    //   returns 0.
    template<typename T>
    T IncidenceTimeSeries<T>::GetLastObservation() {
        return (*observations)[(int)floor(lastTime / periodLength)];
    }

    // Returns the summed value of all complete aggregations, and the incomplete
    //   aggregation
    template<typename T>
    T IncidenceTimeSeries<T>::GetTotal() {
        return aggregatedObservation;
    }
}
