#pragma once

using namespace std;

namespace SimulationLib
{
    class TimeStatistic
    {
    public:
        virtual void Record(double time, double value) = 0;
    };
}
