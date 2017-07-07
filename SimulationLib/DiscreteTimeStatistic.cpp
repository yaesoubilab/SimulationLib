#include "DiscreteTimeStatistic.h"

using namespace std;

namespace SimulationLib
{
    DiscreteTimeStatistic::~DiscreteTimeStatistic() {}

    void DiscreteTimeStatistic::Record(double value)
    {
        return _record(value, count - 1);
    }

    void DiscreteTimeStatistic::Record(double _, double value)
    {
        return _record(value, count - 1);
    }

    // TODO: implement storing observations
    void DiscreteTimeStatistic::_record(double value, long locationIndex)
    {
        double inc = value - mean;

        min = (value < min) ? value : min;
        max = (value > max) ? value : max;

        total += value;
        count += 1;

        mean += inc / (double)count;
        varianceNominator += (count - 1) * inc * (inc / count);

        if (count > 1) variance = varianceNominator / (count - 1);

        // Attempt to record the value if observation-recording is enabled
        if (numObservations > 0 && locationIndex < numObservations)
            observations[locationIndex] = value;
        else if (numObservations > 0 && locationIndex >= numObservations)
            printf("Error: Attempted to record too many observations\n");
    }

    double DiscreteTimeStatistic::GetSum() {return total;}
    double DiscreteTimeStatistic::GetCount() {return count;}
    double DiscreteTimeStatistic::GetMean() {return mean;}
    double DiscreteTimeStatistic::GetVariance() {return variance;}
    double DiscreteTimeStatistic::GetMin() {return min;}
    double DiscreteTimeStatistic::GetMax() {return max;}
    vector<double> DiscreteTimeStatistic::GetObservations() {return observations;}
}
