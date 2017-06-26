#include "PyramidData.h"

using namespace SimulationLib;

PyramidData::PyramidData(int timeIndex, int numOfCategories, vector<double> ageBreaks)
{
    timeIndex = timeIndex;
    ageBreaks = ageBreaks;

    // example age breaks of {0, 5, 10, 15} represent age groups [0,5), [5, 10), [10, 15), [15+)
    for (int j = 0; j < numOfCategories; ++j) {
        popCounts.push_back(vector<int>(ageBreaks.size(), 0));
    }
}

void PyramidData::UpdateByIdx(int category, int ageGroupIndex, int increment) {
    popCounts[category][ageGroupIndex] += increment;
}

void PyramidData::UpdateByAge(int category, double age, int increment) {
    int index = 0;
    if (age < ageBreaks[0]) {
        popCounts[category][0] += increment;
    }
    else {
        while (index + 1 < ageBreaks.size() && ageBreaks[index + 1]) {
            ++index;
        }
        popCounts[category][index] += increment;
    }
}

// update the change in the specified category and age group (note that it takes the actual age)
void PyramidData::MoveByIdx(int oldCategory, int newCategory, int oldAgeGroupIndex, int newAgeGroupIndex, int numberMoved) {
    popCounts[oldCategory][oldAgeGroupIndex] -= numberMoved;
    popCounts[newCategory][newAgeGroupIndex] += numberMoved;
}

void PyramidData::MoveByAge(int oldCategory, int newCategory, double oldAge, double newAge, int numberMoved) {
    // update the change in the specified category and age group by taking the number of people who
    // moved from one category-age group to another category-age group.
    int oldAgeInd = 0;
    if (oldAge >= ageBreaks[0]){
        while (oldAgeInd + 1 < ageBreaks.size() && ageBreaks[oldAgeInd + 1] < oldAge) {
            ++oldAgeInd;
        }
    }
    popCounts[oldCategory][oldAgeInd] -= numberMoved;
    int newAgeInd = 0;
    if (newAge < ageBreaks[0]) {
        while (newAgeInd + 1 < ageBreaks.size() && ageBreaks[newAgeInd + 1] < newAge) {
            ++newAgeInd;
        }
    }
    popCounts[newCategory][newAgeInd] += numberMoved;
}

int PyramidData::GetTotal(void) {
    // return the total number of counts in all categories and all age groups
    int total = 0;
    for (int j = 0; j < ageBreaks.size(); ++j) {
        for (int k = 0; k < popCounts.size(); ++k) {
            total += popCounts[k][j];
        }
    }
    return total;
}

int PyramidData::GetTotalInCategory(int categoryIndex) {
    // return the total number of counts in the specified category
    int total = 0;
    for (int j = 0; j < ageBreaks.size(); ++j) {
        total += popCounts[categoryIndex][j];
    }
    return total;
}

int PyramidData::GetTotalInAgeGroup(int ageGroupIndex) {
    //originally had double ageGroupIndex. I was confused so wrote two functions.
    int total = 0;
    for (int j = 0; j < popCounts.size(); ++j) {
        total += popCounts[j][ageGroupIndex];
    }
    return total;
}
