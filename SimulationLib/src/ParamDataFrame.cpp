#include "Parameter.h"
#include <map>
#include <cstdio>
#include <string>
#include <iostream>
#include "DataFrame.h"
#include <tuple>
#include "ReadParam.h"

#include "Bernoulli.h"
#include "Beta.h"
#include "BetaBinomial.h"
#include "Binomial.h"
#include "ChiSquared.h"
#include "DiracDelta.h"
#include "Exponential.h"
#include "Gamma.h"
#include "GammaPoisson.h"
#include "Geometric.h"
#include "JohnsonSb.h"
#include "JohnsonSl.h"
#include "JohnsonSu.h"
#include "KroneckerDelta.h"
#include "Lognormal.h"
#include "NegBinomial.h"
#include "Normal.h"
#include "Poisson.h"
#include "Triangular.h"
#include "Uniform.h"
#include "UniformDiscrete.h"
#include "Weibull.h"

// Implements DataFrame construction for parameters.

// Marcus: A cursory look at this code reveals a lot of repetition of similar,
//   yet subtly different code structures. Write a comment here giving an explanation
//   of the general layout of this code, and an explanation of what the lambdas
//   which are keys to your map do.
// Additionally, it's tough to tell what the structure of this .cpp file is.
//   It seems like it's a mix of functions, containers, and classes, but besides
//   that I can't tell what's part of classes, what stands alone within the
//   SimulationLib namespace, and how some of the namespaced functions and
//   containers play into the biggest picture. Please enlighten the codebase
//   with your explanations of these aspects of the file.

namespace SimulationLib {
  using namespace StatisticalDistributions;
  using namespace std;
  typedef const Parameter *(*pf)(const char *);

  static tuple<bool, long double, long double> extract(const char *i) {
    for(int j = 0; j < 5; j++) {
      int k;
      sscanf(i, "%*[^,],%n", &k);
      i += k;
    }
    long double min, max;
    char c = F;
    sscanf(i, "%c,%Lf,%Lf", &c, &min, &max);
    return(make_tuple(c == 'T', min, max));
  }

