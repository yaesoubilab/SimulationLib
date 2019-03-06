#include "JSONImport.h"
#include "Param.h"
#include "DataFrame.h"
#include "GeneralStatDist.h"
#include <StatisticalDistribution.h>
#include "nlohmann_json.hpp"
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
#include "Exponential.h"
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

	// Two ways of parameterizing, one by taking in json, another by taking in a string with filename
	Param parameterize(json jobj);
	Param parameterize(string fname);
	// Main Function - fills map so that shortname corresponds with a parameter
	void mapShortNames(json j, std::map<string, Param>& m);

	// function is made public for access by other classes
	GeneralStatDist mapDistribution(json obj);

	// StatisticalDistribution mapDistribution(json jobj);
}