#pragma once
#include <string>
#include <vector>

using namespace std;

namespace SimulationLib
{
	class DiscreteTimeStatistic
	{
	public:
		string name;
		DiscreteTimeStatistic(string name);
		DiscreteTimeStatistic(string name, long numOfObservationsToStore);
		void Record(double value);
		void Record(double value, long locationIndex);

		double GetSum();
		double GetCount();
		double GetMean();
		double GetVariance();
		double GetMin();
		double GetMax();
		vector<double> GetObservations();

	private:
		double mean, variance, min, max, total;
		long count, numObservations;

		vector<double*> observations;
	};

}
