#include "../include/SimulationLib/PyramidTimeSeries.h"

using namespace SimulationLib;
using namespace std;

// Implementing for Prevalence first
PyramidTimeSeries::PyramidTimeSeries(string _name, int _time0, int _timeMax, \
                                     int _periodLength,                      \
                                     function<int (int, int)> calcNPeriods,  \
                                     int _nCategories,  vector<double> _ageBreaks, \
                                     bool _reset)
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
    lastTime     = _time0;
    lastPeriod   = _time0 / _periodLength; // Janky...
    periodLength = _periodLength;
    nCategories  = _nCategories;
    ageBreaks    = _ageBreaks;
    reset        = _reset;

    int numValsPerPeriod = (ageBreaks.size() + 1) * nCategories;

    currentValues = new int[numValsPerPeriod];

    for (int i = 0; i < numValsPerPeriod; ++i)
        currentValues[i] = 0;

    closed = false;

    nPeriods     = calcNPeriods(timeMax, periodLength);

    pyramids     = std::vector< PyramidData<int> >();

    for (int i = 0; i < nPeriods; ++i)
        pyramids.push_back(PyramidData<int>(nCategories, ageBreaks));
}

bool PyramidTimeSeries::UpdateByAge(int time, int category, double age, int increment) {
    return UpdateByIdx(time, category, _getAgeIdx(age), increment);
}

bool PyramidTimeSeries::UpdateByIdx(int time, int category, int ageGroupIdx, int increment) {
    int thisPeriod;

    if (closed == true) {
        printf("Update failed: PyramidTimeSeries has been closed.\n");
        return false;
    }

    if (time < time0 || time > timeMax)
        throw out_of_range("time specified is < time0 or > timeMax");
    if (time < lastTime)
        throw out_of_range("time specified is < last time inputted");
    if (category < 0 || category >= nCategories)
        throw out_of_range("category specified is < 0 or >= nCategories");
    if (ageGroupIdx < 0)
        throw out_of_range("ageGroupIdx is < 0");
    if (ageGroupIdx > ageBreaks.size())
        throw out_of_range("ageGroupIdx is > number of age groups");

    // return false if updating will cause value to go below 0
    if (currentValues[category * (ageBreaks.size() + 1) + ageGroupIdx] + increment < 0)
        return false;

    thisPeriod = calcThisPeriod(time, periodLength);

    bool res = true;
    if (thisPeriod > lastPeriod) {
        //Check if thisPeriod is the period directly following last period
        if (thisPeriod == (lastPeriod + 1))
            res = _storeCurrentValues(lastPeriod);

        // Prevalence values are initially zeroed, so if thisPeriod is
        //   not the period directly following lastPeriod, the zeroes must
        //   be updated appropriately
        // This works for IncidencePTS as well, because currentValues is reset
        //   to all 0's after _storeCurrentValues is called, so calling
        //   _storeCurrentValues additional times will not changed the 0'd entries
        else {
            for (int i = lastPeriod;
                     i < thisPeriod;
                     ++i) {
                    _storeCurrentValues(i);
                }
        }
    }

    lastTime   = time;
    lastPeriod = thisPeriod;

    // printf("assigning %d change to currentValues[%d]\n", increment, category * (ageBreaks.size() + 1) + ageGroupIdx);
    currentValues[category * (ageBreaks.size() + 1) + ageGroupIdx] += increment;

    return res;
}

int PyramidTimeSeries::_getAgeIdx(double age) {
    unsigned long idx;

    if (age < 0)
        throw out_of_range("Age specified must be >= 0");

    idx = 0;
    for (; idx < ageBreaks.size(); ++idx)
        if (ageBreaks[idx] > age)
            break;

    return idx;
}

bool PyramidTimeSeries::_storeCurrentValues(int period) {
    // printf("_storeCurrentValues called for period #%d\n", period);
    bool success = true;
    int nAgeBreaks = ageBreaks.size() + 1;

    if (period >= nPeriods)
        return true;

    for (int i = 0; i < nCategories; ++i)
    {
        for (unsigned long j = 0; j < nAgeBreaks; ++j)
        {
            // printf("currentValues[%d] = %d\n", i*nAgeBreaks+j, currentValues[i*nAgeBreaks+j]);
            if (!pyramids[period].UpdateByIdx(i, j, currentValues[i * nAgeBreaks + j]))
                success = false;
            if (reset)
                currentValues[i * nAgeBreaks + j] = 0;
        }
    }

    return success;
}

