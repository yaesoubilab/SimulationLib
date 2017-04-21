#pragma once
#include <string>
#include <vector>

using namespace std;

namespace SimulationLib
{
	// to store time-series prevalence data 
	class PrevalenceTimeSeries
	{
	public:
		string name;
		PrevalenceTimeSeries(string name);
		void Record(int time, int increment);
			// record the change in prevalence that was observed at the given time (note that time is integer)
			// note: this function takes the "change" in the prevalence so we need to have a separate variable to store the current prevalence.
		vector<int> GetObservations();
			// returns the time series

	private:
	};
}

