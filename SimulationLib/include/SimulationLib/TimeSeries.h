#pragma once
#include <string>

using namespace std;

namespace SimulationLib
{
    template <typename T>
    class TimeSeries
    {
    public:
        using value_type      = T;
        using query_type      = std::tuple<double>;
        using query_signature = T(double);

        virtual string GetName(void) = 0;
        virtual double GetTime0(void) = 0;
        virtual double GetTimeMax(void) = 0;
        virtual int GetPeriodLength(void) = 0;
        virtual bool IsWritable(void) = 0;
        virtual vector<value_type> *GetVector(void) = 0;
        virtual value_type GetTotalAtTime(double t) const = 0;
        virtual value_type operator()(double t) const = 0;
    };
}
