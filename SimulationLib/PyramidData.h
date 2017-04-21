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

		PyramidData(int timeIndex, int numOfCategories, vector<double> ageBreaks); 
			// example age breaks of {0, 5, 10, 15} represent age groups [0,5), [5, 10), [10, 15), [15+)

		void Update(int category, double age, int increment);
			// update the change in the specified category and age group (note that it takes the actual age) 
		void Update(int oldCategory, int newCategory, double oldAge, double newAge, int numberMoved);	
			// update the change in the specified category and age group by taking the number of people who moved from one category-age group to another category-age group.
		int GetTotal();
			// return the total number of counts in all categories and all age groups
		int GetTotalInCategory(int categoryIndex);
			// return the total number of counts in the specified category
		int GetTotalInAgeGroup(double ageGroupIndex);
			// return the total number of counts in the specified age group
	private:
		// declare a two-dimensional vector to store counts in each age group in each category
	};
	
}