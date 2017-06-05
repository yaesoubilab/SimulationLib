#pragma once

#include <string>
#include <vector>
#include "PyramidData.h"

namespace SimulationLib
{
	// to store a collection of pyramid data collected during a simulation run
	class PyramidTimeSeries
	{
	private:
		string name; 
		int time0;
		vector<PyramidData> pyramids;

	public:
		PyramidTimeSeries(string name, int time0, int observationPeriodLength, int numOfCategories, vector<double> ageBreaks) : name{ name } {
			// create a new pyramid and add this to the vector of pyramids
			
		}

	};

	// to store a collection of prevalence data collected in the pyramid format during a simulation run
	class PrevalencePyramidTimeSeries : public PyramidTimeSeries
	{
	private:
		string name;
	public:
		PrevalencePyramidTimeSeries(string name, int time0, int observationPeriodLength, int numOfCategories, vector<double> ageBreaks) : name{ name } {
		}

		void Update(int time, int category, double age, int increment);
		{
			// TODO: modify the arguments to match your implementation of pyramidData
			// check if time > lastTimePyramidCreated +  observationPeriodLength
				// create a new pyramid and populate it with old pyramid data
			// else
				// update old pyramid
		}
		void Update(int time, int oldCategory, int newCategory, double oldAge, double newAge, int numberMoved);

	private:
		
	};

	// to store a collection of incidence data collected in the pyramid format during a simulation run
	class IncidencePyramidTimeSeries : public PyramidTimeSeries
	{
	public:
		IncidencePyramidTimeSeries(string name, int time0, int observationPeriodLength, int numOfCategories, vector<double> ageBreaks);

		void Update(int time, int category, double age, int increment);
		{
			// TODO: modify the arguments to match your implementation of pyramidData
			// check if time > lastTimePyramidCreated +  observationPeriodLength
				// create a new empty pyramid 
			// else
				// update old pyramid
		}

	private:
	};
}
