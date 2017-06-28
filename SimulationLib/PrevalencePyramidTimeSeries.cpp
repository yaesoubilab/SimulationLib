#include "PrevalencePyramidTimeSeries.h"

using namespace SimulationLib;

PrevalencePyramidTimeSeries::calcNPeriods(int timeMax, int periodLength) {
    return   (timeMax % periodLength == 0) \
           ? (timeMax/periodLength + 1)    \
           : ceil(timeMax/periodLength);
}

PrevalencePyramidTimeSeries::calcThisPeriod(int time, int periodLength) {
    return (int)ceil((double)time / (double)periodLength);
}
