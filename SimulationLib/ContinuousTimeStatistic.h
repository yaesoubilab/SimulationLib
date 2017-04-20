#pragma once
#include <string>
#include <vector>

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
		double GetVariance();
		double GetMin();
		double GetMax();

	private:
		double currentValue, mean, variance, min, max;
	};

}