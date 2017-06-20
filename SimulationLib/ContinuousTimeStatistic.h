#pragma once
#include <string>
#include <vector>
#include <cmath>

using namespace std;

namespace SimulationLib
{
	class ContinuousTimeStatistic
	{
	public:
		string name;
		ContinuousTimeStatistic(string name);
		void Record(double time, double increment);

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
