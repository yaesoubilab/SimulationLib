#pragma once
#include <string>
#include <vector>
#include <limits>
#include "TimeStatistic.h"


using namespace std;

namespace SimulationLib
{
	class DiscreteTimeStatistic : public TimeStatistic
	{
	public:
		// DiscreteTimeStatistic calculates time-independent point statistics.
		// To record observations for later access via the ::GetObservations()
		//   method, specify 'numOfObservationsToStore' on instantiation.
		DiscreteTimeStatistic(string name, long numOfObservationsToStore);

		DiscreteTimeStatistic(string _name, long numOfObservationsToStore) :
		  TimeStatistic(_name)
	    {
	        numObservations = numOfObservationsToStore;

	        // Min/max begin as the boundaries of 'double'
	        min = numeric_limits<double>::max();
	        max = numeric_limits<double>::min();

	        // Initialize more statistics
	        total    = 0;
	        count    = 0;
	        mean     = 0;
	        variance = 0;

	        varianceNominator = 0;

	        // If recording, allocate memory for vector and zero everything
	        if (numObservations)
	            observations = vector<double>(numObservations, 0);
	    }

		DiscreteTimeStatistic(string name) : DiscreteTimeStatistic(name, 0) {}

		~DiscreteTimeStatistic();

		// Receives a value 'value', updating statistics in constant time.
		// Records the value if 'numOfObservationsToStore' > 0. Throws error
		//   if no room left to hold observations.
		virtual void Record(double value);
		virtual void Record(double time, double value);

		// Accessor methods for statistics, current to the last call of ::Record.
		double GetSum();
		double GetCount();
		double GetMean();
		double GetVariance();
		double GetMin();
		double GetMax();

		// Returns vector of observations. Warning: will return an uninitialied
		//   variable if DiscreteTimeStatistics was instantiated without
		//   specification of 'numOfObservationsToStore'.
		vector<double> GetObservations();

	private:
		void _record(double value, long locationIndex);

		double mean, variance, min, max, total;
		long count, numObservations;
		double varianceNominator;
		vector<double> observations;
	};

}
