#include "PrevalenceTimeSeries.h"
#include <limits.h>

namespace SimulationLib {
    using namespace std;

    PrevalenceTimeSeries::PrevalenceTimeSeries(string _name) {
        name = _name;
        currentPrevalence = 0;
    };

    void PrevalenceTimeSeries::Record(int time, int increment) {
        // Holds size of vector 'currentPrevalence'
        int size;

        // Will the addition operation cause overflow or underflow?
        if (((increment > 0) && (currentPrevalence > INT_MAX - increment))
         || ((increment < 0) && (currentPrevalence < INT_MIN - increment))) {
            printf("Error: increment would cause integer overflow or underflow\n");
            return;
        }

        // Is 'time' a non-negative integer?
        if (time < 0) {
            printf("Error: 'time' is a negative integer\n");
            return;
        }

        // Update current prevalence, note 'increment' can be positive or
        // negative.
        currentPrevalence += increment;

        // If needed, insert 0-valued elements between last time recorded
        // and 'time'
        size = deltaPrevalence.size();
        if (time > size) {
            for (int i = size; i < time; ++i)
                deltaPrevalence.push_back(0);

            deltaPrevalence.push_back(increment);
        } else if (time == size) {
            deltaPrevalence.push_back(increment);
        } else { // Increment value of an existing element
            deltaPrevalence[time] += increment;
        }

        return;
    }

    vector<int> PrevalenceTimeSeries::GetObservations(void) {
        return deltaPrevalence;
    }

    int PrevalenceTimeSeries::GetCurrentPrevalence(void) {
        return currentPrevalence;
    }
}
