#include "PyramidTimeSeries.h"

using namespace SimulationLib;
using namespace std;

// Implementing for Prevalence first
PyramidTimeSeries::PyramidTimeSeries(string _name, int _time0, int _timeMax, \
                                     int _periodLength,                      \
                                     function<int (int, int)> calcNPeriods,  \
                                     int _nCategories,  vector<double> _ageBreaks)
{
    if (_time0 < 0)
        throw out_of_range("time0 must be >= 0");
    if (_periodLength <= 0)
        throw out_of_range("periodLength must be > 0");
    if (_timeMax < _periodLength)
        throw out_of_range("timeMax must be >= periodLength");
    if (_time0 > _timeMax)
        throw out_of_range("timeMax must be >= time0");
    if (_nCategories < 1)
        throw out_of_range("nCategories must be > 0");

    name         = _name;
    time0        = _time0;
    timeMax      = _timeMax;
    periodLength = _periodLength;
    nCategories  = _nCategories;
    ageBreaks    = _ageBreaks;

    nPeriods     = calcNPeriods(timeMax, periodLength);

    pyramids     = new PyramidData *[nPeriods];

    for (int i = 0; i < nPeriods; ++i)
        pyramids[i] = new PyramidData(nCategories, ageBreaks);
}


PyramidTimeSeries::~PyramidTimeSeries()
{
    for (int i = 0; i < nPeriods; ++i)
        delete pyramids[i];

    delete pyramids;
}

bool PyramidTimeSeries::UpdateByAge(int time, int category, double age, int increment) {
    int thisPeriod;

    if (time < time0 || time > timeMax)
        throw out_of_range("time specified is < time0 or > timeMax");
    if (category < 0 || category >= nCategories)
        throw out_of_range("category specified is < 0 or >= nCategories");
    if (age < 0)
        throw out_of_range("age is < 0");

    thisPeriod = calcThisPeriod(time, periodLength);

    return pyramids[thisPeriod]->UpdateByAge(category, age, increment);
}

bool PyramidTimeSeries::UpdateByIdx(int time, int category, int ageGroupIdx, int increment) {
    int thisPeriod;

    if (time < time0 || time > timeMax)
        throw out_of_range("time specified is < time0 or > timeMax");
    if (category < 0 || category >= nCategories)
        throw out_of_range("category specified is < 0 or >= nCategories");
    if (ageGroupIdx < 0)
        throw out_of_range("ageGroupIdx is < 0");

    thisPeriod = calcThisPeriod(time, periodLength);

    return pyramids[thisPeriod]->UpdateByIdx(category, ageGroupIdx, increment);
}

bool PyramidTimeSeries::MoveByAge(int time, int oldCategory, double oldAge, \
                         int newCategory, double newAge, int increment) {
    int thisPeriod;

    if (time < time0 || time > timeMax)
        throw out_of_range("time specified is < time0 or > timeMax");
    if (oldCategory < 0 || oldCategory >= nCategories)
        throw out_of_range("oldCategory specified is < 0 or >= nCategories");
    if (newCategory < 0 || newCategory >= nCategories)
        throw out_of_range("newCategory specified is < 0 or >= nCategories");
    if (oldAge < 0)
        throw out_of_range("oldAge is < 0");
    if (newAge < 0)
        throw out_of_range("newAge is < 0");

    thisPeriod = calcThisPeriod(time, periodLength);

    return pyramids[thisPeriod] \
             ->MoveByAge(oldCategory, oldAge, newCategory, newAge, increment);
}

bool PyramidTimeSeries::MoveByIdx(int time, int oldCategory, int oldAgeGroupIdx, \
                         int newCategory, int newAgeGroupIdx, int increment) {
    int thisPeriod;

    if (time < time0 || time > timeMax)
        throw out_of_range("time specified is < time0 or > timeMax");
    if (oldCategory < 0 || oldCategory >= nCategories)
        throw out_of_range("oldCategory specified is < 0 or >= nCategories");
    if (newCategory < 0 || newCategory >= nCategories)
        throw out_of_range("newCategory specified is < 0 or >= nCategories");
    if (oldAgeGroupIdx < 0)
        throw out_of_range("oldAgeGroupIdx is < 0");
    if (newAgeGroupIdx < 0)
        throw out_of_range("newAgeGroupIdx is < 0");

    thisPeriod = calcThisPeriod(time, periodLength);

    return pyramids[thisPeriod] \
             ->MoveByAge(oldCategory, oldAgeGroupIdx, newCategory, newAgeGroupIdx, increment);

}

int PyramidTimeSeries::GetTotalAtPeriod(int periodIdx) {
    if (periodIdx >= nPeriods)
        throw out_of_range("periodIdx >= nPeriods");
    if (periodIdx < 0)
        throw out_of_range("periodIdx < 0");

    return pyramids[periodIdx]->GetTotal();
}

int PyramidTimeSeries::GetTotalAtTime(int time) {
    int periodIdx;

    if (time < 0)
        throw out_of_range("time < 0");

    periodIdx = calcThisPeriod(time, periodLength);

    return GetTotalAtPeriod(periodIdx);
}


int PyramidTimeSeries::GetTotalInCategoryAtPeriod(int periodIdx, int category) {
    if (periodIdx >= nPeriods)
        throw out_of_range("periodIdx >= nPeriods");
    if (periodIdx < 0)
        throw out_of_range("periodIdx < 0");
    if (category >= nCategories)
        throw out_of_range("category >= nCategories");
    if (category < 0)
        throw out_of_range("category < 0");

    return pyramids[periodIdx]->GetTotalInCategory(category);
}

int PyramidTimeSeries::GetTotalInCategoryAtTime(int time, int category) {
    int periodIdx;

    if (time < 0)
        throw out_of_range("time < 0");

    periodIdx = calcThisPeriod(time, periodLength);

    return GetTotalInCategoryAtPeriod(periodIdx, category);
}


int PyramidTimeSeries::GetTotalInAgeGroupAtPeriod(int periodIdx, int ageGroupIdx) {
    if (periodIdx >= nPeriods)
        throw out_of_range("periodIdx >= nPeriods");
    if (periodIdx < 0)
        throw out_of_range("periodIdx < 0");
    if ((unsigned long)ageGroupIdx >= ageBreaks.size() + 1)
        throw out_of_range("ageGroupIdx >= numAgeGroups");
    if (ageGroupIdx < 0)
        throw out_of_range("ageGroupIdx < 0");

    return pyramids[periodIdx]->GetTotalInAgeGroup(ageGroupIdx);
}

int PyramidTimeSeries::GetTotalInAgeGroupAtTime(int time, int ageGroupIdx) {
    int periodIdx;

    if (time < 0)
        throw out_of_range("time < 0");

    periodIdx = calcThisPeriod(time, periodLength);

    return GetTotalInAgeGroupAtPeriod(periodIdx, ageGroupIdx);
}
