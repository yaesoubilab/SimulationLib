#include "Likelihood-inl.h"

namespace SimulationLib
{
    using namespace StatisticalDistributionsLib;

    template class LikelihoodFunction<Bernoulli, int(int)>;
    template class LikelihoodFunction<Bernoulli, int(double)>;
    // Add more supported types here...
}
