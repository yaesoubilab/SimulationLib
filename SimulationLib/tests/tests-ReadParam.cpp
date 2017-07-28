#include "catch.hpp"

// Change when needed.
#include "../src/ReadParam.h"

using namespace std;
using namespace SimulationLib;

TEST_CASE("BernoulliA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Bernoulli,,,,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const Bernoulli> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  REQUIRE(r->cdist.success_fraction() == .5);
}

TEST_CASE("BernoulliB", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Bernoulli,.3,,,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const Bernoulli> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  REQUIRE(r->cdist.success_fraction() == .3);
}

TEST_CASE("Double", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("1.5124,,,,,L,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  CHECK(p->min == 1.5124L);
  CHECK(p->max == 1.5124L);
  REQUIRE(holds_alternative<long double>(p->dist));
  CHECK(get<long double>(p->dist) == 1.5124L);
}

TEST_CASE("BetaA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Beta,.3,3,,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const Beta> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->dist.alpha() == .3L);
  CHECK(r->dist.beta() == 3);
  CHECK(r->scale == 1);
  CHECK(r->shift == 0);
}

TEST_CASE("BetaB", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Bernoulli,6,.7,4,,T,1,2");
  REQUIRE(p);
  CHECK(p->calibrate);
  CHECK(p->min == 1);
  CHECK(p->max == 2);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const Beta> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->dist.alpha() == 6);
  CHECK(r->dist.beta() == .7L);
  CHECK(r->scale == 4);
  CHECK(r->shift == 0);
}

TEST_CASE("BetaC", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Beta,44,32,.6,3,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const Beta> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->dist.alpha() == 44);
  CHECK(r->dist.beta() == 32);
  CHECK(r->scale == .6L);
  CHECK(r->shift == 3);
}

TEST_CASE("BetaBinomial", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Beta-binomial,10.5,4.3,17,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const BetaBinomial> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->alpha == 10.5);
  CHECK(r->beta == 4.3);
  CHECK(r->n == 17);
}

TEST_CASE("BinomialA", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Binomial,,,,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const Binomial> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->cdist.success_fraction() == .5);
  CHECK(r->cdist.trials() == 1);
}

TEST_CASE("BinomialA", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Binomial,45,,,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const Binomial> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->cdist.success_fraction() == .5);
  CHECK(r->cdist.trials() == 45);
}

TEST_CASE("BinomialC", "[paramFromString]") {
  shared_ptr<const Parameter> p
    = paramFromString("Binomial,24,.6,,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const Binomial> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->cdist.success_fraction() == .6L);
  CHECK(r->cdist.trials() == 45);
}

TEST_CASE("ChiA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Chi-squared,,,,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const ChiSquared> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->cdist.shape() == .5);
}

TEST_CASE("ChiB", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Chi-squared,47,,,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const ChiSquared> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->cdist.shape() == 23.5);
}

TEST_CASE("Kronecker", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Constant,33,,,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const KroneckerDelta> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->v == 33);
}

TEST_CASE("Dirac", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Constant,33.,,,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const DiracDelta> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->v == 33);
}

TEST_CASE("ExponentialA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Exponential,3.6,,,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const Exponential> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->cdist.lambda() == 3.6L);
  CHECK(r->shift == 0);
}

TEST_CASE("ExponentialB", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Exponential,1.3,4.2,,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const Exponential> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->cdist.lambda() == 1.3L);
  CHECK(r->shift == 4.2L);
}

TEST_CASE("GammaA", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Gamma,,,,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const Gamma> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->cdist.shape() == 1);
  CHECK(r->cdist.scale() == 1);
  CHECK(r->shift == 0);
}

TEST_CASE("GammaB", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Gamma,4.3,,,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const Gamma> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->cdist.shape() == 4.3L);
  CHECK(r->cdist.scale() == 1);
  CHECK(r->shift == 0);
}

TEST_CASE("GammaC", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Gamma,432.6,3.5,,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const Gamma> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->cdist.shape() == 432.6L);
  CHECK(r->cdist.scale() == 3.5);
  CHECK(r->shift == 0);
}

TEST_CASE("GammaD", "[paramFromString]") {
  shared_ptr<const Parameter> p = paramFromString("Gamma,1.3,5.6,3.2,,F,,");
  REQUIRE(p);
  CHECK_FALSE(p->calibrate);
  shared_ptr<const StatisticalDistribution<long double> > q;
  REQUIRE(holds_alternative<typeof(q)>(p->dist));
  q = get<typeof(q)>(p->dist);
  shared_ptr<const Gamma> r;
  r = dynamic_pointer_cast<typeof(*r)>(q);
  REQUIRE(r);
  CHECK(r->cdist.shape() == 1.3L);
  CHECK(r->cdist.scale() == 5.6L);
  CHECK(r->shift == 3.2L);
}
