#pragma once
#include <string>

using namespace std;

namespace SimulationLib
{
    template <typename T>
    class TimeSeries
    {
    public:
        virtual string GetName(void) = 0;
        virtual double GetTime0(void) = 0;
        virtual double GetTimeMax(void) = 0;
        virtual int GetPeriodLength(void) = 0;
        virtual bool IsWritable(void) = 0;
        virtual vector<T> *GetVector(void) = 0;
        virtual T GetTotalAtTime(double t) = 0;
        virtual T operator()(double t) = 0;
    };
}
