#pragma once

#include <string>
#include <vector>

using namespace std;

namespace SimulationLib
{
	class PyramidData
	{
	public:
		string name;
		int timeIndex;

		PyramidData(int timeIndex, int numOfCategories, vector<double> ageBreaks); // example age breaks of {0, 5, 10, 15} represent age groups [0,5), [5, 10), [10, 15), [15+)

		void Update(int category, double age, int increment);
		void Update(int oldCategory, int newCategory, double oldAge, double newAge, int numberMoved);
		
		int GetTotal();
		int GetTotalInCategory(int categoryIndex);
		int GetTotalInAgeGroup(double ageGroupIndex);
		void CalculateProportions();
	private:
		// declare a two-dimensional vector to store counts in each age group in each category
	};
	
}