#pragma once

#include <algorithm>
#include <exception>

#include "Likelihood.h"
#include "Likelihood-adaptors.h"
#include "Likelihood-sums.h"

namespace SimulationLib {

// CalculateLikelihood takes model data (from one trajectory, or many) and
//   calculates the probability that the model data would match the
//   observational data ('obs'), provided that any element of model data is
//   valued on a distribution described by the lambda function 'dg'.
//
// 1. With one set of model data, one set of observational data, and a set of
//   parameters on which to query the model and observational data,
//   CalculateLikelihood will evaluate the likelihood, for each member of
//   'params', that model and observational data would match. The lg-sum of
//   these likelihoods is then returned.
//
// 2. With one or more sets of model data, one set of observational data, and a set
//   of parameters on which to query the model and observational data,
//   CalculateLikelihood will query all model data on a query, and then use the
//   lambda function 'modelReducer' to collapse the results of each query into
//   a single value (ex. an average or median). In this way, we build a function
//   'f' which acts as a composite of the model trajectories and interfaces
//   with the LikelihoodFunction class to produce a Likelihood function. This
//   function is, as in the case where there is only one set of model data,
//   evaluated on each member of 'params', and the lg-sum of the result of these
//   calculations is computed and returned.
//
// 3. (A second option for employing CalculateLikelihood with multiple sets of
//  model data would be to calculate a lg-sum likelihood on each pair of
//  [model data #i, observational data] for each 'i' in the set of model data,
//  and write your own routine to produce a composite value from your computed
//  set of lg-sum likelihoods).
//
// Options (1.) and (2.) correspond to the two overloads of CalculateLikelihood,
//   respectively. To implement (3.) would require using the first overload
//   of CalculateLikelihood within a map or similar prodedure, and implementing
//   a routine to reduce the results of the map.

template

<// Container: a TimeSeries, PyramidTimeSeries, etc.
 // QuerySig:  the signature of a query to the Container
 // QueryType: the tuple containing the types of a query to the Container
 // Queries:   a vector of these tuples
 class Container,
 typename QuerySig              = typename Container::query_signature,
 typename QueryType             = typename Container::query_type,
 typename Queries               = std::vector<QueryType>,

 //  // Distribution: the type of distribution on which the model's output is valued

 // ProbabilityT: the type of probability returned by that distribution's pdf.
 
 // class Distribution,
 
 typename ProbabilityT          = long double,

 // LFnGen: the type of Likelihood function generator corresponding to the
 //   signature of a query on 'model' and 'obs', and corresponding to the
 //   distribution used to value model output.
 // DistributionGenerator: a std::function type which takes model parameters
 //   and output and returns a distribution on that output.

 // class LFnGen                   = LikelihoodFunction<Distribution, QuerySig>,

 typename DistributionGenerator/* = typename LFnGen::DistributionGenerator*/>

inline
long double
CalculateLikelihood(const Container&             model,  // model data
                    const Container&             obs,    // observational data
                    const Queries&               params, // params on which we query
                    const DistributionGenerator& dg)     // function to create
                                                         //   distributions
{
    // Construct Likelihood function generator on the model data
    auto lgen = LikelihoodOn(std::forward<decltype(model)>(model), dg);

    // Get Likelihood function L, which calculates the probability that
    //   some model output would occur under some set of parameters, which
    //   correspond to types contained in QueryType
    auto L = lgen.GetLikelihoodFunction();

    // Create a lambda function 'g' which will have signature
    //   g: QueryType... => Container::value_type
    auto g = QueryLambdaForContainer(obs);

    // Curry L using 'g': P will now have signature
    //   P: QueryType => ProbabilityT

    // This corresponds to the probability that under a particular parameter
    //   set, the model data (when valued on a distribution) would match the
    //   observational data
    auto P = CurriedProbabilityOnG(L, g);

    // Evaluate P on each set of parameters contained in 'params' and return
    //   its lg-sum.
    long double sum = ProbabilityLgSum(P, params);

    return sum;
}


// This is the overload of CalculateLikelihood which supports computing
//   likelihood on multiple trajectories of model data.
template

<// Container:    a TimeSeries, PyramidTimeSeries, etc.
 // Containers:   a vector of 'Container'. For instance, a vector of TimeSeries
 // ValueT:       the type of value returned by a query to the Container. For
 //                 instance, a TimeData<int> would return an 'int'.
 // QuerySig:       the signature of a query to the Container
 // QueryType: the tuple containing the types of a query to the Container
 // Queries:   a vector of these tuples
 // ModelReducer: a lambda that takes a vector of ValueT (a vector of values,
 //                 each a piece of data returned from a query to a Container),
 //                 and returns a ValueT representing some composition of these
 //                 values (an average, median, etc...)
 class       Container,
 typename    Containers   = std::vector<Container &>,
 typename    ValueT       = typename Container::value_type,
 typename    QuerySig     = typename Container::query_signature,
 typename    QueryType    = typename Container::query_type,
 typename    Queries      = std::vector<QueryType>,
 typename    ModelReducer = function<ValueT(std::vector<ValueT>)>,

