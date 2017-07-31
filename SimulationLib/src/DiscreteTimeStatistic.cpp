#include "../include/SimulationLib/DiscreteTimeStatistic.h"

using namespace std;

namespace SimulationLib
{
    DiscreteTimeStatistic::~DiscreteTimeStatistic() {}

    void DiscreteTimeStatistic::Record(double value)
    {
        return _record(value);
    }

    void DiscreteTimeStatistic::Record(double _, double value)
    {
        return _record(value);
    }

    // TODO: implement storing observations
    void DiscreteTimeStatistic::_record(double value)
    {
        double inc = value - mean;

        min = (value < min) ? value : min;
        max = (value > max) ? value : max;

        total += value;
        count += 1;

        mean += inc / (double)count;
        varianceNominator += (count - 1) * inc * (inc / count);

        if (count > 1) variance = varianceNominator / (count - 1);

        // Record value
        observations.push_back(value);
    }

    double DiscreteTimeStatistic::GetSum() {return total;}
    double DiscreteTimeStatistic::GetCount() {return count;}
    double DiscreteTimeStatistic::GetMean() {return mean;}
    double DiscreteTimeStatistic::GetVariance() {return variance;}
    double DiscreteTimeStatistic::GetMin() { if (count == 0) return 0; else return min;}
    double DiscreteTimeStatistic::GetMax() { if (count == 0) return 0; else return max;}
    vector<double> DiscreteTimeStatistic::GetObservations() {return observations;}

    double DiscreteTimeStatistic::GetPercentile(double alpha) {
        int size, idx;

        if (alpha <= 0 || alpha > 1) {
            throw out_of_range("Alpha was <= 0 or > 1");
        }

        // Sort the vector
        std::sort(observations.begin(), observations.end());

        size = observations.size();

        // Used "nearest-rank method" to find percentile
        // subtract 1 to fit to 0 indexing
        idx = (int) ceil((double) size * alpha) - 1;

        return observations[idx];
    }
}
