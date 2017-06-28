#include "IncidencePyramidTimeSeries.h"

using namespace SimulationLib;

IncidencePyramidTimeSeries::calcNPeriods(int timeMax, int periodLength) {
    return ceil(timeMax/periodLength);
}

IncidencePyramidTimeSeries::calcThisPeriod(int time, int periodLength) {
    return (int)floor((double)time / (double)periodLength);
}
