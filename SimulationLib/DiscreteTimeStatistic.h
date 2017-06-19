#pragma once
#include <string>
#include <vector>
#include <limits>


using namespace std;

namespace SimulationLib
{
	class DiscreteTimeStatistic
	{
	public:
		string name;
		DiscreteTimeStatistic(string name);
		DiscreteTimeStatistic(string name, int numOfObservationsToStore);
		void Record(double value);

		double GetSum();
		double GetCount();
		double GetMean();
		double GetVariance();
		double GetMin();
		double GetMax();
		vector<double> GetObservations();

	private:
		double mean, variance, min, max, total;
		double varianceNominator;
		int count;
		int numObservations;
		vector<double> observations;
	};

}
