#include "Likelihood-inl.h"
#include <Bernoulli.h>

namespace SimulationLib
{
    using namespace StatisticalDistributions;

    template class LikelihoodFunction<Bernoulli, int, int>;
    template class LikelihoodFunction<Bernoulli, int, double>;
    // Add more supported types here...
}
