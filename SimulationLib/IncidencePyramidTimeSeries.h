#pragma once

#include "PyramidTimeSeries.h"

class IncidencePyramidTimeSeries : public PyramidTimeSeries {
private:
    int calcNPeriods(int timeMax, int periodLength);
    int calcThisPeriod(int time, int periodLength);
};
