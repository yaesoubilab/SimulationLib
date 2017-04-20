#pragma once

#include <string>
#include <vector>

using namespace std;

namespace SimulationLib
{
	class PrevalencePopulationPyramid
	{
	public:
		string name;
		PrevalencePopulationPyramid(string name, double time, vector<double> ageBreaks);
		void Update(int sex, double age, int increment);
		void Move(int sex, double oldAge, double newAge);
		void RecalculateProportions();

	private:

	};

}