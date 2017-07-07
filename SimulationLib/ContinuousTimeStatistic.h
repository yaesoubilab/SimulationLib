#pragma once
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include "TimeStatistic.h"

using namespace std;

namespace SimulationLib
{
	class ContinuousTimeStatistic : public TimeStatistic
	{
	public:
		string name;

		// Instantiates a new ContinuousTimeStatistic class. 'name' is any
		//   string describing the data to be stored in the class.
		ContinuousTimeStatistic(string _name) : TimeStatistic(_name) {
	        count     = 0;
	        baseTime  = 0;

	        mean      = 0;
	        variance  = 0;

	        // Guarantees that first value will be recorded as the max and the min
	        min       = numeric_limits<double>::max();
	        max       = numeric_limits<double>::min();

	        lastValue = 0;
	        lastTime  = 0;
	        tot       = 0; // Intermediate variable used to simplify calculations
	    }

		~ContinuousTimeStatistic();

		// Note: Record(double) will fail, because a time dimension must be
		//   specified for ContinuousTimeStatistic. In the future, this may
		//   cause an error to be thrown.
		void Record(double increment);
		void Record(double time, double increment);

		// Getter methods
		double GetMean();
		double GetCount();
		double GetVariance();
		double GetMin();
		double GetMax();

	private:
		long count;
		double currentValue, mean, variance, min, max;

		double baseTime;
		double lastValue, lastTime, value;
		double tot;
	};

}
