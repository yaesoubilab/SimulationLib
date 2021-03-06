#include "../include/SimulationLib/Likelihood.h"
#include <iostream>

using namespace std;
using namespace SimulationLib;

// 'Distribution', 'OutT', and 'InT' are written shorthand here as
//   'D', 'OutT', 'InTs'
template <class D, typename OutT, typename... InTs>
typename LikelihoodFunction<D,OutT(InTs...)>::LikelihoodFunctionT
LikelihoodFunction<D,OutT(InTs...)>::GetLikelihoodFunction(void)
{
    // Explicitly capture 'this'. Note that if 'this' becomes invalid due
    //   due object destruction, this lambda will have undefined and unsafe
    //   behavior!
    return [this] (OutT v, InTs&&... ins) {
        return Likelihood(v, ins...);
    };
}

template <class D, typename OutT, typename... InTs>
typename LikelihoodFunction<D,OutT(InTs...)>::ProbabilityT
LikelihoodFunction<D,OutT(InTs...)>::Likelihood(OutT v, InTs... ins)
{
    // 'y' is the value of 'f' at time 't'
    OutT y = f(ins...);

    // 'd' is the distribution on f(ins...)
    D d = dg(y, ins...);

    // 'l' is the probability of [f(ins...) = v]
    // printf("value of model(%f)=%d\n", ins..., y);
    // printf("passing to pdf: %d\n", v);
    ProbabilityT l = d.pdf(v);

    // printf("pdf is %Le\n\n", l);

    return l;
}

// Implements the () operator for the class by forwarding to the public
//   member function 'Likelihood'
template <class D, typename OutT, typename... InTs>
typename LikelihoodFunction<D,OutT(InTs...)>::ProbabilityT
LikelihoodFunction<D,OutT(InTs...)>::operator()(OutT v, InTs... ins)
{
    return Likelihood(v, ins...);
}