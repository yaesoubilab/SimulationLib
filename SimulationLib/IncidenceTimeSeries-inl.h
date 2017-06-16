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
      (string _name, double _time0, double _observationPeriodLength) {
        if (_time0 < 0)
            printf("Error: time0 must be non-negative\n");
        if (_observationPeriodLength <= 0)
            printf("Error: observationPeriodLength must be a positive integer\n");

        name = _name;
        time0 =   _time0;
        timeMax = _time0;
        observationPeriodLength = _observationPeriodLength;
        observations = vector<T>(1, 0);
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
        if (time < timeMax) {
            printf("Error: cannot retroactively record behind max time yet seen\n");
            return;
        }

        currentPeriod = (int)floor((timeMax - time0) / observationPeriodLength);
        timePeriod =    (int)floor((time - time0)    / observationPeriodLength);

        // Update sum of all aggregates and max time yet seen
        aggregatedObservation += value;
        timeMax = time;

        // If 'time' is part of the current aggregate, add to that aggregate
        if (currentPeriod == timePeriod) {
            observations[currentPeriod] += value;
            return;
        }

        // Add an appropriate number of 0-valued aggregates between the
        // current aggregate and the new aggregate corresponding to timePeriod
        for (int i = currentPeriod + 1; i < timePeriod; ++i)
            observations.push_back(0);

        observations.push_back(value);
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
        return observations.back();
    }

    // Returns the summed value of all complete aggregations, and the incomplete
    //   aggregation
    template<typename T>
    T IncidenceTimeSeries<T>::GetTotal() {
        return aggregatedObservation;
    }
}
