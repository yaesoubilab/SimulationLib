#pragma once
#include <string>
#include <vector>

using namespace std;

namespace SimulationLib
{
	// to store time-series incidence data and to perform calculations on the time-series
	class IncidenceTimeSeries
	{
	public:
		string name;
		IncidenceTimeSeries(string name, int time0, int observationPeriodLength); 
			// time0: the initial simulation time (usually 0)
			// observationPeriodLength: specifies how often the recordings should be aggregated
		template<typename T> void Record(int time, T value); 
			// record a new data point that was observed at the given time (note that time is integer) 
			// note: observations should be aggregated depending on the value of observationPeriodLength 
				// (observationPeriodLength = 5 means that 5 recordings should be summed up to get 1 data point.) 
		template<typename T> vector<T> GetObservations(); 
			// return the vector of recorded data points
		template<typename T> T GetLastObservation();
			// return the last (aggregated) observation stored
		template<typename T> T GetTotal(); 
			// return the sum of all recorded data points

	private:
		double observationPeriodLength;
		double aggregatedObservation;
		vector<double> observations;
	};
}