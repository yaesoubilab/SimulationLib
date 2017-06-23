#include <map>
#include <cstdio>
#include <string>
#include <iostream>
#include "DataFrame.h"

#include "Beta.h"
#include "ChiSquared.h"
#include "Exponential.h"
#include "Gamma.h"
#include "JohnsonSb.h"
#include "JohnsonSl.h"
#include "JohnsonSu.h"
#include "Lognormal.h"
#include "Normal.h"
#include "Triangular.h"
#include "Uniform.h"
#include "Weibull.h"
#include "DiracDelta.h"

#include "BetaBinomial.h"
#include "Binomial.h"
#include "Empirical.h"
#include "GammaPoisson.h"
#include "NegBinomial.h"
#include "Poisson.h"
#include "UniformDiscrete.h"
#include "KroneckerDelta.h"

namespace SimulationLib {
  using namespace StatisticalDistributions;

  typedef const StatisticalDistribution<long double> *csdp;

  typedef const StatisticalDistribution<long> *csip;
  
  typedef csdp (*ddmk)(const char *);

  typedef csip (*idmk)(const char *);
  
  static const std::map<const std::string, const ddmk> mpd = {
      make_pair("Beta", [](const char *i) -> csdp {
	  long double alpha, beta, scale = 1, shift = 0;
	  sscanf(i, "%Lf,%Lf,%Lf,%Lf", &alpha, &beta, &scale, &shift);
	  return new Beta(alpha, beta, scale, shift);
	}),
	make_pair("Chi squared", [](const char *i) -> csdp {
	    long double dof;
	    sscanf(i, "%Lf", &dof);
	    return new ChiSquared(dof);
	  }),
	make_pair("Exponential", [](const char *i) -> csdp {
	    long double rate, shift = 0;
	    sscanf(i, "%Lf,%Lf", &rate, &shift);
	    return new Exponential(rate, shift);
	  }),
	make_pair("Gamma", [](const char * i) -> csdp {
	    long double alpha = 1, beta = 1, shift = 0;
	    sscanf(i, "%Lf,%Lf,%Lf", &alpha, &beta, &shift);
	    return new Gamma(alpha, beta, shift);
	  }),
	make_pair("Johnson Sb", [](const char * i) -> csdp {
	    long double mu, sigma, gamma, delta;
	    sscanf(i, "%Lf,%Lf,%Lf,%Lf", &mu, &sigma, &gamma, &delta);
	    return new JohnsonSb(mu, sigma, gamma, delta);
	  }),
	make_pair("Johnson Sl", [](const char * i) -> csdp {
	    long double mu, sigma, gamma, delta;
	    sscanf(i, "%Lf,%Lf,%Lf,%Lf", &mu, &sigma, &gamma, &delta);
	    return new JohnsonSb(mu, sigma, gamma, delta);
	  }),
	make_pair("Johnson Su", [](const char * i) -> csdp {
	    long double mu, sigma, gamma, delta;
	    sscanf(i, "%Lf,%Lf,%Lf,%Lf", &mu, &sigma, &gamma, &delta);
	    return new JohnsonSb(mu, sigma, gamma, delta);
	  }),
	make_pair("Lognormal", [](const char * i) -> csdp {
	    long double mu = 0, sigma = 1, shift = 0;
	    sscanf(i, "%Lf,%Lf,%Lf", &mu, &sigma, &shift);
	    return new Lognormal(mu, sigma, shift);
	  }),
	make_pair("Normal", [](const char * i) -> csdp {
	    long double mu, sigma;
	    sscanf(i, "%Lf,%Lf", &mu, &sigma);
	    return new Normal(mu, sigma);
	  }),
	make_pair("Triangular", [](const char * i) -> csdp {
	    long double min, mode, max;
	    sscanf(i, "%Lf,%Lf,%Lf", &min, &max, &mode);
	    return new Triangular(min, max, mode);
	  }),
	make_pair("Uniform", [](const char * i) -> csdp {
	    long double min = 0, max = 1;
	    sscanf(i, "%Lf,%Lf", &min, &max);
	    return new Uniform(min, max);
	  }),
	make_pair("Weibull", [](const char * i) -> csdp {
	    long double a = 1, b = 1;
	    sscanf(i, "%Lf,%Lf", &a, &b);
	    return new Weibull(a, b);
	  }),
	make_pair("Constant", [](const char *i) -> csdp {
	    long double v = 0;
	    sscanf(i, "%Lf", &v);
	    return new DiracDelta(v);
	  })
  };

