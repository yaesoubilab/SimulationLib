#pragma once

#include <string>
#include <vector>
#include <cmath>
#include "TimeStatistic.h"
#include "TimeSeries.h"

using namespace std;

// This class uses Drake's `-inl.h` pattern.  When seeing linker errors from
// this class, please refer to http://drake.mit.edu/cxx_inl.html.
//
// Instantiated templates for the following kinds of T's are provided:
// - double
// - int
//
// They are already available to link against in the containing library.

#define RECORD_ON_ALL -1

namespace SimulationLib
{
	// to store time-series incidence data and to perform calculations on the time-series
	template <typename T>
	class IncidenceTimeSeries : public TimeSeries<T>
	{
	public:
		string name;

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

		IncidenceTimeSeries(string name, double time0, double timeMax, \
							double periodLength, int recordPeriod,
							TimeStatistic *stats);

		IncidenceTimeSeries(string name, double time0, double timeMax, double periodLength) :\
		  IncidenceTimeSeries(name, time0, timeMax, periodLength, 0, NULL) {}

		~IncidenceTimeSeries();

	    // Records a new value at time 'time' and adds it to the current
	    //   aggregation.
	    // For successive calls, 'time' must monotonically increase. In other words,
	    //   the function will issue an error if called with a 'time' of lower value
	    //   than one which was seen in any previous invocation of the function.
	    void Record(double time, T value);

	    // Call at the end of the time series to trigger Record of statistics
	    // for current period
	    void Close(void);

	    // Returns a vector containing all complete aggregations, and the current
	    //   incomplete aggregation.
	    virtual vector<T> *GetVector();

	    // Returns a value of type 'T' containing the sum of the incomplete
	    //   aggregation. If no points have yet been added to this aggregation,
	    //   returns 0.
	    T GetLastObservation();

	    // Returns the summed value of all complete aggregations, and the incomplete
	    //   aggregation
	    T GetTotal();

	    double GetTime0();
	    string GetName();
	    bool IsWritable();

	private:
		double time0;
		double timeMax;
		double periodLength;

		double lastTime;
		int    lastPeriod;

		T aggregatedObservation;
		vector<T> *observations;

		bool writable;

		int recordPeriod;
		TimeStatistic *stats;
	};
}
