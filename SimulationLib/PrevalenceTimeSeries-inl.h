#include "PrevalenceTimeSeries.h"

namespace SimulationLib {
    using namespace std;

    template <typename T>
    PrevalenceTimeSeries<T>::PrevalenceTimeSeries
      (string _name, double _timeMax, double _periodLength) {

        int numPeriods; // Number of periods to allocate memory for

        name = _name;

        timeMax      = _timeMax;
        periodLength = _periodLength;
        numPeriods   = (int)ceil(timeMax / periodLength);

        currentPrevalence = (T)0;

        lastTime          = (T)0;
        lastPeriod        = 0;

        prevalence        = vector<T>(numPeriods + 1, (T)0);
    }

    template <typename T>
    void PrevalenceTimeSeries<T>::Record(double time, T increment) {
        int thisPeriod;

        // Is 'time' a non-negative integer?
        if (time < 0) {
            printf("Error: 'time' must be non-negative\n");
            return;
        }

        if (time > timeMax) {
            printf("Error: 'time' cannot be greater than 'timeMax'\n");
            return;
        }

        if (time < lastTime) {
            printf("Error: Successive calls to ::Record must have \
                      monotonically increasing time\n");
            return;
        }

        // Update current prevalence, note 'increment' can be positive or
        // negative.
        thisPeriod = (int)ceil(time / periodLength);

        if (thisPeriod > lastPeriod)
            _storePrevalence(lastPeriod);

        currentPrevalence += increment;

        lastTime = time;
        lastPeriod = thisPeriod;

        return;
    }

    template <typename T>
    vector<T> PrevalenceTimeSeries<T>::GetObservations(void) {
        // Make sure vector is current with last data
        _storePrevalence(lastPeriod);

        return prevalence;
    }

    template <typename T>
    T PrevalenceTimeSeries<T>::GetCurrentPrevalence(void) {
        return currentPrevalence;
    }

    template <typename T>
    void PrevalenceTimeSeries<T>::_storePrevalence(int period) {
        prevalence[period] = currentPrevalence;
    }
}
