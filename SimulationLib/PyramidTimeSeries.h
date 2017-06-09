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
		PyramidTimeSeries(string name, int time0, int observationPeriodLength,
			int numOfCategories, vector<double> ageBreaks) : name{ name } {
			// create a new pyramid and add this to the vector of pyramids
			PyramidData new_pyramid(time0, numOfCategories, ageBreaks);
			pyramids.push_back(new_pyramid);
	};

	// to store a collection of prevalence data collected in the pyramid format during a simulation run
	class PrevalencePyramidTimeSeries : public PyramidTimeSeries
	{
	private:
		string name;
	public:
		PrevalencePyramidTimeSeries(string name, int time0, int observationPeriodLength,
			int numOfCategories, vector<double> ageBreaks) : name{ name } {
				PyramidTimeSeries new_prevalence_series(name, time0, observationPeriodLength, numOfCategories, ageBreaks);
		}

		void Update(int time, int category, double ageGroupIndex, int increment);
		{
			if (time > new_prevalence_series.pyramids.back().timeIndex + observationPeriodLength) {
				PyramidTimeSeries add_prevalence_series(name, time0, observationPeriodLength, numOfCategories, ageBreaks);
				new_prevalence_series.pyramids.back() = new_prevalence_series.pyramids.end()[-2];
				new_prevalence_series.pyramids.back()[category][ageGroupIndex] += increment;
			} else {
				new_prevalence_series.pyramids.back()[category][ageGroupIndex] += increment;
			}
		}

		void Update(int time, int oldCategory, int newCategory, int oldAgeGroupIndex, int newAgeGroupIndex, int numberMoved) {
			if (time > new_prevalence_series.pyramids.back().timeIndex + observationPeriodLength) {
				PyramidTimeSeries add_prevalence_series(name, time0, observationPeriodLength, numOfCategories, ageBreaks);
				new_prevalence_series.pyramids.back() = new_prevalence_series.pyramids..end()[-2];
				new_prevalence_series.pyramids.back()[oldCategory][oldAgeGroupIndex] -= numberMoved;
				new_prevalence_series.pyramids.back()[newCategory][newAgeGroupIndex] += numberMoved;
			} else {
				new_prevalence_series.pyramids.back()[oldCategory][oldAgeGroupIndex] -= numberMoved;
				new_prevalence_series.pyramids.back()[newCategory][newAgeGroupIndex] += numberMoved;
			}
		};

	private:

	};

	// to store a collection of incidence data collected in the pyramid format during a simulation run
	class IncidencePyramidTimeSeries : public PyramidTimeSeries
	{
	public:
		IncidencePyramidTimeSeries(string name, int time0, int observationPeriodLength, int numOfCategories, vector<double> ageBreaks) {
				PyramidTimeSeries new_incidence_series(name, time0, observationPeriodLength, numOfCategories, ageBreaks)
		};

		void Update(int time, int category, double ageGroupIndex, int increment);
		{
			if (time > new_prevalence_series.pyramids.back().timeIndex + observationPeriodLength) {
				PyramidTimeSeries add_prevalence_series(name, time0, observationPeriodLength, numOfCategories, ageBreaks);
				new_prevalence_series.pyramids.back()[category][ageGroupIndex] += increment;
			} else {
				new_prevalence_series.pyramids.back()[category][ageGroupIndex] += increment;
			}
		}

		void Update(int time, int oldCategory, int newCategory, int oldAgeGroupIndex, int newAgeGroupIndex, int numberMoved) {
			if (time > new_prevalence_series.pyramids.back().timeIndex + observationPeriodLength) {
				PyramidTimeSeries add_prevalence_series(name, time0, observationPeriodLength, numOfCategories, ageBreaks);
				new_prevalence_series.pyramids.back()[oldCategory][oldAgeGroupIndex] -= numberMoved;
				new_prevalence_series.pyramids.back()[newCategory][newAgeGroupIndex] += numberMoved;
			} else {
				new_prevalence_series.pyramids.back()[oldCategory][oldAgeGroupIndex] -= numberMoved;
				new_prevalence_series.pyramids.back()[newCategory][newAgeGroupIndex] += numberMoved;
			}
		};

	private:

	};

}
