#pragma once

#include <cmath>
#include "PyramidTimeSeries.h"

using namespace SimulationLib;

class PrevalencePyramidTimeSeries : public PyramidTimeSeries {
public:
    PrevalencePyramidTimeSeries (string name, int time0, int timeMax, \
                                 int periodLength, int nCategories,   \
                                 vector<double> ageBreaks)            \
      : PyramidTimeSeries(name, time0, timeMax, periodLength,         \
                            (timeMax%periodLength==0)                 \
                          ? (timeMax/periodLength + 1)                \
                          : ceil(timeMax/periodLength),               \
                          nCategories, ageBreaks) {}
private:
    int calcThisPeriod(int time, int periodLength);
};
