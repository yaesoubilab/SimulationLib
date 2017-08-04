#pragma once

#include <functional>
#include <tuple>

using namespace std;

namespace SimulationLib
{
    // Distribution: a distribution from the StatisticalDistributionsLib
    //
    // TimeT:  type of a time-valued function's time parameter
    // ValueT: type of a time-valued function's range
    //
    // Here, LikelihoodFunction is forward-declared with 'T' to allow
    //   for template specialization syntax resembling a function signature,
    //   i.e. ValueT(TimeT)
    template <typename Distribution, typename T> class LikelihoodFunction;
    template <typename Distribution, typename ValueT, typename TimeT>
    class LikelihoodFunction<Distribution, ValueT(TimeT)>
    {
    public:
        // Type of probability returned, right now probability is represented
        //   as a long double
        using ProbabilityT = long double;

        // Type of the Likelihood function, returning the probability that
        //   function 'f' is valued at 'ValueT' at time 'TimeT'.
        using LikelihoodFunctionT = function<ProbabilityT(TimeT, ValueT)>;

        // A unary function which takes a TimeT and returns a ValueT
        using TimeValuedFunction = function<ValueT(TimeT)>;

        // Type of a function which takes a time 't' and a value 'f(t)' and returns
        //   a distribution on f(t).
        using DistributionGenerator =
          function<Distribution (TimeT, ValueT)>;

        // The LikelihoodFunction class is a function-like class which can
        //   give the probability of a [time-valued unary function whose output
        //   is parameterized on a specified distribution] being valued at
        //   a specified value at a specified time.
        //
        //   f: A function f(t) which takes a time parameter TimeT and returns
        //        a value of type ValueT
        //
        //   dg: A function which, given an actual value of function 'f' at
        //          a time 't', returns a Distribution on f(t)
        LikelihoodFunction(TimeValuedFunction f,
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
        ProbabilityT Likelihood(TimeT t, ValueT v);

        // Same as ::Likelihood, but as an operator overload
        ProbabilityT operator()(TimeT t, ValueT v);

    private:
        // The time-valued function (from the constructor)
        TimeValuedFunction f;

        // The function which generates parameters for distribution 'dist'
        //   (also from the constructor)
        DistributionGenerator dg;
    };


}
