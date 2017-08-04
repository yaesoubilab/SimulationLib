#pragma once

#include <vector>

#include "Likelihood.h"
#include "TimeSeries.h"

using namespace SimulationLib;

// Returns a LikelihoodFunction class on vector 'v' using DistributionGenerator
//   'dg'
template <typename Distribution, typename T>
LikelihoodFunction<Distribution, T(int)>
LikelihoodOnVector(vector<T>& v,
                   typename LikelihoodFunction<Distribution, T(int)>::DistributionGenerator dg)
{
    auto f = [&v] (int t) -> T { return v.at(t); };

    // Signature of function 'f' is T(int) because vectors are index by
    //   integers
    return LikelihoodFunction<Distribution, T(int)>(f, dg);
}

// Returns a LikelihoodFunction class on TimeSeries 'ts' using
//   DistributionGenerator 'dg'
template <typename Distribution, typename T>
LikelihoodFunction<Distribution, T(double)>
LikelihoodOnTimeSeries(TimeSeries<T>& ts,
                       typename LikelihoodFunction<Distribution, T(double)>::DistributionGenerator dg)
{
    auto f = [&ts] (double t) -> T { return ts(t); };

    // Signature of function 'f' is T(double), because the 't' parameter for
    //   TimeSeries classes is a double
    return LikelihoodFunction<Distribution, T(double)>(f, dg);
}
