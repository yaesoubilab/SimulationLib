#include "Likelihood-inl.h"
#include <Normal.h>
#include <Poisson.h>
#include <Uniform.h>
#include <UniformDiscrete.h>
#include <Binomial.h>

namespace SimulationLib
{
    using namespace StatisticalDistributions;

    // We currently support Normal and Poisson distributions on any function
    // of type OutT(InTs...) where
    //   OutT = [int | double],
    //   InTs = [int | double | size_t]

    template class LikelihoodFunction<Normal,  int    (int)>;
    template class LikelihoodFunction<Normal,  int    (double)>;
    template class LikelihoodFunction<Normal,  int    (size_t)>;

    template class LikelihoodFunction<Normal,  double (int)>;
    template class LikelihoodFunction<Normal,  double (double)>;
    template class LikelihoodFunction<Normal,  double (size_t)>;

    template class LikelihoodFunction<UniformDiscrete,  int    (int)>;
    template class LikelihoodFunction<UniformDiscrete,  int    (double)>;
    template class LikelihoodFunction<UniformDiscrete,  int    (size_t)>;

    template class LikelihoodFunction<UniformDiscrete,  double (int)>;
    template class LikelihoodFunction<UniformDiscrete,  double (double)>;
    template class LikelihoodFunction<UniformDiscrete,  double (size_t)>;

    template class LikelihoodFunction<Binomial,  int    (int)>;
    template class LikelihoodFunction<Binomial,  int    (double)>;
    template class LikelihoodFunction<Binomial,  int    (size_t)>;

    template class LikelihoodFunction<Binomial,  double (int)>;
    template class LikelihoodFunction<Binomial,  double (double)>;
    template class LikelihoodFunction<Binomial,  double (size_t)>;

    template class LikelihoodFunction<Uniform,  int    (int)>;
    template class LikelihoodFunction<Uniform,  int    (double)>;
    template class LikelihoodFunction<Uniform,  int    (size_t)>;

    template class LikelihoodFunction<Uniform,  double (int)>;
    template class LikelihoodFunction<Uniform,  double (double)>;
    template class LikelihoodFunction<Uniform,  double (size_t)>;

    template class LikelihoodFunction<Poisson, int    (int)>;
    template class LikelihoodFunction<Poisson, int    (double)>;
    template class LikelihoodFunction<Poisson, int    (size_t)>;

    template class LikelihoodFunction<Poisson, double (int)>;
    template class LikelihoodFunction<Poisson, double (double)>;
    template class LikelihoodFunction<Poisson, double (size_t)>;

    // Special template specializations for testing, make sure to REMOVE!
    template class LikelihoodFunction<Normal,  double (int, double)>;

    // Add more supported types here...
}
