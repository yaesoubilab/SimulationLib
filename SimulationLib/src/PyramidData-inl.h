#include "../include/SimulationLib/PyramidData.h"
#include <iostream>

using namespace std;
using namespace SimulationLib;

template <typename T>
PyramidData<T>::PyramidData(int _numCategories, vector<double> _ageBreaks)
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
    popCounts = std::vector< std::vector<T> >();
    for (int i = 0; i < numCategories; ++i)
        popCounts.emplace_back(std::vector<T>());

    // Zero out all population groups
    for (int i = 0; i < numCategories; ++i)
    {
        for (int j = 0; j < numAgeGroups; ++j)
        {
            popCounts[i].emplace_back(0);
        }
    }
}

// Returns the age group index corresponding to age 'age'. Throws exception
//   if 'age' is below zero.
template <typename T>
int PyramidData<T>::getAgeIdx(double age) {
    unsigned long idx;

    if (age < 0)
        throw out_of_range("Age specified must be >= 0");

    idx = 0;
    for (; idx < ageBreaks.size(); ++idx)
        if (ageBreaks[idx] > age)
            break;

    return idx;
}

template <typename T>
bool PyramidData<T>::UpdateByIdx(int category, int ageGroupIndex, T increment) {
    T currentPopulation;

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

template <typename T>
bool PyramidData<T>::UpdateByAge(int category, double age, T increment) {
    int ageGroupIndex;
    try { ageGroupIndex = getAgeIdx(age); } catch (...) { throw; }

    return UpdateByIdx(category, ageGroupIndex, increment);
}

// update the change in the specified category and age group (note that it takes the actual age)
template <typename T>
bool PyramidData<T>::MoveByIdx(int oldCategory, int oldAgeGroupIndex, int newCategory, int newAgeGroupIndex, T numberMoved) {
    T oldPopulation;

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

template <typename T>
bool PyramidData<T>::MoveByAge(int oldCategory, double oldAge, int newCategory, double newAge, T numberMoved) {
    int oldAgeIdx, newAgeIdx;

    try { oldAgeIdx = getAgeIdx(oldAge); } catch (...) { throw; }
    try { newAgeIdx = getAgeIdx(newAge); } catch (...) { throw; }

    return MoveByIdx(oldCategory, oldAgeIdx, newCategory, newAgeIdx, numberMoved);
}

template <typename T>
T PyramidData<T>::GetTotal(void) {
    T total;

    total = 0;
    for (int i = 0; i < numCategories; ++i)
        for (int j = 0; j < numAgeGroups; ++j)
            total += popCounts[i][j];

    return total;
}

template <typename T>
T PyramidData<T>::GetTotalInCategory(int categoryIndex) {
    T total;

    if (categoryIndex < 0 || categoryIndex >= numCategories)
        throw out_of_range("categoryIndex was negative or greater than #categories-1");

    total = 0;
    for (int i = 0; i < numAgeGroups; ++i)
        total += popCounts[categoryIndex][i];

    return total;
}


template <typename T>
T PyramidData<T>::GetTotalInAgeGroup(int ageGroupIndex) {
    T total;

    if (ageGroupIndex < 0 || ageGroupIndex >= numAgeGroups)
        throw out_of_range("ageGroupIndex was negative or greater than #ageGroups-1");

    total = 0;
    for (int i = 0; i < numCategories; ++i)
        total += popCounts[i][ageGroupIndex];

    return total;
}

template <typename T>
T PyramidData<T>::GetTotalInAgeGroupAndCategory(int ageGroupIndex, int categoryIndex) {
    T total;

    if (categoryIndex < 0 || categoryIndex >= numCategories)
        throw out_of_range("categoryIndex was negative or greater than #categories-1");

    if (ageGroupIndex < 0 || ageGroupIndex >= numAgeGroups)
        throw out_of_range("ageGroupIndex was negative or greater than #ageGroups-1");

    total = popCounts[categoryIndex][ageGroupIndex];

    return total;
}

template <typename T>
T PyramidData<T>::operator()(int ageGroupIndex, int categoryIndex) {
    return GetTotalInAgeGroupAndCategory(ageGroupIndex, categoryIndex);
}

template <typename T>
size_t PyramidData<T>::GetNumCategories(void) {
    return numCategories;
}

template <typename T>
vector<double> PyramidData<T>::GetAgeBreaks(void) {
    return ageBreaks;
}


template <typename T>
void PyramidData<T>::PrintPyramid(void)
{
    for (int i = 0; i < numCategories; ++i)
    {
        printf("This is category %d\n", i);
        for (int j = 0; j < numAgeGroups; ++j)
        {
            // printf("Age group %d: %d\n", j, popCounts[i][j]);
            cout << "Age group " << j << ": ";
            cout << popCounts[i][j];
            cout << endl;
        }
    }
    return;
}

