#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>

#include "PyramidData.h"

namespace SimulationLib
{
	// to store a collection of pyramid data collected during a simulation run
	class PyramidTimeSeries
	{
	private:
		string name;
		int time0;
		int timeMax;
		int periodLength;
		int nCategories;
		int nPeriods;

		vector<double> ageBreaks;

		PyramidData **pyramids;

		// This implemented in PrevalencePyramidTimeSeries and
		//   IncidencePyramidTimeSeries, abiding by the Template Method pattern
		//   described here: http://www.gotw.ca/publications/mill18.htm
		// Since this function can express the entire functional difference
		//   between Prevalence and Incidence, this pattern elegantly allows the
		//   specification of the two derived classes with minimal code.
		virtual int calcThisPeriod(int time, int periodLength) = 0;

	public:
		// Initializes a new PyramidTimeSeries.
		//
		// 'name': Name of the time series
		//
		// 'time0': Index of the earliest time entered in the series. All times
		//   entered into the PyramidTimeSeries must be >0 'time0'
		//
		// 'timeMax': The maximum time to be entered in the series. Must be
		//   >= 'time0'
		//
		// 'periodLength': The length of each aggregation period. Since times
		//   are currently specified as integers, a 'periodLength' of 1 will
		//   correspond to a period for each legal value of 'time'
		//
		// 'nCategories': The number of categories (e.g. sex, race) to allow
		//   specification of. Must be >= 1.
		//
		// 'ageBreaks': A vector of the points of division of age. An empty vector
		//   will correspond to persons of any age being lumped into the same
		//   bin. A vector of {10, 20} would correspond to three bins with the
		//   following support: [0, 10), [10, 20), [20, +inf]
		//
		// An out-of-range exception will be thrown for invalid specifications
		//   of any of these parameters
		PyramidTimeSeries(string name, int time0, int timeMax, int periodLength, \
						  int nPeriods, int nCategories, vector<double> ageBreaks);

		~PyramidTimeSeries();

		// Given a 'time'     (time0 <= time <= timeMax),
		// 		   'category' (0 <= category < nCategories)
		// 		   'age'	  (age >= 0), updates the data structure by changing
		//   the number of individuals in the specified group by 'increment'.
		//   Note that 'age' is translated into its respective age bin. Throws
		//   exceptions for illegal values, and returns false if 'increment'
		//   would cause the population of the group to vall below zero.
		bool UpdateByAge(int time, int category, double age, int increment);

		// Idential to UpdateByAge, except age is specified using age group index,
		//  rather than an age in years.
		bool UpdateByIdx(int time, int category, int ageGroupIdx, int increment);

		// Moves 'increment' individuals from the specified old group to the
		//   specified new group. Throws exceptions for illegal parameters.
		//   'increment' must be a non-negative integer. Returns false if the
		//   move would cause the number of individuals in the specified old
		//   group to become negative.
		bool MoveByAge(int time, int oldCategory, double oldAge, \
						         int newCategory, double newAge, int increment);

		// Idential to MoveByAge, but the old and new groups are specified
		//   by an age group index, rather than an age in years.
		bool MoveByIdx(int time, int oldCategory, int oldAgeGroupIdx, \
						         int newCategory, int newAgeGroupIdx, int increment);

		int GetTotalAtPeriod(int periodIdx);
		int GetTotalAtTime(int time);

		int GetTotalInCategoryAtPeriod(int periodIdx, int category);
		int GetTotalInCategoryAtTime(int time, int category);

		int GetTotalInAgeGroupAtPeriod(int periodIdx, int ageGroupIdx);
		int GetTotalInAgeGroupAtTime(int time, int ageGroupIdx);
	};
}