bool PyramidTimeSeries::MoveByAge(int time, int oldCategory, double oldAge, \
                         int newCategory, double newAge, int increment) {
    int thisPeriod;

    if (closed == true) {
        printf("Update failed: PyramidTimeSeries has been closed.\n");
        return false;
    }

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
    if (increment < 0)
        throw out_of_range("increment moved was negative");

    thisPeriod = calcThisPeriod(time, periodLength);

    // If the MoveByAge period is the period currently stored in the buffer
    // update values in the buffer rather than the pyramid data
    if (thisPeriod == lastPeriod) {
        int oldAgeIdx, newAgeIdx, oldPopulation;
        oldAgeIdx = _getAgeIdx(oldAge);
        newAgeIdx = _getAgeIdx(newAge);

        oldPopulation = currentValues[oldCategory * (ageBreaks.size() + 1) + oldAgeIdx];

        // Check to see if 'increment' is l.t.e to population group size
        if (oldPopulation - increment < 0)
            return false;

        currentValues[oldCategory * (ageBreaks.size() + 1) + oldAgeIdx] -= increment;
        currentValues[newCategory * (ageBreaks.size() + 1) + newAgeIdx] += increment;
        return true;
    }
    else {
        return pyramids[thisPeriod] \
             .MoveByAge(oldCategory, oldAge, newCategory, newAge, increment);
    }
}

bool PyramidTimeSeries::MoveByIdx(int time, int oldCategory, int oldAgeGroupIdx, \
                         int newCategory, int newAgeGroupIdx, int increment) {
    int thisPeriod;

    if (closed == true) {
        printf("Update failed: PyramidTimeSeries has been closed.\n");
        return false;
    }

    if (time < time0 || time > timeMax)
        throw out_of_range("time specified is < time0 or > timeMax");
    if (oldCategory < 0 || oldCategory >= nCategories)
        throw out_of_range("oldCategory specified is < 0 or >= nCategories");
    if (newCategory < 0 || newCategory >= nCategories)
        throw out_of_range("newCategory specified is < 0 or >= nCategories");
    if (oldAgeGroupIdx < 0)
        throw out_of_range("oldAgeGroupIdx is < 0");
    if (oldAgeGroupIdx > ageBreaks.size())
        throw out_of_range("oldAgeGroupIdx is > number of age groups");
    if (newAgeGroupIdx < 0)
        throw out_of_range("newAgeGroupIdx is < 0");
    if (newAgeGroupIdx > ageBreaks.size())
        throw out_of_range("newAgeGroupIdx is > number of age groups");
    if (increment < 0)
        throw out_of_range("increment moved was negative");

    thisPeriod = calcThisPeriod(time, periodLength);

    if (thisPeriod == lastPeriod) {
        int oldPopulation;

        oldPopulation = currentValues[oldCategory * (ageBreaks.size() + 1) + oldAgeGroupIdx];

        // Check to see if 'increment' is l.t.e to population group size
        if (oldPopulation - increment < 0)
            return false;

        currentValues[oldCategory * (ageBreaks.size() + 1) + oldAgeGroupIdx] -= increment;
        currentValues[newCategory * (ageBreaks.size() + 1) + newAgeGroupIdx] += increment;
        return true;
    }
    else {
        return pyramids[thisPeriod] \
             .MoveByIdx(oldCategory, oldAgeGroupIdx, newCategory, newAgeGroupIdx, increment);
    }
}

