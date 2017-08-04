#include "../include/SimulationLib/Likelihood.h"

using namespace std;
using namespace SimulationLib;

// 'Distribution', 'ValueT', and 'TypeT' are written shorthand here as
//   'D', 'VT', 'TT'
template <class D, typename VT, typename TT>
typename LikelihoodFunction<D,VT(TT)>::LikelihoodFunctionT
LikelihoodFunction<D,VT(TT)>::GetLikelihoodFunction(void) {

    // Explicitly capture 'this'. Note that if 'this' becomes invalid due
    //   due object destruction, this lambda will have undefined and unsafe
    //   behavior!
    return [this] (TT t, VT v) {
        return Likelihood(t, v);
    };
}

template <class D, typename VT, typename TT>
typename LikelihoodFunction<D,VT(TT)>::ProbabilityT
LikelihoodFunction<D,VT(TT)>::Likelihood(TT t, VT v) {
    // 'y' is the value of 'f' at time 't'
    VT y = f(t);

    // 'd' is the distribution on f(t)
    D d = dg(t, y);

    // 'l' is the probability of [f(t) = v]
    ProbabilityT l = d.pdf(v);
    return l;
}

// Implements the () operator for the class by forwarding to the public
//   member function 'Likelihood'
template <class D, typename VT, typename TT>
typename LikelihoodFunction<D,VT(TT)>::ProbabilityT
LikelihoodFunction<D,VT(TT)>::operator()(TT t, VT v) { return Likelihood(t, v); }
