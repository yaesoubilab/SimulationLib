#pragma once

#include <string>
#include <vector>

using namespace std;

namespace SimulationLib
{
	class PyramidData
	{

	private:
		int **popCounts;
		vector<double> ageBreaks;
	public:
		string name;
		int timeIndex;

		PyramidData(int timeIndex, int numOfCategories, vector<double> ageBreaks);

		void UpdateByIdx(int category, int ageGroupIndex, int increment);
		void UpdateByAge(int category, double age, int increment);
		// update the change in the specified category and age group (note that it takes the actual age)
		void MoveByIdx(int oldCategory, int oldAgeGroupIndex, int newCategory, int newAgeGroupIndex, int numberMoved);
		void MoveByAge(int oldCategory, double oldAge, int newCategory, double newAge, int numberMoved);

		int GetTotal(void);

		int GetTotalInCategory(int categoryIndex);

		int GetTotalInAgeGroup(int ageGroupIndex);
	};
}