  template<>
  DataFrame<StatisticalDistribution<long double> >::DataFrame(const char *file,
							     bool loopTime)
    : DataFrame(file, loopTime, [](const char *str) {
	int fstcomma = std::strchr(str, ',') - str;
	char *buf = (char *)malloc(strlen(str));
	strcpy(buf, str);
	buf[fstcomma] = 0;
	try {
	  return(std::shared_ptr<const StatisticalDistribution<long double> >
		 (mpd.at(buf)(buf + fstcomma + 1)));
									      
	} catch(std::out_of_range x) {
	  std::cerr << "Distribution not found: " << '"'
		    << buf << '"' << std::endl;
	  return(std::shared_ptr<
		 const StatisticalDistribution<long double> >());
	}
      }) {}

  static const std::map<const std::string, const idmk> mpi({
      make_pair("Beta-binomial", [](const char *i) -> csip {
	  long n;
	  long double alpha = 1, beta = 1;
	  sscanf(i, "%li,%Lf,%Lf", &n, &alpha, &beta);
	  return new BetaBinomial(n, alpha, beta);
	}),
	make_pair("Binomial", [](const char *i) -> csip {
	    long n = 1;
	    long double p = .5;
	    sscanf(i, "%li,%Lf", &n, &p);
	    return new BetaBinomial(n, p);
	  }),
	make_pair("Empirical", [](const char *i) -> csip {
	    int n;
	    long double l;
	    std::vector<long double> wts;
	    sscanf(i, "%i", &n);
	    i = strchr(i, ',') + 1;
	    for(int j = 0; j < n; j++) {
	      if(!sscanf(i, "%Lf", &l))
		break;
	      i = strchr(i, ',') + 1;
	      wts.push_back(l);
	    }
	    return new Empirical(wts);
	  }),
	make_pair("Gamma-Poisson", [](const char *i) -> csip {
	    long double alpha = 1, beta = 1;
	    sscanf(i, "%Lf,%Lf", &alpha, &beta);
	    return new GammaPoisson(alpha, beta);
	  }),
	make_pair("Negative binomial", [](const char *i) -> csip {
	    long n;
	    long double p = .5;
	    sscanf(i, "%li,%Lf", &n, &p);
	    return new NegBinomial(n, p);
	  }),
	make_pair("Poisson", [](const char *i) -> csip {
	    long double mu = 1;
	    sscanf(i, "%Lf", &mu);
	    return new Poisson(mu);
	  }),
	make_pair("Uniform discrete", [](const char *i) -> csip {
	    long low, high;
	    sscanf(i, "%li,%li", &low, &high);
	    return new UniformDiscrete(low, high);
	  }),
	make_pair("Constant", [](const char *i) -> csip {
	    long v;
	    sscanf(i, "%li", &v);
	    return new KroneckerDelta(v);
	  })
	});

  template<>
  DataFrame<StatisticalDistribution<long> >::DataFrame(const char *file,
							     bool loopTime)
    : DataFrame(file, loopTime, [](const char *str) {
	int fstcomma = std::strchr(str, ',') - str;
	char *buf = (char *)malloc(strlen(str));
	strcpy(buf, str);
	buf[fstcomma] = 0;
	try {
	  return(std::shared_ptr<const StatisticalDistribution<long> >
		 (mpi.at(buf)(buf + fstcomma + 1)));
									      
	} catch(std::out_of_range x) {
	  std::cerr << "Distribution not found: " << '"'
		    << buf << '"' << std::endl;
	  return(std::shared_ptr<const StatisticalDistribution<long> >());
	}
      }) {}
  
}
