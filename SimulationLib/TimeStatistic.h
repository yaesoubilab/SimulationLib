#pragma once

using namespace std;

namespace SimulationLib
{
    class TimeStatistic
    {
    public:
        // All statistics libraries must implement these two methods
        virtual void Record(double value) = 0;
        virtual void Record(double time, double value) = 0;
    };
}
