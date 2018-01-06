#include "../include/SimulationLib/JSONImport.h"
#include "../include/SimulationLib/Param.h"
#include "GeneralStatDist.h"
#include <StatisticalDistribution.h>
#include <json.hpp>
#include <string>
#include "Bernoulli.h"
#include "Beta.h"
#include "BetaBinomial.h"
#include "Binomial.h"
#include "ChiSquared.h"
#include "Constant.h"
#include "DiracDelta.h"
// #include "Dirichlet.h"
// #include "DirichletMultinomial.h"
#include "Empirical.h"
// #include "Exponential.h"
#include "Gamma.h"
#include "GammaPoisson.h"
#include "Geometric.h"
#include "JohnsonSb.h"
#include "JohnsonSl.h"
#include "JohnsonSu.h"
#include "KroneckerDelta.h"
#include "Lognormal.h"
#include "Multinomial.h"
#include "NegBinomial.h"
#include "Normal.h"
#include "Poisson.h"
#include "RNG.h"
#include "Triangular.h"
#include "Uniform.h"
#include "UniformDiscrete.h"
#include "Weibull.h"

namespace SimulationLib{
	using namespace StatisticalDistributions;
	using json = nlohmann::json;
	Param parameterize(json jobj);
	Param parameterize(string fname);
	GeneralStatDist mapDistribution(json obj);
	// StatisticalDistribution mapDistribution(json jobj);
}