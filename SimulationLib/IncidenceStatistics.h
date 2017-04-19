#pragma once
#include <string>
#include <vector>

using namespace std;

namespace SimulationLib
{
	class IncidenceStatistics
	{
		public:
			string name;
			IncidenceStatistics(string inName);

		private:
			double mean, variance, min, max, total;
			long count;
			vector<double*> observations;
	};

}