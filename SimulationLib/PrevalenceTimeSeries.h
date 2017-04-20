#pragma once
#include <string>
#include <vector>

using namespace std;

namespace SimulationLib
{
	struct PrevalenceObservation
	{
		double time;
		double value;
	};

	class PrevalenceTimeSeries
	{
	public:
		string name;
		PrevalenceTimeSeries(string name);
		void Record(double time, double value);

	private:
		vector<PrevalenceObservation> observations;
	};
}

