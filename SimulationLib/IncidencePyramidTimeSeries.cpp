#include "IncidencePyramidTimeSeries.h"

using namespace std;
using namespace SimulationLib;


int IncidencePyramidTimeSeries::calcThisPeriod(int time, int periodLength) {
    return (int)ceil((double)time / (double)periodLength);
}

void IncidencePyramidTimeSeries::initializeZeroes(void) {
	int nCategories = GetNumberCategories();
	int nAgeBreaks = GetAgeBreaks().size();

	int numValsPerPeriod = (nAgeBreaks + 1) * nCategories;

	for (int i = 0; i < nCategories; ++i)
    {
        for (unsigned long j = 0; j < nAgeBreaks; ++j)
            UpdateByIdx(0,i,j,0);
    }
}
