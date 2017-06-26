#pragma once

#include <string>
#include <vector>

using namespace std;

namespace SimulationLib
{
	class PyramidData
	{

	private:
		vector<vector<int>> popCounts;
		vector<double> ageBreaks;
	public:
		string name;
		int timeIndex;

		PyramidData(int timeIndex, int numOfCategories, vector<double> ageBreaks);

		void Update(int category, int ageGroupIndex, int increment);
		void Update(int category, double age, int increment);
		// update the change in the specified category and age group (note that it takes the actual age)
		void Update(int oldCategory, int newCategory, int oldAgeGroupIndex, int newAgeGroupIndex, int numberMoved);
		void Update(int oldCategory, int newCategory, double oldAge, double newAge, int numberMoved);

		int GetTotal(void);

		int GetTotalInCategory(int categoryIndex);

		int GetTotalInAgeGroup(int ageGroupIndex);
	};
}
