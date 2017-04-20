#pragma once
#include <string>
#include <vector>

using namespace std;

namespace SimulationLib
{
	class IncidenceStatistic
	{
		public:
			string name;
			IncidenceStatistic(string inName);
			void Record(double value);

		private:
			double mean, variance, min, max, total;
			long count;
			vector<double*> observations;
	};

}