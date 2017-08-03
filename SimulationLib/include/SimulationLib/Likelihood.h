#pragma once

#include <functional>
#include <tuple>

using namespace std;

namespace SimulationLib
{
    using namespace SimulationLibUtils;

    // Distribution: a distribution from the StatisticalDistributionsLib
    //
    // TimeT:  type of function's time parameter
    // ValueT: type of function's result
    template <typename Distribution, typename TimeT = double, typename ValueT = int>
    class LikelihoodFunction
    {
    public:
        // Type of probability returned, right now probability is represented
        //   as a double
        using ProbabilityT = long double;

        using LikelihoodFunctionT = function<ProbabilityT(TimeT, ValueT)>;

        // A unary function which takes a TimeT and returns a ValueT
        using TimeValuedFunction = function<ValueT(TimeT)>;

        // Type of a function which takes a time 't' and a value 'v' and returns
        //   a distribution on function 'v' at time 't' and value 'v'.
        using DistributionGenerator =
          function<Distribution(TimeT, ValueT)>;

        // The LikelihoodFunction class is a function-like class which can
        //   give the probability of a [time-valued unary function whose output
        //   is parameterized on a specified distribution] being valued at
        //   a specified value at a particular time.
        //
        //   f: A function f(t) which takes a time parameter TimeT and returns
        //        a value of type ValueT
        //
        //   dg: A function which, given an actual value of function 'f' at
        //          a time 't', returns a Distribution
        LikelihoodFunction(TimeValuedFunction f,
                           DistributionGenerator dg) : f(f), dg(dg) {};

        // Returns a lambda function which can easily be passed around unencumbered
        //   by the requirement of object binding imposed on 'Likelihood' or
        //   'operator()'.
        // WARNING: the returned lambda will break if its originating object
        //   (the instance of LikelihoodFunction) is destroyed!
        LikelihoodFunctionT GetLikelihoodFunction(void);

        // Given a time 't' and a value 'v', returns the probability that
        //   the function 'f' will be valued 'v' at time 't'.
        ProbabilityT Likelihood(TimeT t, ValueT v);

        // Same as ::Likelihood, but as an operator overload
        ProbabilityT operator()(TimeT t, ValueT v);

    private:

        // The time-valued function
        TimeValuedFunction f;

        // The function which generates parameters for distribution 'dist'
        DistributionGenerator dg;

        // The kind of distribution used over each value of function 'f'
        Distribution dist;
    };


}
