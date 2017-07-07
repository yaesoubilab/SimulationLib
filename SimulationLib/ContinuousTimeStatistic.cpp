#include "ContinuousTimeStatistic.h"

namespace SimulationLib {
    ContinuousTimeStatistic::ContinuousTimeStatistic(string name) {
        count     = 0;
        baseTime  = 0;

        mean      = 0;
        variance  = 0;

        // Guarantees that first value will be recorded as the max and the min
        min       = numeric_limits<double>::max();
        max       = numeric_limits<double>::min();

        lastValue = 0;
        lastTime  = 0;
        tot       = 0; // Intermediate variable used to simplify calculations
    }

    // (Empty destructor)
    ContinuousTimeStatistic::~ContinuousTimeStatistic() {}

    void ContinuousTimeStatistic::Record(double increment) {
        printf("Error: ContinuousTimeStatistic requires time dimension\n");
        return;
    }

    void ContinuousTimeStatistic::Record(double time, double increment) {
        count += 1;

        if (time == baseTime) {
            lastTime = time;
            lastValue = increment;
        } else if (time > baseTime) {
            tot += (lastTime - baseTime) * (time - lastTime) * pow(value - mean, 2);
            mean = (mean * (lastTime - baseTime) + (time - lastTime) * lastValue) / \
                   (time - baseTime);

            lastTime = time;
            lastValue = value;

            min = (value < min) ? value : min;
            max = (value > max) ? value : max;

            if (count > 1)
                variance = tot / (lastTime - baseTime);
        }

    }

    // Getter methods
    double ContinuousTimeStatistic::GetSum()      { return 0; } // WARNING: not properly implemented at the moment!
    double ContinuousTimeStatistic::GetMean()     { return mean;}
    double ContinuousTimeStatistic::GetCount()    { return count; }
    double ContinuousTimeStatistic::GetVariance() { return variance; }
    double ContinuousTimeStatistic::GetMin()      { return min; }
    double ContinuousTimeStatistic::GetMax()      { return max; }
}
