#include "PyramidData.h"

using namespace std;
using namespace SimulationLib;

PyramidData::PyramidData(int _numCategories, vector<double> _ageBreaks)
{
    numAgeGroups  = _ageBreaks.size() + 1;
    numCategories = _numCategories;
    ageBreaks     = _ageBreaks;

    // Check to make sure at least one category requested
    if (numCategories < 1)
        throw out_of_range("Must have >0 categories");

    // Check to make sure ageBreaks is monotonically increasing, and that
    // the first age-break is greater than zero
    double lastAge = 0;
    for (unsigned long i = 0; i < ageBreaks.size(); ++i)
    {
        if (ageBreaks[i] <= 0 || ageBreaks[i] <= lastAge)
            throw out_of_range("'ageBreaks' must be monotonically increasing and >0");
        lastAge = ageBreaks[i];
    }

    // Allocate all memory for popCounts
    popCounts = new int *[numCategories];
    for (int i = 0; i < numCategories; ++i)
    {
        popCounts[i] = new int[numAgeGroups];
    }

    // Zero out all population groups
    for (int i = 0; i < numCategories; ++i)
    {
        for (int j = 0; j < numAgeGroups; ++j)
        {
            popCounts[i][j] = 0;
        }
    }
}

PyramidData::~PyramidData(void)
{
    // Free each category array
    for (int i = 0; i < numCategories; ++i)
    {
        delete popCounts[i];
    }

    // Free the array itself
    delete popCounts;
}

// Returns the age group index corresponding to age 'age'. Throws exception
//   if 'age' is below zero.
int PyramidData::getAgeIdx(double age) {
    unsigned long idx;

    if (age < 0)
        throw out_of_range("Age specified must be >= 0");

    idx = 0;
    for (; idx < ageBreaks.size(); ++idx)
        if (ageBreaks[idx] > age)
            break;

    return idx;
}

bool PyramidData::UpdateByIdx(int category, int ageGroupIndex, int increment) {
    int currentPopulation;

    if (category < 0 || category >= numCategories)
        throw out_of_range("category was negative or greater than #categories-1");
    if (ageGroupIndex < 0 || ageGroupIndex >= numAgeGroups)
        throw out_of_range("ageGroupIndex was negative or greater than #ageGroups-1");

    currentPopulation = popCounts[category][ageGroupIndex];

    // Check to make sure 'increment' will not bring population into the negative
    if (currentPopulation + increment < 0)
        return false;

    popCounts[category][ageGroupIndex] += increment;
    return true;
}

bool PyramidData::UpdateByAge(int category, double age, int increment) {
    int ageGroupIndex;
    try { ageGroupIndex = getAgeIdx(age); } catch (...) { throw; }

    return UpdateByIdx(category, ageGroupIndex, increment);
}

// update the change in the specified category and age group (note that it takes the actual age)
bool PyramidData::MoveByIdx(int oldCategory, int oldAgeGroupIndex, int newCategory, int newAgeGroupIndex, int numberMoved) {
    int oldPopulation;

    if (numberMoved < 0)
        throw out_of_range("numberMoved was negative");
    if (oldCategory < 0 || oldCategory >= numCategories)
        throw out_of_range("oldCategory was negative or greater than #categories-1");
    if (newCategory < 0 || newCategory >= numCategories)
        throw out_of_range("newCategory was negative or greater than #categories-1");
    if (oldAgeGroupIndex < 0 || oldAgeGroupIndex >= numAgeGroups)
        throw out_of_range("oldAgeGroupIndex was negative or greater than #ageGroups-1");
    if (newAgeGroupIndex < 0 || newAgeGroupIndex >= numAgeGroups)
        throw out_of_range("newAgeGroupIndex was negative or greater than #ageGroups-1");

    oldPopulation = popCounts[oldCategory][oldAgeGroupIndex];

    // Check to see if 'numberMoved' is l.t.e to population group size
    if (oldPopulation - numberMoved < 0)
        return false;

    popCounts[oldCategory][oldAgeGroupIndex] -= numberMoved;
    popCounts[newCategory][newAgeGroupIndex] += numberMoved;
    return true;
}

bool PyramidData::MoveByAge(int oldCategory, double oldAge, int newCategory, double newAge, int numberMoved) {
    int oldAgeIdx, newAgeIdx;

    try { oldAgeIdx = getAgeIdx(oldAge); } catch (...) { throw; }
    try { newAgeIdx = getAgeIdx(newAge); } catch (...) { throw; }

    return MoveByIdx(oldCategory, oldAgeIdx, newCategory, newAgeIdx, numberMoved);
}

int PyramidData::GetTotal(void) {
    int total;

    total = 0;
    for (int i = 0; i < numCategories; ++i)
        for (int j = 0; j < numAgeGroups; ++j)
            total += popCounts[i][j];

    return total;
}

int PyramidData::GetTotalInCategory(int categoryIndex) {
    int total;

    if (categoryIndex < 0 || categoryIndex >= numCategories)
        throw out_of_range("categoryIndex was negative or greater than #categories-1");

    total = 0;
    for (int i = 0; i < numAgeGroups; ++i)
        total += popCounts[categoryIndex][i];

    return total;
}


int PyramidData::GetTotalInAgeGroup(int ageGroupIndex) {
    int total;

    if (ageGroupIndex < 0 || ageGroupIndex >= numAgeGroups)
        throw out_of_range("ageGroupIndex was negative or greater than #ageGroups-1");

    total = 0;
    for (int i = 0; i < numCategories; ++i)
        total += popCounts[i][ageGroupIndex];

    return total;
}

int PyramidData::GetTotalInAgeGroupAndCategory(int ageGroupIndex, int categoryIndex) {
    int total;

    if (categoryIndex < 0 || categoryIndex >= numCategories)
        throw out_of_range("categoryIndex was negative or greater than #categories-1");

    if (ageGroupIndex < 0 || ageGroupIndex >= numAgeGroups)
        throw out_of_range("ageGroupIndex was negative or greater than #ageGroups-1");

    total = 0;

    total += popCounts[categoryIndex][ageGroupIndex];

    return total;
}

