// // #include "Bernoulli.h"
// // #include "Beta.h"
// // #include "BetaBinomial.h"
// // #include "Binomial.h"
// // #include "ChiSquared.h"
// // #include "DiracDelta.h"
// // #include "Dirichlet.h"
// // #include "DirichletMultinomial.h"
// // #include "Empirical.h"
// // #include "Exponential.h"
// // #include "Gamma.h"
// // #include "GammaPoisson.h"
// // #include "Geometric.h"
// // #include "JohnsonSb.h"
// // #include "JohnsonSl.h"
// // #include "JohnsonSu.h"
// // #include "KroneckerDelta.h"
// // #include "Lognormal.h"
// // #include "Multinomial.h"
// // #include "NegBinomial.h"
// // #include "Normal.h"
// // #include "Poisson.h"
// // #include "RNG.h"
// // #include "Triangular.h"
// // #include "Uniform.h"
// // #include "UniformDiscrete.h"
// // #include "Weibull.h"
// #include <boost/shared_ptr.hpp>
// #include "RNG.h"

// class GeneralStatDist {
// 	 enum class var_type {
//     	int_type = 1, long_type, long_double_type, arr_type, template_type, vector_type
//   	};
// 	struct GSD_internal {
// 		virtual ~GSD_internal() {}
// 		virtual long double Sample(StatisticalDistributions::RNG &rng) const = 0; 
// 	};

// 	template<typename T> struct GSD_model: GSD_internal {
// 		GSD_model(const T& t): object(t) {}
// 		virtual ~GSD_model() {}
// 		virtual long double Sample(StatisticalDistributions::RNG &rng) const 
// 		{ return object.Sample(rng);}

// 	private:
// 		T object;
// 	};

// 	boost::shared_ptr<GSD_internal> object;

// public:
// 	template<typename T> GeneralStatDist (const T& gsd) :
// 		object(new GSD_model<T> ( gsd)) {}

// 	long double Sample(StatisticalDistributions::RNG &rng) const
// 	{return object->Sample(rng);}

// 	};


// /* USAGE EXAMPLE
// ----------------------------------------------------
// 	typedef std::vector<GeneralStatDist> bp;
// 	typedef bp::const_iterator bp_iter;

// 	using namespace StatisticalDistributions;
// 	bp backpack;
// 	Triangular t(2, 5, 3);
// 	Normal n(2, 5);
// 	backpack.push_back(GeneralStatDist(t));
// 	backpack.push_back(GeneralStatDist(n));

//   	long seed = 5;
//   	RNG myRNG(seed);
// 	   std::cout<< "Sampling backpack test";
// 	   for( bp_iter item = backpack.begin(); item != backpack.end(); ++item )
// 	   std::cout << item->Sample(myRNG);
// ----------------------------------------------------
// END OF USAGE EXAMPLE */

