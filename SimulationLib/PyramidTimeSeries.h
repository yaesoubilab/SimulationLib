#pragma once

#include <string>
#include <vector>
#include "PyramidData.h"

namespace SimulationLib
{
	// to store a collection of pyramid data collected during a simulation run
	class PyramidTimeSeries
	{
	public:
		PyramidTimeSeries(string name, int time0, int observationPeriodLength, int numOfCategories, vector<double> ageBreaks);
	private: 
		vector<PyramidData> pyramids; 
	};

	// to store a collection of prevalence data collected in the pyramid format during a simulation run
	class PrevalencePyramid : public PyramidTimeSeries
	{
	public:
		PrevalencePyramid(string name, int time0, int observationPeriodLength, int numOfCategories, vector<double> ageBreaks);

		void Update(int time, int category, double age, int increment);
		void Update(int time, int oldCategory, int newCategory, double oldAge, double newAge, int numberMoved);

	private:
		
	};

	// to store a collection of incidence data collected in the pyramid format during a simulation run
	class IncidenceTimeSeries : public PyramidTimeSeries
	{
	public:
		IncidenceTimeSeries(string name, int time0, int observationPeriodLength, int numOfCategories, vector<double> ageBreaks);

		void Update(int time, int category, double age, int increment);

	private:
	};
}
