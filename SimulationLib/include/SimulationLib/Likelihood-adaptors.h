#pragma once

#include <vector>

#include "Likelihood.h"
#include "TimeSeries.h"
#include "PyramidData.h"
#include "PyramidTimeSeries.h"

using namespace SimulationLib;

// Returns a LikelihoodFunction class on vector 'v' using DistributionGenerator
//   'dg'
template <typename Distribution, typename T>
LikelihoodFunction<Distribution, T(size_t)>
LikelihoodOnVector(vector<T>& v,
                   const typename LikelihoodFunction<Distribution, T(size_t)>::DistributionGenerator dg)
{
    auto f = [&v] (size_t i) -> T { return v.at(i); };

    // Signature of function 'f' is T(size_t) because vectors are index by
    //   size_t
    return LikelihoodFunction<Distribution, T(size_t)>
      ( f,
        std::forward<decltype(dg)>(dg) );
}

// Returns a LikelihoodFunction class on TimeSeries 'ts' using
//   DistributionGenerator 'dg'
template <typename Distribution, typename T>
LikelihoodFunction<Distribution, T(double)>
LikelihoodOnTimeSeries(TimeSeries<T>& ts,
                       const typename LikelihoodFunction<Distribution, T(double)>::DistributionGenerator dg)
{
    auto f = [&ts] (double t) -> T { return ts(t); };

    // Signature of function 'f' is T(double), because the 't' parameter for
    //   TimeSeries classes is a double
    return LikelihoodFunction<Distribution, T(double)>
      ( f,
        std::forward<decltype(dg)>(dg) );
}
