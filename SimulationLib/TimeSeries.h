#pragma once

using namespace std;

namespace SimulationLib
{
    template <typename T>
    class TimeSeries
    {
    public:
                // string name;
        virtual double GetTime0(void) = 0;
        virtual bool IsWritable(void) = 0;
        virtual vector<T> *GetVector(void) = 0;
    };
}
