#pragma once

#include <string>
#include "StatisticalDistribution.h"

using namespace std;

namespace SimulationLib
{

	class Parameter : public StatisticalDistributions::StatisticalDistribution
	{
	public: 
		Parameter(string name);
		string name;
		double currentSample; 
	private:
	};
}