int PyramidTimeSeries::_getTotalInCurrentValues(int periodIdx, \
                                                bool categoryBool, int categoryIdx, \
                                                bool ageGroupBool, int ageGroupIdx) {
    int currentValuesSize = (ageBreaks.size() + 1) * nCategories;

    if (categoryBool) {
        if (categoryIdx < 0 || categoryIdx >= nCategories)
            throw out_of_range("category specified is < 0 or >= nCategories");
    }
    if (ageGroupBool) {
        if (ageGroupIdx < 0)
            throw out_of_range("ageGroupIdx is < 0");
        if (ageGroupIdx > ageBreaks.size())
            throw out_of_range("ageGroupIdx is > number of age groups");
    }

    int total;
    total = 0;

    if (categoryBool && ageGroupBool)
        return currentValues[categoryIdx * (ageBreaks.size() + 1) + ageGroupIdx];
    else if (categoryBool) {
        // indexOffset calculates the number that needs to be added to the index
        // in order to pull the correct values from the currentValues array
        int indexOffset;
        indexOffset = categoryIdx * (ageBreaks.size() + 1);

        for (int i = 0; i < ageBreaks.size() + 1; ++i)
            total += currentValues[indexOffset + i];
        return total;
    }
    else if (ageGroupBool) {
        for (int i = ageGroupIdx; i < currentValuesSize; i += (ageBreaks.size() + 1))
            total += currentValues[i];
        return total;
    }
    else {
        for (int i = 0; i < currentValuesSize; ++i)
            total += currentValues[i];
        return total;
    }

}

int PyramidTimeSeries::GetTotalAtPeriod(int periodIdx) {
    if (!closed) {
        printf("Warning: PyramidTimeSeries not closed\n");
    }

    if (periodIdx >= nPeriods)
        throw out_of_range("periodIdx >= nPeriods");
    if (periodIdx < 0)
        throw out_of_range("periodIdx < 0");

    // Here we are checking to see whether the values we are searching for are in
    // the buffer currentValues or not. First we check if the periodIdx is equal
    // to the lastPeriod class variable. Futhermore, we must check that the
    // PyrmaidTimeSeries is not closed, because we only want to access the buffer
    // value if the Series hasn't been closed. Else we should access the stored
    // pyramid data values.
    if (periodIdx == lastPeriod && !closed)
        return _getTotalInCurrentValues(periodIdx, false, 0, false, 0);
    else
        return pyramids[periodIdx].GetTotal();
}

int PyramidTimeSeries::GetTotalAtTime(int time) {
    int periodIdx;

    if (!closed) {
        printf("Warning: PyramidTimeSeries not closed\n");
    }

    if (time < 0)
        throw out_of_range("time < 0");

    periodIdx = calcThisPeriod(time, periodLength);

    return GetTotalAtPeriod(periodIdx);
}


int PyramidTimeSeries::GetTotalInCategoryAtPeriod(int periodIdx, int category) {
    if (!closed) {
        printf("Warning: PyramidTimeSeries not closed\n");
    }

    if (periodIdx >= nPeriods)
        throw out_of_range("periodIdx >= nPeriods");
    if (periodIdx < 0)
        throw out_of_range("periodIdx < 0");
    if (category >= nCategories)
        throw out_of_range("category >= nCategories");
    if (category < 0)
        throw out_of_range("category < 0");

    if (periodIdx == lastPeriod && !closed)
        return _getTotalInCurrentValues(periodIdx, true, category, false, 0);
    else
        return pyramids[periodIdx].GetTotalInCategory(category);
}

int PyramidTimeSeries::GetTotalInCategoryAtTime(int time, int category) {
    int periodIdx;

    if (!closed) {
        printf("Warning: PyramidTimeSeries not closed\n");
    }

    if (time < 0)
        throw out_of_range("time < 0");

    periodIdx = calcThisPeriod(time, periodLength);

    return GetTotalInCategoryAtPeriod(periodIdx, category);
}


int PyramidTimeSeries::GetTotalInAgeGroupAtPeriod(int periodIdx, int ageGroupIdx) {
    if (!closed) {
        printf("Warning: PyramidTimeSeries not closed\n");
    }

    if (periodIdx >= nPeriods)
        throw out_of_range("periodIdx >= nPeriods");
    if (periodIdx < 0)
        throw out_of_range("periodIdx < 0");
    if ((unsigned long)ageGroupIdx >= ageBreaks.size() + 1)
        throw out_of_range("ageGroupIdx >= numAgeGroups");
    if (ageGroupIdx < 0)
        throw out_of_range("ageGroupIdx < 0");

    if (periodIdx == lastPeriod && !closed)
        return _getTotalInCurrentValues(periodIdx, false, 0, true, ageGroupIdx);
    else
        return pyramids[periodIdx].GetTotalInAgeGroup(ageGroupIdx);
}

