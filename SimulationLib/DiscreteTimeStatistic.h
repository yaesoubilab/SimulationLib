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
		long count;
		vector<double*> observations;
	};

}