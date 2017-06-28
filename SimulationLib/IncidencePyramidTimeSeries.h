#pragma once

#include <cmath>
#include "PyramidTimeSeries.h"

using namespace SimulationLib;

class IncidencePyramidTimeSeries : public PyramidTimeSeries {
public:
    IncidencePyramidTimeSeries (string name, int time0, int timeMax, \
                                int periodLength, int nCategories,   \
                                vector<double> ageBreaks)            \
      : PyramidTimeSeries(name, time0, timeMax, periodLength,        \
                          ceil(timeMax/periodLength), nCategories, ageBreaks) {}
private:
    int calcThisPeriod(int time, int periodLength);
};
