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

	public:
		PyramidTimeSeries(string name, int time0, int timeMax, int periodLength,
						  int nCategories, vector<double> ageBreaks);

		~PyramidTimeSeries();

		bool UpdateByAge(int time, int category, double age, int increment);
		bool UpdateByIdx(int time, int category, int ageGroupIdx, int increment);

		bool MoveByAge(int time, int oldCategory, double oldAge, \
						         int newCategory, double newAge, int increment);

		bool MoveByIdx(int time, int oldCategory, int oldAgeGroupIdx, \
						         int newCategory, int newAgeGroupIdx, int increment);
	};

	class IncidencePyramidTimeSeries : public PyramidTimeSeries {

	};

	class PrevalencePyramidTimeSeries : public PyramidTimeSeries {

	};
}
