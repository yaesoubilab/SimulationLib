#include "DiscreteTimeStatistic.h"

#pragma once
#include <string>
#include <vector>
#include <limits>

using namespace std;

namespace SimulationLib
{
    DiscreteTimeStatistic::DiscreteTimeStatistic(string name)
        { DiscreteTimeStatistic(name, 0); }

    DiscreteTimeStatistic::DiscreteTimeStatistic(string _name, int numOfObservationsToStore)
    {
        name = _name;
        numObservations = numOfObservationsToStore;

        if (numObservations)
            observations = vector<double*>(numObservations, NULL);

        min = numeric_limits<double>::min();
        max = numeric_limits<double>::max();
        total    = 0;
        count    = 0;
        mean     = 0;
        variance = 0;

        varianceNominator = 0;
    }

    void DiscreteTimeStatistic::Record(double value)
    {
        double inc = value - mean;

        min    = (value < min) ? value : min;
        max    = (value > max) ? value : max;

        total += value;
        count += 1;

        mean += obs - _mean;
        varianceNominator += (count - 1) * inc * (inc / count);

        if (count) variance = varianceNominator / (count - 1);
    }

    double DiscreteTimeStatistic::GetSum()
    {

    }
    double DiscreteTimeStatistic::GetCount()
    {

    }
    double DiscreteTimeStatistic::GetMean()
    {

    }
    double DiscreteTimeStatistic::GetVariance()
    {

    }
    double DiscreteTimeStatistic::GetMin()
    {

    }
    double DiscreteTimeStatistic::GetMax()
    {

    }
    vector<double> DiscreteTimeStatistic::GetObservations()
    {

    }
}
