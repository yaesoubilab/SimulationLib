#pragma once
#include <string>
#include <vector>

using namespace std;

namespace SimulationLib
{
	// to store time-series incidence data and to perform calculations on the time-series
	class IncidenceTimeSeries
	{
	public:
		string name;

		// Initializes a new IncidenceTimeSeries.
	    // 'name': name of the time series
	    // 'time0': first time point in simulation (usually 0)
	    // 'observationPeriodLength': how often recordings are aggregated
		IncidenceTimeSeries(string _name, int _time0, int _observationPeriodLength);

	    // Records a new value at time 'time' and adds it to the current
	    //   aggregation.
	    // For successive calls, 'time' must monotonically increase. In other words,
	    //   the function will issue an error if called with a 'time' of lower value
	    //   than one which was seen in any previous invocation of the function.
	    template<typename T> void Record(int time, T value);

	    // Returns a vector containing all complete aggregations, and the current
	    //   incomplete aggregation.
	    template<typename T> vector<T> GetObservations();

	    // Returns a value of type 'T' containing the sum of the incomplete
	    //   aggregation. If no points have yet been added to this aggregation,
	    //   returns 0.
	    template<typename T> T GetLastObservation();

	    // Returns the summed value of all complete aggregations, and the incomplete
	    //   aggregation
	    template<typename T> T GetTotal();

	private:
		int time0;
		int timeMax;
		double observationPeriodLength;
		double aggregatedObservation;
		vector<double> observations;
	};
}
