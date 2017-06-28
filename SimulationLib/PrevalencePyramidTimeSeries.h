#pragma once

#include <cmath>
#include "PyramidTimeSeries.h"

class PrevalencePyramidTimeSeries : public PyramidTimeSeries {
private:
    int calcNPeriods(int timeMax, int periodLength);
    int calcThisPeriod(int time, int periodLength);
};
