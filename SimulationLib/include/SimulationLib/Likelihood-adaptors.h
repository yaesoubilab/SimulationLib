#pragma once

#include <vector>
#include <tuple>
#include <functional>

#include "Likelihood.h"

using namespace SimulationLib;

// Helper method for QueryLambdaForContainer. Takes advantage of an
//   std::index_sequence by treating its template parameters as a parameter
//   pack, and unpacking them to specify the parameters of the lambda
//   function being generated.
template <size_t... I,
          class Container,
          typename QueryResult       = typename Container::value_type,
          typename QueryType         = typename Container::query_type,
          typename QuerySignature    = typename Container::query_signature>
function<QuerySignature>
QueryLambdaForContainer(Container &c, std::index_sequence<I...>)
{
    return [&c] (typename std::tuple_element<I, QueryType>::type... params)
                   -> QueryResult {
        return c(params...);
    };
}

// Given a Container with member types ::query_type, ::query_signature, and
//   ::value_type, generates a lambda function of signature ::query_signature
//   which calls the Container's operator() method and returns the result.
//
//   Types:
//
//     Container: any class with member types ::query_type, ::query_signature,
//       and ::value_type which implements an operator() method of signature
//       ::query_signature.
//
//     QueryType: alias to Container::query_type
//     QuerySignature: alias to Container::query_signature
//     QueryCardinality: the number of arguments to the Container's operator()
//       method
//     QueryIndexSequence: An index {0,1,...} of size=QueryCardinality, used
//       by the helper method for unpacking the ::query_type tuple
//
template <class Container,
          typename QueryType          = typename Container::query_type,
          typename QuerySignature     = typename Container::query_signature,
          size_t   QueryCardinality   = std::tuple_size<QueryType>::value,
          class    QueryIndexSequence = 
            std::make_index_sequence<QueryCardinality>>
function<QuerySignature>
QueryLambdaForContainer(Container &c)
{
    return QueryLambdaForContainer(std::forward<decltype(c)>(c),
                                   QueryIndexSequence{});
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
//
//   Dist: any statistical distribution
//
//   Container: any class with member types ::query_signature, ::value_type,
//     ::query_type, and an operator() whose signature (absent the object
//     pointer) matches 'query_signature'. See PyramidTimeSeries.h for an example
//     of a valid specification of these member types.
//
//   QuerySignature: aliased to Container::query_signature
//
//   LikelihoodFn: aliased to the type of LikelihoodFunction that would be generated
//     for the given distribution and query signature
template <typename Dist,
          class Container,
          typename QuerySignature        = typename Container::query_signature,
          class    LikelihoodFn          = LikelihoodFunction<Dist, QuerySignature>,
          typename... Args>
LikelihoodFn
LikelihoodOn(const Container &c,
             const std::function<Dist(Args...)> &dg)
{
    // Retrieve a lambda function on the container
    function<QuerySignature> f =
      QueryLambdaForContainer(std::forward<decltype(c)>(c));

    // Construct the LikelihoodFunction
    return {f, dg};
}

// Returns a LikelihoodFunction class on vector 'v' using DistributionGenerator
//   'dg'
template <class Distribution, typename T>
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
