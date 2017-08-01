#pragma once

#include <function>

using namespace std;

namespace SimulationLib
{
	class Likelihood
	{

	private:

	public:
		TimeSeries *TS;

		// doesn't currently exist
		Distribution *distName;



		Likelihood(TimeSeries *TS, Distribuition *distName);

		function<double(double,int)> CreateFunction(void);


	};
}
