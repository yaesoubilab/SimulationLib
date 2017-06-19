#include "DiscreteTimeStatistic.h"

using namespace std;

namespace SimulationLib
{
    DiscreteTimeStatistic::DiscreteTimeStatistic(string name)
        { DiscreteTimeStatistic(name, 0); }

    DiscreteTimeStatistic::DiscreteTimeStatistic(string _name, int numOfObservationsToStore)
    {
        name = _name;
        numObservations = numOfObservationsToStore;

        // If recording, allocate memory for vector and zero everything
        if (numObservations)
            observations = vector<double>(numObservations, 0);

        // Min/max begin as the boundaries of 'double'
        min = numeric_limits<double>::min();
        max = numeric_limits<double>::max();

        // Initialize more statistics
        total    = 0;
        count    = 0;
        mean     = 0;
        variance = 0;

        varianceNominator = 0;
    }

    // TODO: implement storing observations
    void DiscreteTimeStatistic::Record(double value)
    {
        double inc = value - mean;

        min = (value < min) ? value : min;
        max = (value > max) ? value : max;

        total += value;
        count += 1;

        mean += value - mean;
        varianceNominator += (count - 1) * inc * (inc / count);

        if (count) variance = varianceNominator / (count - 1);
    }

    double DiscreteTimeStatistic::GetSum() {return total;}
    double DiscreteTimeStatistic::GetCount() {return count;}
    double DiscreteTimeStatistic::GetMean() {return mean;}
    double DiscreteTimeStatistic::GetVariance() {return variance;}
    double DiscreteTimeStatistic::GetMin() {return min;}
    double DiscreteTimeStatistic::GetMax() {return max;}
    vector<double> DiscreteTimeStatistic::GetObservations() {return observations;}
}
