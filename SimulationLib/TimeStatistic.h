#pragma once

using namespace std;

namespace SimulationLib
{
    class TimeStatistic
    {
    public:
        TimeStatistic(string ;name) {
            name = _name;
        }

        string name;

        // All statistics libraries must implement these three methods
        virtual ~TimeStatistic() {};
        virtual void Record(double value) = 0;
        virtual void Record(double time, double value) = 0;
    };
}