int PyramidTimeSeries::GetTotalInAgeGroupAtTime(int time, int ageGroupIdx) {
    int periodIdx;

    if (!closed) {
        printf("Warning: PyramidTimeSeries not closed\n");
    }

    if (time < 0)
        throw out_of_range("time < 0");

    periodIdx = calcThisPeriod(time, periodLength);

    return GetTotalInAgeGroupAtPeriod(periodIdx, ageGroupIdx);
}

int PyramidTimeSeries::GetTotalInAgeGroupAndCategoryAtPeriod(int periodIdx, int ageGroupIdx, int category) {
    if (!closed) {
        printf("Warning: PyramidTimeSeries not closed\n");
    }

    if (periodIdx >= nPeriods)
        throw out_of_range("periodIdx >= nPeriods");
    if (periodIdx < 0)
        throw out_of_range("periodIdx < 0");
    if ((unsigned long)ageGroupIdx >= ageBreaks.size() + 1)
        throw out_of_range("ageGroupIdx >= numAgeGroups");
    if (ageGroupIdx < 0)
        throw out_of_range("ageGroupIdx < 0");
    if (category >= nCategories)
        throw out_of_range("category >= nCategories");
    if (category < 0)
        throw out_of_range("category < 0");

    if (periodIdx == lastPeriod && !closed)
        return _getTotalInCurrentValues(periodIdx, true, ageGroupIdx, true, category);
    else
        return pyramids[periodIdx].GetTotalInAgeGroupAndCategory(ageGroupIdx, category);
}

int PyramidTimeSeries::GetTotalInAgeGroupAndCategoryAtTime(int time, int ageGroupIdx, int category) {
    int periodIdx;

    if (!closed) {
        printf("Warning: PyramidTimeSeries not closed\n");
    }

    if (time < 0)
        throw out_of_range("time < 0");

    periodIdx = calcThisPeriod(time, periodLength);

    return GetTotalInAgeGroupAndCategoryAtPeriod(periodIdx, ageGroupIdx, category);
}

int PyramidTimeSeries::operator()(int t, int ageGroupIdx, int category) {
    return GetTotalInAgeGroupAndCategoryAtTime(t, ageGroupIdx, category);
}

void PyramidTimeSeries::Close(void) {
    closed = true;
    _storeCurrentValues(lastPeriod);

    return;
}

bool PyramidTimeSeries::IsWritable(void) {
    return !closed;
}

int PyramidTimeSeries::GetNumberCategories(void) {
    return nCategories;
}

vector<double> PyramidTimeSeries::GetAgeBreaks(void) {
    vector<double> copy;
    for (int i = 0; i < ageBreaks.size(); i++)
        copy.push_back(ageBreaks[i]);
    return copy;
}

int PyramidTimeSeries::GetPeriodLength(void) {
    return periodLength;
}

int PyramidTimeSeries::GetTimeMax(void) {
    return timeMax;
}

int PyramidTimeSeries::GetNPeriods(void) {
    return nPeriods;
}

int PyramidTimeSeries::GetTime0(void) {
    return time0;
}

string PyramidTimeSeries::GetName(void) {
    return name;
}

void PyramidTimeSeries::PrintPyramidTimeSeries(void) {
    for (int i = 0; i < nPeriods; ++i)
    {
        printf("This is period number %d\n", i);
        pyramids[i].PrintPyramid();
    }
}

void PyramidTimeSeries::PrintCurrentValues(void) {
    printf("Current values:");
    int currentValuesSize;
    currentValuesSize = (ageBreaks.size() + 1) * nCategories;

    for (int i = 0; i < currentValuesSize; ++i) {
        printf("%d, ", currentValues[i]);
    }
    printf("\n");
}
