#include "PrevalencePyramidTimeSeries.h"

using namespace std;
using namespace SimulationLib;

int PrevalencePyramidTimeSeries::calcThisPeriod(int time, int periodLength) {
    return (int)ceil((double)time / (double)periodLength);
}