  static const map<string, pf> m({
    make_pair("Bernoulli", [](const char *i) -> const Parameter * {
	long double p = .5;
	sscanf(i, "%Lf", &p);
	auto f = extract(i);
	return new Parameter(make_shared<Bernoulli>(p),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Beta", [](const char *i) -> const Parameter * {
	long double alpha, beta, scale = 1, shift = 0;
	sscanf(i, "%Lf,%Lf,%Lf,%Lf", &alpha, &beta, &scale, &shift);
	auto f = extract(i);
	return new Parameter(make_shared<Beta>(alpha, beta, scale, shift),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Beta-binomial", [](const char *i) -> const Parameter * {
	long double alpha, beta;
	long n;
	sscanf(i, "%lu,%Lf,%Lf", &n, &alpha, &beta);
	auto f = extract(i);
	return new Parameter(make_shared<BetaBinomial>(n, alpha, beta),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Binomial", [](const char *i) -> const Parameter * {
	long double p = .5;
	long n = 1;
	sscanf(i, "%lu,%Lf", &n, &p);
	auto f = extract(i);
	return new Parameter(make_shared<Binomial>(n, p),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Chi-squared", [](const char *i) -> const Parameter * {
	long double dof = 1;
	sscanf(i, "%Lf", &dof);
	auto f = extract(i);
	return new Parameter(make_shared<ChiSquared>(dof),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Constant", [](const char *i) -> const Parameter * {
	long double v;
	long a;
	int w;
	sscanf(i, "%Lf", &v);
	auto f = extract(i);
	if(sscanf(i, "%li,%n", &a, &w) == 2)
	  return new Parameter(make_shared<KroneckerDelta>(a),
			     get<1>(f), get<2>(f), get<0>(f));
	return new Parameter(make_shared<DiracDelta>(v),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Exponential", [](const char *i) -> const Parameter * {
	long double rate, shift = 0;
	sscanf(i, "%Lf,%Lf", &rate, &shift);
	auto f = extract(i);
	return new Parameter(make_shared<Exponential>(rate, shift),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Gamma", [](const char *i) -> const Parameter * {
	long double alpha = 1, beta = 1, shift = 0;
	sscanf(i, "%Lf,%Lf,%Lf", &alpha, &beta, &shift);
	auto f = extract(i);
	return new Parameter(make_shared<Gamma>(alpha, beta, shift),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Gamma-Poisson", [](const char *i) -> const Parameter * {
	long double alpha = 1, beta = 1;
	sscanf(i, "%Lf,%Lf", &alpha, &beta);
	auto f = extract(i);
	return new Parameter(make_shared<GammaPoisson>(alpha, beta),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Geometric", [](const char *i) -> const Parameter * {
	long double p = .5;
	sscanf(i, "%Lf", &p);
	auto f = extract(i);
	return new Parameter(make_shared<Geometric>(p),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Johnson Sb", [](const char *i) -> const Parameter * {
	long double mu, sigma, gamma, delta;
	sscanf(i, "%Lf,%Lf,%Lf,%Lf", &mu, &sigma, &gamma, &delta);
	auto f = extract(i);
	return new Parameter(make_shared<JohnsonSb>(mu, sigma, gamma, delta),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Johnson Sl", [](const char *i) -> const Parameter * {
	long double mu, sigma, gamma, delta;
	sscanf(i, "%Lf,%Lf,%Lf,%Lf", &mu, &sigma, &gamma, &delta);
	auto f = extract(i);
	return new Parameter(make_shared<JohnsonSb>(mu, sigma, gamma, delta),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Johnson Su", [](const char *i) -> const Parameter * {
	long double mu, sigma, gamma, delta;
	sscanf(i, "%Lf,%Lf,%Lf,%Lf", &mu, &sigma, &gamma, &delta);
	auto f = extract(i);
	return new Parameter(make_shared<JohnsonSb>(mu, sigma, gamma, delta),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Lognormal", [](const char *i) -> const Parameter * {
	long double mu = 0, sigma = 1, shift = 0;
	sscanf(i, "%Lf,%Lf,%Lf", &mu, &sigma, &shift);
	auto f = extract(i);
	return new Parameter(make_shared<Lognormal>(mu, sigma, shift),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Negative binomial", [](const char *i) -> const Parameter * {
	long double p = .5;
	long n;
	sscanf(i, "%lu,%Lf", &n, &p);
	auto f = extract(i);
	return new Parameter(make_shared<NegBinomial>(n, p),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Normal", [](const char *i) -> const Parameter * {
	long double mu = 0, sigma = 1;
	sscanf(i, "%Lf,%Lf", &mu, &sigma);
	auto f = extract(i);
	return new Parameter(make_shared<Normal>(mu, sigma),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Poisson", [](const char *i) -> const Parameter * {
	long double mu = 1;
	sscanf(i, "%Lf", &mu);
	auto f = extract(i);
	return new Parameter(make_shared<Poisson>(mu),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Triangular", [](const char *i) -> const Parameter * {
	long double min, mode, max;
	sscanf(i, "%Lf,%Lf,%Lf", &min, &max, &mode);
	auto f = extract(i);
	return new Parameter(make_shared<Triangular>(min, max, mode),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Uniform", [](const char *i) -> const Parameter * {
	long double min = 0, max = 1;
	sscanf(i, "%Lf,%Lf", &min, &max);
	auto f = extract(i);
	return new Parameter(make_shared<Uniform>(min, max),
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Uniform discrete", [](const char *i) -> const Parameter * {
	long a, b;
	shared_ptr<UniformDiscrete> u;
	if(sscanf(i, "%li,%li", &a, &b) == 2)
	  u = make_shared<UniformDiscrete>(a, b);
	else
	  u = make_shared<UniformDiscrete>(a);
	auto f = extract(i);
	return new Parameter(u,
			     get<1>(f), get<2>(f), get<0>(f));
      }),
    make_pair("Weibull", [](const char *i) -> const Parameter * {
	long double a, b;
	sscanf(i, "%Lf,%Lf", &a, &b);
	auto f = extract(i);
	return new Parameter(make_shared<Weibull>(a, b),
			     get<1>(f), get<2>(f), get<0>(f));
      })
      });

    template<>
  DataFrame<Parameter>::DataFrame(const char *file, bool loopTime)
    : DataFrame(file, loopTime, paramFromString) {}

  shared_ptr<const Parameter> paramFromString(const char *str) {
    long double d;
    if(sscanf(str, "%Lf", &d))
      return(make_shared<Parameter>(d));
    int fstcomma = strchr(str, ',') - str;
    char *buf = (char *)malloc(strlen(str));
    strcpy(buf, str);
    buf[fstcomma] = 0;
    try {
      return(shared_ptr<const Parameter>
	     (m.at(buf)(buf + fstcomma + 1)));
    } catch(out_of_range x) {
      cerr << "Distribution not found: " << '"'
		<< buf << '"' << endl;
      return(shared_ptr<const Parameter>());
    }
  }

}
