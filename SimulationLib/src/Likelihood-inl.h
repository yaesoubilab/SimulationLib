#include "../include/SimulationLib/Likelihood.h"

using namespace std;
using namespace SimulationLib;

template <class D, typename TT, typename VT>
typename LikelihoodFunction<D,TT,VT>::LikelihoodFunctionT
LikelihoodFunction<D,TT,VT>::GetLikelihoodFunction(void) {

    // Explicitly capture 'this'. Note that if 'this' becomes invalid due
    //   due object destruction, this lambda will have undefined and unsafe
    //   behavior!
    return [this] (TT t, VT v) {
        return Likelihood(t, v);
    };
}

template <class D, typename TT, typename VT>
typename LikelihoodFunction<D,TT,VT>::ProbabilityT
LikelihoodFunction<D,TT,VT>::Likelihood(TT t, VT v) {

    // 'y' is the value of the target function at time 't'
    VT y = f(t);

    // Ask for a distribution on the value of f(t) the given 't' and 'y'
    dist = dg(t, y);

    // Return the probability of function 'f' being valued 'v' at time 't'
    //   by calling the new probability density function with value 'v'
    return dist.pdf(v);
}

// Implements the () operator for the class by forwarding to the public
//   member function 'Likelihood'
template <class D, typename TT, typename VT>
typename LikelihoodFunction<D,TT,VT>::ProbabilityT
LikelihoodFunction<D,TT,VT>::operator()(TT t, VT v) { return Likelihood(t, v); }
