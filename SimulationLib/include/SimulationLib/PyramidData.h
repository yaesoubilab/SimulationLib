#pragma once

#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

namespace SimulationLib
{
	template <typename T>
	class PyramidData
	{

	private:
		std::vector< std::vector< T > > popCounts;

		int numCategories;
		int numAgeGroups;
		vector<double> ageBreaks;

		int getAgeIdx(double age);
	public:
		using value_type      = T;
		using query_type      = std::tuple<int, int>;
		using query_signature = T(int, int);

		string name;

		// Initializes a PyramidData class, allocating memory for
		//   (numCategories x size(ageBreaks)) values.
		//
		// 'numCategories': the number of categories the population is segmented
		//   into. Must be >0
		//
		// 'ageBreaks': a vector defining the intervals of age along the spectrum
		//   from [0, +inf]. A vector of {0, 5, 10} corresponds to the age groups
		//   [0, 5), [5, 10), [10, +inf]. If an empty vector is passed, each
		//   population category will be binned into one age group of bounds
		//   [0, +inf]. Each member of the vector must be a positive, nonzero
		//   double.
		PyramidData(int numCategories, vector<double> ageBreaks);

		// Updates the specified category and age group index by 'increment'.
		// Returns true on success, and false if the increment would cause
		//   the size of the specified population to become negative. Values of
		//   'category' and 'ageGroupIndex' which are out-of-bounds will result
		//   in an exception being thrown.
		bool UpdateByIdx(int category, int ageGroupIndex, T increment);

		// The same, except age is specified directly, and subsequently translated
		//   into the correct age group.
		bool UpdateByAge(int category, double age, T increment);

		// update the change in the specified category and age group (note that it takes the actual age)

		// Moves 'numberMoved' individuals from an old category and age group
		//   to a new category and age group. Returns true on success, and
		//   false if either the 'numberMoved' is greater than the size of
		//   the referenced 'old' population.
		// Throws an exception if either pair of indices specify a population
		//   group that does not exist, or if 'numberMoved' is a negative
		//   integer.
		bool MoveByIdx(int oldCategory, int oldAgeGroupIndex, \
					   int newCategory, int newAgeGroupIndex, T numberMoved);

		// Similar, except age is explicitly specified and translated internally
		//   into its corresponding age group.
		bool MoveByAge(int oldCategory, double oldAge, \
					   int newCategory, double newAge, T numberMoved);

		// Returns the total number of individuals across all categories and
		//   age groups
		value_type GetTotal(void);

		// Returns the total number of individuals in the specified category.
		// Throws exception for invalid 'categoryIndex'.
		value_type GetTotalInCategory(int categoryIndex);

		// Returns the total number of individuals in the specified age group.
		// Throws exception for invalid 'ageGroupIndex'.
		value_type GetTotalInAgeGroup(int ageGroupIndex);

		// Returns the total number of individuals in the specified category and age group.
		value_type GetTotalInAgeGroupAndCategory(int ageGroupIndex, int categoryIndex);

		value_type operator()(int ageGroupIndex, int categoryIndex);

		size_t GetNumCategories(void);
		vector<double> GetAgeBreaks(void);

		// For debugging, prints out the content of the Pyramid data structure.
		void PrintPyramid(void);
	};
}
