#include "catch.hpp"

// Change when needed.
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

using namespace std;
using namespace SimulationLib;

TEST_CASE("Gibberish", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("fheai,feac,saf,quux");
  CHECK_FALSE(p);

TEST_CASE("BernoulliA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Bernoulli,,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Bernoulli> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  REQUIRE(r->cdist().success_fraction() == .5);
}

TEST_CASE("BernoulliB", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Bernoulli,.3,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Bernoulli> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  REQUIRE(r->cdist().success_fraction() == .3);
}

TEST_CASE("Double", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("1.5124,,,,,L,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  CHECK(p->min == 1.5124L);
  CHECK(p->max == 1.5124L);
  REQUIRE(holds_alternative<long double>(p->dist));
  CHECK(get<long double>(p->dist) == 1.5124L);
}

TEST_CASE("BetaA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Beta,.3,3,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Beta> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->dist().alpha() == .3L);
  CHECK(r->dist().beta() == 3);
  CHECK(r->scale() == 1);
  CHECK(r->shift() == 0);
}

TEST_CASE("BetaB", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Bernoulli,6,.7,4,,T,,1,2");
  REQUIRE(p);
  CHECK(p->calibrate);
  CHECK(p->min == 1);
  CHECK(p->max == 2);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Beta> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->dist().alpha() == 6);
  CHECK(r->dist().beta() == .7L);
  CHECK(r->scale() == 4);
  CHECK(r->shift() == 0);
}

TEST_CASE("BetaC", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Beta,44,32,.6,3,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Beta> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->dist().alpha() == 44);
  CHECK(r->dist().beta() == 32);
  CHECK(r->scale() == .6L);
  CHECK(r->shift() == 3);
}

TEST_CASE("BetaBinomial", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Beta-binomial,10.5,4.3,17,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const BetaBinomial> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->alpha() == 10.5);
  CHECK(r->beta() == 4.3);
  CHECK(r->n() == 17);
}

TEST_CASE("BinomialA", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Binomial,,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Binomial> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().success_fraction() == .5);
  CHECK(r->cdist().trials() == 1);
}

TEST_CASE("BinomialA", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Binomial,45,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Binomial> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().success_fraction() == .5);
  CHECK(r->cdist().trials() == 45);
}

TEST_CASE("BinomialC", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Binomial,24,.6,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Binomial> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().success_fraction() == .6L);
  CHECK(r->cdist().trials() == 45);
}

TEST_CASE("ChiA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Chi-squared,,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const ChiSquared> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().shape() == .5);
}

TEST_CASE("ChiB", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Chi-squared,47,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const ChiSquared> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().shape() == 23.5);
}

TEST_CASE("Kronecker", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Constant,33,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const KroneckerDelta> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->v() == 33);
}

TEST_CASE("Dirac", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Constant,33.3124,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const DiracDelta> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->v() == Approx(33.3124));
}

TEST_CASE("ExponentialA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Exponential,3.6,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Exponential> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().lambda() == 3.6L);
  CHECK(r->shift() == 0);
}

TEST_CASE("ExponentialB", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Exponential,1.3,4.2,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Exponential> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().lambda() == 1.3L);
  CHECK(r->shift() == 4.2L);
}

TEST_CASE("GammaA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Gamma,,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Gamma> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().shape() == 1);
  CHECK(r->cdist().scale() == 1);
  CHECK(r->shift() == 0);
}

TEST_CASE("GammaB", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Gamma,4.3,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Gamma> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().shape() == 4.3L);
  CHECK(r->cdist().scale() == 1);
  CHECK(r->shift() == 0);
}

TEST_CASE("GammaC", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Gamma,432.6,3.5,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Gamma> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().shape() == 432.6L);
  CHECK(r->cdist().scale() == 3.5);
  CHECK(r->shift() == 0);
}

TEST_CASE("GammaD", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Gamma,1.3,5.6,3.2,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Gamma> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().shape() == 1.3L);
  CHECK(r->cdist().scale() == 5.6L);
  CHECK(r->shift() == 3.2L);
}

TEST_CASE("Gamma-PoissonA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Gamma-Poisson,,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const GammaPoisson> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().successes() == 1);
  CHECK(r->cdist().success_fraction() == Approx(.5));
}

TEST_CASE("Gamma-PoissonB", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Gamma-Poisson,12,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const GammaPoisson> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().successes() == 12);
  CHECK(r->cdist().success_fraction() == Approx(.5));
}

TEST_CASE("Gamma-PoissonC", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Gamma-Poisson,43,2,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const GammaPoisson> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().successes() == 43);
  CHECK(r->cdist().success_fraction() == Approx(2./3.));
}

