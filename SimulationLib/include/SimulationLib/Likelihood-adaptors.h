#pragma once

#include <vector>
#include <tuple>
#include <functional>

#include "Likelihood.h"
#include "TimeSeries.h"
#include "PyramidTimeSeries.h"
#include "PyramidData.h"

using namespace SimulationLib;

// Generates a generic lambda to make a query on some Container class
//   which implements a () operator of parameters 'QueryParameters' and returns
//   a value of type Container::value_type.
template <typename Container,
          typename QueryResult = typename Container::value_type,
          typename... QueryParameters>
function<QueryResult(QueryParameters...)>
QueryLambdaForContainer(Container &c)
{
    return [&c] (QueryParameters&&... params) {
        return c(std::forward<QueryParameters>(params)...);
    };
}

// Takes some Container and some DistributionGenerator on values stored
//   in that Container and returns a LikelihoodFunction on that Container
//   using the passed DistributionGenerator.
//
// Explicitly supported Container types: TimeSeries, PyramidData, PyramidTimeSeries
//
// Explicitly unsupported Container types: std::vector
//
// However, any class conforming to the specifications of Container (described
//   below) may be used
//
// Types:
//   Dist: any statistical distribution
//   Container: any class with member types ::query_signature and ::value_type
//     and an operator() whose signature (absent object pointer) matches
//     'query_signature'.
//   QuerySignature: aliased to Container::query_signature
template <typename Dist,
          typename Container,
          typename QuerySignature = typename Container::query_signature>
LikelihoodFunction<Dist, QuerySignature>
LikelihoodOn(Container &c,
             const typename LikelihoodFunction<Dist, QuerySignature>
                              ::DistributionGenerator dg)
{
    function<QuerySignature> f =
      QueryLambdaForContainer(std::forward<decltype(c)>(c));

    return {f, std::forward<decltype(dg)>(dg)};
}

// Returns a LikelihoodFunction class on vector 'v' using DistributionGenerator
//   'dg'
template <typename Distribution, typename T>
LikelihoodFunction<Distribution, T(size_t)>
LikelihoodOnVector(std::vector<T>& v,
                   const typename LikelihoodFunction<Distribution, T(size_t)>::DistributionGenerator dg)
{
    auto f = [&v] (size_t i) -> T { return v.at(i); };

    // Signature of function 'f' is T(size_t) because vectors are index by
    //   size_t
    return LikelihoodFunction<Distribution, T(size_t)>
      ( f,
        std::forward<decltype(dg)>(dg) );
}
