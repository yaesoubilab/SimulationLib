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
		IncidenceTimeSeries(string name, int time0, int observationPeriodLength); // observationPeriodLength specifies how often the recordings should be aggregated
		void Record(int time, double value);
		vector<double> GetObservations();

	private:
		double observationPeriodLength;
		double aggregatedObservation;
		vector<double> observations;
	};
}