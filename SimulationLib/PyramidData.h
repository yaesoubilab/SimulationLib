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

		PyramidData(int timeIndex, int numOfCategories, vector<double> ageBreaks) : timeIndex{ timeIndex }, ageBreaks{ ageBreaks }
		{
			// example age breaks of {0, 5, 10, 15} represent age groups [0,5), [5, 10), [10, 15), [15+)
			for (int j = 0; j < numOfCategories; ++j) {
				popCounts.push_back(vector<int>(ageBreaks.size(), 0));
			}
		};
			

		void Update(int category, double age, int increment) {
			
			int index;
			if (age < ageBreaks[0]) {
				popCounts[category][0] += increment;
			}
			
			else {
				index = 0;
				while (index + 1 < ageBreaks.size() && ageBreaks[index + 1]) {
					++index;
				}
				popCounts[category][index] += increment;
			}
			
		
		};
			// update the change in the specified category and age group (note that it takes the actual age) 
		void Update(int oldCategory, int newCategory, double oldAge, double newAge, int numberMoved) {
			// update the change in the specified category and age group by taking the number of people who 
			//moved from one category-age group to another category-age group.
			
			int oldAgeInd = 0;
			if (oldAge >= ageBreaks[0]){
				while (oldAgeInd + 1 < ageBreaks.size() && ageBreaks[oldAgeInd + 1] < oldAge) {
					++oldAgeInd;
				}
			}
			popCounts[oldCategory][oldAgeInd] -= numberMoved;
			
			int newAgeInd = 0;
			if (newAge < ageBreaks[0]) {
				while (newAgeInd + 1 < ageBreaks.size() && ageBreaks[newAgeInd + 1] < newAge) {
					++newAgeInd;
				}
			}
			popCounts[newCategory][newAgeInd] += numberMoved;
		
		}
		int GetTotal() {
			// return the total number of counts in all categories and all age groups
			int total = 0;
			for (int j = 0; j < ageBreaks.size(); ++j) {
				for (int k = 0; k < popCounts.size(); ++k) {
					total += popCounts[k][j];
				}
			}

			return total;

		}

		int GetTotalInCategory(int categoryIndex) {
			// return the total number of counts in the specified category
			int total = 0;
			for (int j = 0; j < ageBreaks.size(); ++j) {
				total += popCounts[categoryIndex][j];
			}

		}
		int GetTotalInAgeGroup(int ageGroupIndex) {
			//originally had double ageGroupIndex. I was confused so wrote two functions. 
			int total = 0;
			for (int j = 0; j < popCounts.size(); ++j) {
				total += popCounts[j][ageGroupIndex];
			}
		}

		int GetTotalInAgeGroup(int age) {
			int ageIndex = 0;

			while (ageIndex < ageBreaks.size() - 1 && ageBreaks[ageIndex+1] < age) {
				++ageIndex;

			}

			int total = GetTotalInAgeGroup(ageIndex);

		}
			// return the total number of counts in the specified age group
	//private:
		// declare a two-dimensional vector to store counts in each age group in each category
		
	};
	
}