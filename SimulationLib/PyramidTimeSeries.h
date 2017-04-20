#pragma once

#include <string>
#include <vector>
#include "PyramidData.h"

namespace SimulationLib
{
	class PyramidTimeSeries
	{
	public:
		PyramidTimeSeries(string name, int time0, int observationPeriodLength, int numOfCategories, vector<double> ageBreaks);
	private: 
		vector<PyramidData> pyramids; 
	};

	class PrevalencePyramid : public PyramidTimeSeries
	{
	public:
		PrevalencePyramid(string name, int time0, int observationPeriodLength, int numOfCategories, vector<double> ageBreaks);

		void Update(int time, int category, double age, int increment);
		void Update(int time, int oldCategory, int newCategory, double oldAge, double newAge, int numberMoved);

	private:
		
	};

	class IncidenceTimeSeries : public PyramidTimeSeries
	{
	public:
		IncidenceTimeSeries(string name, int time0, int observationPeriodLength, int numOfCategories, vector<double> ageBreaks);

		void Update(int time, int category, double age, int increment);

	private:
	};
}
