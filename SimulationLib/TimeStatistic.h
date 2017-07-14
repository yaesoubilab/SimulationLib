#pragma once

using namespace std;

namespace SimulationLib
{
    class TimeStatistic
    {
    public:
        TimeStatistic(string _name) {
            name = _name;
        }

        string name;

        // All statistics libraries must implement these three methods
        virtual ~TimeStatistic() {};
        virtual void Record(double value) = 0;
        virtual void Record(double time, double value) = 0;

        virtual double GetSum(void)      = 0;
        virtual double GetCount(void)    = 0;
        virtual double GetMean(void)     = 0;
        virtual double GetVariance(void) = 0;
        virtual double GetMin(void)      = 0;
        virtual double GetMax(void)      = 0;
    };
}
