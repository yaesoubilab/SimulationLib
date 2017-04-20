#pragma once
#include <string>
#include <vector>

using namespace std;

namespace SimulationLib
{
	class PrevalenceStatistic
	{
	public:
		string name;
		PrevalenceStatistic(string name);
		void Record(double time, double increment);

	private:
		double currentValue, mean, variance, min, max;
	};

}