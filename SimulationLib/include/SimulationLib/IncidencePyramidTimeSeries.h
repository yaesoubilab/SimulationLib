#pragma once

#include <cmath>
#include <functional>
#include <string>
#include "PyramidTimeSeries.h"

using namespace std;
using namespace SimulationLib;

class IncidencePyramidTimeSeries : public PyramidTimeSeries {
public:
    // Initializes a new IncidencePyramidTimeSeries.
    //
    // 'name': Name of the time series
    //
    // 'time0': Index of the earliest time entered in the series. All times
    //   entered into the PyramidTimeSeries must be >0 'time0'
    //
    // 'timeMax': The maximum time to be entered in the series. Must be
    //   >= 'time0'
    //
    // 'periodLength': The length of each aggregation period. Since times
    //   are currently specified as integers, a 'periodLength' of 1 will
    //   correspond to a period for each legal value of 'time'
    //
    // 'nCategories': The number of categories (e.g. sex, race) to allow
    //   specification of. Must be >= 1.
    //
    // 'ageBreaks': A vector of the points of division of age. An empty vector
    //   will correspond to persons of any age being lumped into the same
    //   bin. A vector of {10, 20} would correspond to three bins with the
    //   following support: [0, 10), [10, 20), [20, +inf]
    //
    // An out-of-range exception will be thrown for invalid specifications
    //   of any of these parameters
    IncidencePyramidTimeSeries (string name, int time0, int timeMax, \
                                int periodLength, int nCategories,   \
                                vector<double> ageBreaks)            \
      : PyramidTimeSeries(name, time0, timeMax, periodLength,        \
                          [] (int tMax, int pLength) -> int          \
                             {return ceil(tMax/pLength);},           \
                          nCategories, ageBreaks) {}
      // ^(Note use of lambda function in call to PyramidTimeSeries constructor)


private:
    int calcThisPeriod(int time, int periodLength);
};
