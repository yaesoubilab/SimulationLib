#include "../include/SimulationLib/Likelihood.h"

using namespace std;
using namespace SimulationLib;

template <class Distribution, typename TimeT, typename ValueT>
auto LikelihoodFunction::operator()(TimeT t, ValueT v) {

    // 'y' is the value of the target function at time 't'
    ValueT y = f(t);

    // Ask for parameters of the distribution for the given 't' and 'y'
    BoundInitializerArgs params = dpg(t, y);

    // (Re-)Initialize distribution using these parameters
    std::apply(dist.init, params);

    // Return the probability of function 'f' being valued 'v' at time 't'
    //   by calling the new probability density function with value 'v'
    return dist.pdf(v);
}
