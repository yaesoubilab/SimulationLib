#pragma once

#include <functional>
#include <tuple>

using namespace std;

namespace SimulationLib
{
    // Distribution: a distribution from the StatisticalDistributionsLib
    //
    // f: a function which takes arguments InTs and returns OutT
    // InTs: type of a function's input *(variadic)*
    // OutT: type of a function's output
    //
    // Here, LikelihoodFunction is forward-declared with 'T' to allow
    //   for template specialization syntax resembling a function signature,
    //   i.e. OutT(InTs)
    template <typename Distribution, typename T> class LikelihoodFunction;
    template <typename Distribution, typename OutT, typename... InTs>
    class LikelihoodFunction<Distribution, OutT(InTs...)>
    {
    public:
        // Type of probability returned, right now probability is represented
        //   as a long double
        using ProbabilityT = long double;

        // Type of the Likelihood function, returning the probability that
        //   function 'f' is valued at 'OutT' at time 'InTs'.
        using LikelihoodFunctionT = function<ProbabilityT(OutT, InTs...)>;

        // A unary function which takes a InTs and returns a OutT
        using F = function<OutT(InTs...)>;

        // Type of a function which takes parameters to F and a value 'f(params)' and returns
        //   a distribution on f(params).
        using DistributionGenerator =
          function<Distribution(OutT, InTs...)>;

        // The LikelihoodFunction class is a function-like class which can
        //   give the probability of a [time-valued unary function whose output
        //   is parameterized on a specified distribution] being valued at
        //   a specified value at a specified time.
        //
        //   f: A function f(t) which takes a time parameter InTs and returns
        //        a value of type OutT
        //
        //   dg: A function which, given an actual value of function 'f' at
        //          a time 't', returns a Distribution on f(t)
        LikelihoodFunction(F f,
                           DistributionGenerator dg) : f(f), dg(dg) {};

        // Returns a lambda function which can easily be passed around unencumbered
        //   by the requirement of object binding imposed on class members such
        //   as 'Likelihood' or 'operator()'.
        // WARNING: the returned lambda will break if its originating object
        //   (the instance of LikelihoodFunction) is destroyed! Be careful
        //   about local variables going out of scope...
        LikelihoodFunctionT GetLikelihoodFunction(void);

        // Given a time 't' and a value 'v', returns the probability that
        //   the function 'f' will be valued 'v' at time 't'.
        ProbabilityT Likelihood(OutT v, InTs... inputs);

        // Same as ::Likelihood, but as an operator overload
        ProbabilityT operator()(OutT v, InTs... inputs);

    private:
        // The time-valued function (from the constructor)
        F f;

        // The function which generates parameters for distribution 'dist'
        //   (also from the constructor)
        DistributionGenerator dg;
    };


}
