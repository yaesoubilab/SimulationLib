#include "IncidenceTimeSeries.h"

namespace SimulationLib {
    using namespace std;

    // Initializes a new IncidenceTimeSeries.
    // 'name': name of the time series
    // 'time0': first time point in simulation (usually 0, must be non-negative)
    // 'observationPeriodLength': how often recordings are aggregated
    IncidenceTimeSeries(string _name, int _time0, int _observationPeriodLength) {

    }

    // Records a new value at time 'time' and adds it to the current
    //   aggregation.
    // For successive calls, 'time' must monotonically increase. In other words,
    //   the function will issue an error if called with a 'time' of lower value
    //   than one which was seen in any previous invocation of the function.
    template<typename T> void Record(int time, T value) {


        return;
    }

    // Returns a vector containing all complete aggregations, and the current
    //   incomplete aggregation.
    template<typename T> vector<T> GetObservations() {
        vector<T> v;
        return v;
    }

    // Returns a value of type 'T' containing the sum of the incomplete
    //   aggregation. If no points have yet been added to this aggregation,
    //   returns 0.
    template<typename T> T GetLastObservation() {
        return observations.back()
    }

    // Returns the summed value of all complete aggregations, and the incomplete
    //   aggregation
    template<typename T> T GetTotal() {
        return 0;
    }
}
