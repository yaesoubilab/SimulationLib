#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <math.h>
#include "TimeStatistic.h"


using namespace std;

namespace SimulationLib
{
	class DiscreteTimeStatistic : public TimeStatistic
	{
	public:
		// DiscreteTimeStatistic calculates time-independent point statistics.
		// All observations are recorded in a vector to enable calculating the percentile
		DiscreteTimeStatistic(string _name) : TimeStatistic(_name)
	    {
	        // Min/max begin as the boundaries of 'double'
	        min = numeric_limits<double>::max();
	        max = numeric_limits<double>::min();

	        // Initialize more statistics
	        total    = 0;
	        count    = 0;
	        mean     = 0;
	        variance = 0;

	        varianceNominator = 0;
	    }

		// DiscreteTimeStatistic(string name) : DiscreteTimeStatistic(name, 0) {}

		~DiscreteTimeStatistic();

		// Receives a value 'value', updating statistics in constant time.
		// Records the value if 'numOfObservationsToStore' > 0. Throws error
		//   if no room left to hold observations.
		void Record(double value);
		void Record(double time, double value);

		// Accessor methods for statistics, current to the last call of ::Record.
		double GetSum();
		double GetCount();
		double GetMean();
		double GetVariance();
		double GetMin();
		double GetMax();

		// Returns vector of observations. Warning: will return an uninitialied
		//   variable if DiscreteTimeStatistics was instantiated without
		//   specification of 'numOfObservationsToStore'.
		vector<double> GetObservations();

		// Return the alpha-th percentile. Throws error if alpha out of range;
		double GetPercentile(double alpha);

	private:
		void _record(double value);

		double mean, variance, min, max, total;
		long count;
		double varianceNominator;
		vector<double> observations;
	};

}
