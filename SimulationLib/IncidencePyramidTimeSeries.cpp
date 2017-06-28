#include "IncidencePyramidTimeSeries.h"

using namespace std;
using namespace SimulationLib;

int IncidencePyramidTimeSeries::calcThisPeriod(int time, int periodLength) {
    return (int)floor((double)time / (double)periodLength);
}
