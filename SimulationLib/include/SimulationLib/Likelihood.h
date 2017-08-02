#pragma once

#include <function>

#include <RNG.h>
#include <StatisticalDistributionsLib>

using namespace std;

namespace SimulationLib
{
    // Distribution: a distribution from the StatisticalDistributionsLib
    //
    // TimeT:  type of function's time parameter
    // ValueT: type of function's result
    template <class Distribution, typename TimeT = double, typename ValueT = int>
    class LikelihoodFunction<Distribution, ValueT(TimeT)>
    {
    public:
        // Type of probability returned, right now probability is represented
        //   as a double
        using ProbabilityT       = double;

        // A unary function which takes a TimeT and returns a ValueT
        using TimeValuedFunction = function<ValueT(TimeT)>;

        // The type of a Distribution's Init method, with the class instance
        //   already bound
        using BoundInitializer = std::mem_fn<Distribution::Init::result_type, Distribution::Init>

        // The types of any arguments (not including class instance) to a Distribution's
        //   ::Init method
        using BoundInitializerArgs = std::function<BoundInitializer>::argument_type;

        // Type of a function which takes a time 't' and a value 'v' and returns
        //   the parameters of a distribution on function 'v' at time 't' and
        //   value 'v'.
        using DistributionParamGenerator =
          function<std::tuple<BoundInitializerArgs>(TimeT, ValueT)>;

        // The LikelihoodFunction class is a function-like class which can
        //   give the probability of a [time-valued unary function whose output
        //   is parameterized on a specified distribution] being valued at
        //   a specified value at a particular time.
        //
        //   f: A function f(t) which takes a time parameter TimeT and returns
        //        a value of type ValueT
        //
        //   dpg: A function which, given an actual value of function 'f' at
        //          a time 't', returns parameters to initialize a distribution
        //          'Distribution' around this value. In other words, a 'dpg'
        //          must return a tuple of arguments to the specified
        //          'Distribution''s ::Init method
        LikelihoodFunction(TimeValuedFunction f,
                           DistributionParamGenerator dpg) : f(f), dpg(dpg) {};

        // Given a time 't' and a value 'v', returns the probability that
        //   the function 'f' will be valued 'v' at time 't'.
        ProbabilityT operator()(TimeT t, TSValueT v);

    private:
        // The time-valued function
        TimeValuedFunction f;

        // The function which generates parameters for distribution 'dist'
        DistributionParamGenerator dpg;

        // The kind of distribution used over each value of function 'f'
        Distribution dist;
    };
}
