#pragma once
#include <string>
#include <vector>

using namespace std;

// This class uses Drake's `-inl.h` pattern.  When seeing linker errors from
// this class, please refer to http://drake.mit.edu/cxx_inl.html.
//
// Instantiated templates for the following kinds of T's are provided:
// - double
// - int
//
// They are already available to link against in the containing library.

namespace SimulationLib
{
	// to store time-series incidence data and to perform calculations on the time-series
	template <typename T>
	class IncidenceTimeSeries
	{
	public:
		string name;

		// Initializes a new IncidenceTimeSeries.
	    // 'name': name of the time series
	    // 'time0': first time point in simulation (usually 0)
	    // 'observationPeriodLength': how often recordings are aggregated
		IncidenceTimeSeries(string _name, double _time0, double _observationPeriodLength);

	    // Records a new value at time 'time' and adds it to the current
	    //   aggregation.
	    // For successive calls, 'time' must monotonically increase. In other words,
	    //   the function will issue an error if called with a 'time' of lower value
	    //   than one which was seen in any previous invocation of the function.
	    void Record(double time, T value);

	    // Returns a vector containing all complete aggregations, and the current
	    //   incomplete aggregation.
	    vector<T> GetObservations();

	    // Returns a value of type 'T' containing the sum of the incomplete
	    //   aggregation. If no points have yet been added to this aggregation,
	    //   returns 0.
	    T GetLastObservation();

	    // Returns the summed value of all complete aggregations, and the incomplete
	    //   aggregation
	    T GetTotal();

	private:
		double time0;
		double timeMax;
		double observationPeriodLength;
		T aggregatedObservation;
		vector<T> observations;
	};
}

#include "IncidenceTimeSeries.inl"
