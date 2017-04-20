#pragma once
#include <string>
#include <vector>

using namespace std;

namespace SimulationLib
{
	class PrevalenceTimeSeries
	{
	public:
		string name;
		PrevalenceTimeSeries(string name);
		void Record(int time, int increment);
		
		vector<int> GetObservations();

	private:
	};
}

