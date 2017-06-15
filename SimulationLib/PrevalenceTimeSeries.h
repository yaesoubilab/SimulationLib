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

		// Initializes a new time series. Time unit 0 is valued at 0. The name
		//   'name' of the time series is stored in PrevalenceTimeSeries::name.
		PrevalenceTimeSeries(string name);

		// For a non-negative integer 'time', records the change in prevalence
		//   'increment' at that time unit.
		void Record(int time, int increment);

		// Returns an integer vector where an element at index 'i' represents
		// the change in prevalence at time unit 'i'
		vector<int> GetObservations(void);

		// Returns the current prevalence (the current prevalence is defined
		//   as the sum of every change in prevalence already recorded through
		//   calling 'Record').
		int GetCurrentPrevalence(void);

	private:
		// Integral of 'deltaPrevalence' vector, updated in constant time
		int currentPrevalence;

		// Each element of 'deltaPrevalence' holds the *change* in prevalence.
		// Element of highest index is the most recent change.
		// Indices for which no change in prevalence has yet been recorded
		//   are assigned a change of 0.
		vector<int> deltaPrevalence;
	};
}
