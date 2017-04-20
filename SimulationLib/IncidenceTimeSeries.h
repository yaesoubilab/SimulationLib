#pragma once
#include <string>
#include <vector>

using namespace std;

namespace SimulationLib
{
	class IncidenceTimeSeries
	{
	public:
		string name;
		IncidenceTimeSeries(string name, double observationPeriodLength);
		void Record(double time, double value);

	private:
		double observationPeriodLength;
		double aggregatedObservation;
		vector<double> observations;
	};
}