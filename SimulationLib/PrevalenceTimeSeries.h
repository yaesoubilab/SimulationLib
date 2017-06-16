#pragma once
#include <string>
#include <vector>

using namespace std;

/// This class uses Drake's `-inl.h` pattern.  When seeing linker errors from
/// this class, please refer to http://drake.mit.edu/cxx_inl.html.
///
/// Instantiated templates for the following kinds of T's are provided:
/// - double
/// - int
///
/// They are already available to link against in the containing library.
namespace SimulationLib
{
	template <typename T>
	class PrevalenceTimeSeries
	{
	public:
		string name;

		// Initializes a new time series. Time unit 0 is valued at 0.
		// - 'name': name of the time series
		// - 'timeMax': the upper boundary of time. Time domain is [0, timeMax].
		// - 'periodLength': the length of each period
		PrevalenceTimeSeries(string name, double timeMax, double periodLength);

		// For a non-negative integer 'time', records the change in prevalence
		//   'increment' at that time unit.
		void Record(double time, T increment);

		// Returns a vector. Element 'i' represents the prevalence at the most
		// recent observation <= 'i'.
		vector<T> GetObservations(void);

		// Returns the current prevalence (the current prevalence is defined
		//   as the sum of every change in prevalence already recorded through
		//   calling 'Record').
		T GetCurrentPrevalence(void);

	private:
		double timeMax;
		double periodLength;

		// Updated in constant time
		T currentPrevalence;
		double currentTime;

		// Each element of 'prevalence' represents a period of length
		//   'periodLength', who value is the most recent observation (the
		//   observation of highest time-value recorded for that period).
		// Element of highest index is the most recent.
		vector<T> prevalence;
	};
}
