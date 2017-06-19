#pragma once

#include "IncidenceTimeSeries.h"

namespace SimulationLib {
    using namespace std;

    // Initializes a new IncidenceTimeSeries.
    // 'name': name of the time series
    // 'time0': first time point in simulation (usually 0, must be non-negative)
    // 'observationPeriodLength': how often recordings are aggregated
    template <typename T>
    IncidenceTimeSeries<T>::IncidenceTimeSeries
      (string _name, double _time0, double _timeMax, double _periodLength) {

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

        name                    = _name;
        time0                   = _time0;
        mostRecentTime          = _time0;
        timeMax                 = _timeMax;
        periodLength            = _periodLength;

        numPeriods              = (int)ceil(timeMax / periodLength);
        observations            = vector<T>(numPeriods, (T)0);
    }

    // Records a new value at time 'time' and adds it to the current
    //   aggregation.
    // For successive calls, 'time' must monotonically increase. In other words,
    //   the function will issue an error if called with a 'time' of lower value
    //   than one which was seen in any previous invocation of the function.
    // template<typename T> void IncidenceTimeSeries::Record(int time, T value) {
    template <typename T>
    void IncidenceTimeSeries<T>::Record(double time, T value) {

        int currentPeriod; // Vector index of current period being aggregated
        int timePeriod;    // Vector index of period corresponding to val of 'time'

        // Is 'time' too low?
        if (time < time0) {
            printf("Error: Time specified is earlier than 'time0'\n");
            return;
        }

        // Is 'time' previous to the latest time yet seen?
        if (time < mostRecentTime) {
            printf("Error: successive entries must be monotonically non-decreasing\n");
            return;
        }

        timePeriod = (int)floor(time / periodLength);

        // Update sum of all aggregates and max time yet seen
        aggregatedObservation += value;
        mostRecentTime = time;

        observations[timePeriod] += value;
        return;
    }

    // Returns a vector containing all complete aggregations, and the current
    //   incomplete aggregation.
    template<typename T>
    vector<T> IncidenceTimeSeries<T>::GetObservations() {
        return observations;
    }

    // Returns a value of type 'T' containing the sum of the incomplete
    //   aggregation. If no points have yet been added to this aggregation,
    //   returns 0.
    template<typename T>
    T IncidenceTimeSeries<T>::GetLastObservation() {
        return observations[(int)floor(mostRecentTime / periodLength)];
    }

    // Returns the summed value of all complete aggregations, and the incomplete
    //   aggregation
    template<typename T>
    T IncidenceTimeSeries<T>::GetTotal() {
        return aggregatedObservation;
    }
}