TEST_CASE("GeometricA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Geometric,,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Geometric> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().success_fraction() == .5);
}

TEST_CASE("GeometricB", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Geometric,.3,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Geometric> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().success_fraction() == .3L);
}

TEST_CASE("JohnsonSb", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Johnson Sb,1.3,5.6,3.2,4.2,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const JohnsonSb> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->mu() == 1.3L);
  CHECK(r->sigma() == 5.6L);
  CHECK(r->gamma() == 3.2L);
  CHECK(r->delta() == 4.2L);
}

TEST_CASE("JohnsonSl", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Johnson Sl,1.3,5.6,3.2,4.2,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const JohnsonSl> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->mu() == 1.3L);
  CHECK(r->sigma() == 5.6L);
  CHECK(r->gamma() == 3.2L);
  CHECK(r->delta() == 4.2L);
}

TEST_CASE("JohnsonSu", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Johnson Su,1.3,5.6,3.2,4.2,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const JohnsonSu> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->mu() == 1.3L);
  CHECK(r->sigma() == 5.6L);
  CHECK(r->gamma() == 3.2L);
  CHECK(r->delta() == 4.2L);
}

TEST_CASE("LognormalA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Lognormal,,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Lognormal> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().location() == 0);
  CHECK(r->cdist().scale() == 1);
  CHECK(r->shift() == 0);
}

TEST_CASE("LognormalB", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Lognormal,4.3,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Lognormal> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().location() == 4.3L);
  CHECK(r->cdist().scale() == 1);
  CHECK(r->shift() == 0);
}

TEST_CASE("LognormalC", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Lognormal,2.4,1.5,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Lognormal> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().location() == 2.4L);
  CHECK(r->cdist().scale() == 1.5);
  CHECK(r->shift() == 0);
}

TEST_CASE("LognormalD", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Lognormal,5.4,2.3,1.6,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Lognormal> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().location() == 5.4L);
  CHECK(r->cdist().scale() == 2.3L);
  CHECK(r->shift() == 1.6L);
}

TEST_CASE("NegativeBinomialA", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Negative binomial,46,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const NegBinomial> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().success_fraction() == Approx(.5));
  CHECK(r->cdist().successes() == 46);
}

TEST_CASE("NegativeBinomialB", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Negative binomial,42,.36,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const NegBinomial> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().success_fraction() == Approx(.36));
  CHECK(r->cdist().successes() == 42);
}

TEST_CASE("NormalA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Normal,,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Normal> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().mean() == 0);
  CHECK(r->cdist().standard_deviation() == 1);
}

TEST_CASE("NormalB", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Normal,4.2,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Normal> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().mean() == 4.2L);
  CHECK(r->cdist().standard_deviation() == 1);
}


TEST_CASE("NormalC", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Normal,1.5,2.6,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Normal> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().mean() == 1.5);
  CHECK(r->cdist().standard_deviation() == 2.6L);
}

TEST_CASE("PoissonA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Poisson,,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Poisson> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().mean() == 1);
}

TEST_CASE("PoissonB", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Poisson,23,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Poisson> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().mean() == 23);
}

TEST_CASE("Triangular", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Triangular,1.3,2.4,2.1,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Triangular> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().lower() == 1.3L);
  CHECK(r->cdist().upper() == 2.4L);
  CHECK(r->cdist().mode() == 2.1L);
}

TEST_CASE("UniformA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Uniform,,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Uniform> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().lower() == 0);
  CHECK(r->cdist().upper() == 1);
}

TEST_CASE("UniformB", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Uniform,1.7,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Uniform> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().lower() == 0);
  CHECK(r->cdist().upper() == 1.7L);
}

TEST_CASE("UniformC", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Uniform,-12,23.3,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Uniform> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().lower() == -12);
  CHECK(r->cdist().upper() == 23.3L);
}

TEST_CASE("UniformDiscreteA", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Uniform discrete,23,,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const UniformDiscrete> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->low() == 0);
  CHECK(r->high() == 23);
}

TEST_CASE("UniformDiscreteB", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Uniform discrete,12,27,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const UniformDiscrete> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->low() == 12);
  CHECK(r->high() == 27);
}

TEST_CASE("Weibull", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Weibull,.3,6.2,,,F,,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<decltype(q)>(p->dist));
  q = get<decltype(q)>(p->dist);
  shared_ptr<const Weibull> r;
  r = dynamic_pointer_cast<remove_reference_t<decltype(*r)> >(q);
  REQUIRE(r);
  CHECK(r->cdist().shape() == .3L);
  CHECK(r->cdist().scale() == 6.2L);
}