 // Distribution: the type of distribution on which the model's output is valued
 // ProbabilityT: the type of probability returned by that distribution's pdf.
 class       Distribution,
 typename    ProbabilityT          = long double,

 // LFnGen: the type of Likelihood function generator corresponding to the
 //   signature of a query on a model in 'models', and 'obs', and corresponding
 //   to the distribution used to value model output.
 // DistributionGenerator: a std::function type which takes model query
 //   parameters and output and returns a distribution on that output.
 class       LFnGen                = LikelihoodFunction<Distribution, QuerySig>,
 typename    DistributionGenerator = typename LFnGen::DistributionGenerator>

inline
ProbabilityT
CalculateLikelihood(const Containers&            models,       // models
                    const ModelReducer&          modelReducer, // model reducer
                    const Container&             obs,         // obs. data
                    const Queries&               params,       // queries to test
                                                               //   likelihood on
                    const DistributionGenerator& dg)           // function to create
                                                               //   distributions
{
    // Holds lambdas which allow queries on each model data in 'models'
    std::vector<std::function<QuerySig>> modelLambdas {};

    // Generate lambdas for each model data in 'models' and add them to the
    //   'modelLambdas' vector
    std::transform(models.begin(),
                   models.end(),
                   modelLambdas.begin(),
                   QueryLambdaForContainer);

    // The function 'f' to be used by the Likelihood function. 'f' is a composite
    //   of lambdas for each model data in 'models', and the composition of these
    //   lambdas into 'f' is dictated by the 'modelReducer'.
    std::function<QuerySig> f = [&modelReducer] (auto&&... ps) {

        // Holds results of queries to each model data in 'models'
        std::vector<ValueT> modelQueryResults{};

        // Query each model data in 'models' and store the result in
        //   'modelQueryResults'
        std::transform(modelLambdas.begin(),
                       modelLambdas.end(),
                       modelQueryResults.begin(),
                       [&ps...] (auto g) {
                         // Forward query parameters to the lambda for a
                         //   particular trajectory of model data
                         g(std::forward<decltype(ps)>(ps)...);
                       });

        // Use 'modelReducer' to combine the results of the query to each
        //   trajectory's model data into a single value of type 'ProbabilityT',
        //   representing that the model (as opposed to, say, one trajectory
        //   of the model) would produce the same values as 'data' under the
        //   query parameter set 'params' and assuming that values returned by
        //   queries to model data are valued on a distribution described by
        //   'dg'
        return modelReducer(modelQueryResults);
    };

    // Construct Likelihood function generator
    LFnGen lgen{f, std::forward<decltype(dg)>(dg)};

    // Get Likelihood function L, which calculates the probability that
    //   some model output would occur under some set of parameters, which
    //   correspond to types contained in QueryType
    auto L = lgen.GetLikelihoodFunction();

    // Create a lambda function 'g' which will have signature
    //   g: QueryType... => Container::value_type
    auto g = QueryLambdaForContainer(std::forward<decltype(obs)>(obs));

    // Curry L using 'g': P will now have signature
    //   P: QueryType => ProbabilityT

    // This corresponds to the probability that under a particular parameter
    //   set, the model data (when valued on a distribution) would match the
    //   observational data
    auto P = CurriedProbabilityOnG(L, g);

    // Evaluate P on each set of parameters contained in 'params' and return
    //   its lg-sum.
    return ProbabilityLgSum(P, params);
}

template
<typename... Params,
 typename    ValueT,
 typename    ParamsTpl  = std::tuple<Params...>,
 typename    Function   = std::function<ValueT(Params...)>,
 typename    Calibrator = std::function<ParamsTpl(Function, ParamsTpl)>
>
inline
ParamsTpl // optimal parameters
Calibrate(ParamsTpl initParams, // input parameters
          Function f,           // function under calibration
          Calibrator c)         // calibrator
{
    return c(std::forward<decltype(f)>(f),
             std::forward<decltype(initParams)>(initParams));
}


}
