#include "Likelihood-inl.h"
#include <Normal.h>
#include <Poisson.h>

namespace SimulationLib
{
    using namespace StatisticalDistributions;

    // We currently support Normal and Poisson distributions on any function
    // of type ValueT(TimeT) where ValueT, TimeT = [int | double].

    template class LikelihoodFunction<Normal, int(int)>;
    template class LikelihoodFunction<Normal, double(double)>;
    template class LikelihoodFunction<Normal, int(double)>;
    template class LikelihoodFunction<Normal, double(int)>;

    template class LikelihoodFunction<Poisson, int(int)>;
    template class LikelihoodFunction<Poisson, double(double)>;
    template class LikelihoodFunction<Poisson, int(double)>;
    template class LikelihoodFunction<Poisson, double(int)>;

    // Add more supported types here...
}
