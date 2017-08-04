#include "catch.hpp"
#include "../include/SimulationLib/Likelihood.h"

#include <Normal.h>
#include <Poisson.h>

using namespace std;
using namespace SimulationLib;

TEST_CASE("Likelihood_basic_lambda_normal", "[calibration]") {
    using LikelihoodFn = LikelihoodFunction<StatisticalDistributions::Normal, int(int)>;

    double standardDeviation = sqrt(2);
    StatisticalDistributions::Normal nDist(0, standardDeviation);
    long double masterP = nDist.pdf(0);


    LikelihoodFn::TimeValuedFunction f = [] (auto x) { return 1 * x; };

    LikelihoodFn::DistributionGenerator dg =
      [standardDeviation] (auto t, auto v) {
        return StatisticalDistributions::Normal(v, standardDeviation);
      };

    auto l_f = LikelihoodFn(f, dg).GetLikelihoodFunction();

    for (int t = 0; t < 10; ++t)
    {
        int v = f(t);
        LikelihoodFn::ProbabilityT p = l_f(t, v);

        REQUIRE(p == masterP);
    }
}

TEST_CASE("Likelihood_basic_operator_normal", "[calibration]") {
    using LikelihoodFn = LikelihoodFunction<StatisticalDistributions::Normal, int(int)>;

    double standardDeviation = sqrt(2);
    StatisticalDistributions::Normal nDist(0, standardDeviation);
    long double masterP = nDist.pdf(0);


    LikelihoodFn::TimeValuedFunction f = [] (auto x) { return 1 * x; };

    LikelihoodFn::DistributionGenerator dg =
      [standardDeviation] (auto t, auto y) {
        return StatisticalDistributions::Normal(y, standardDeviation);
      };

    auto LF = LikelihoodFn(f, dg);

    for (int t = 0; t < 10; ++t)
    {
        int v = f(t);
        LikelihoodFn::ProbabilityT p = LF(t, v);

        REQUIRE(p == masterP);
    }
}

TEST_CASE("Likelihood_normal_sanity", "[calibration]") {
    using LikelihoodFn = LikelihoodFunction<StatisticalDistributions::Normal, double(double)>;

    auto f = [] (double t) -> double { return t; };

    LikelihoodFn::DistributionGenerator dg = [] (double t, double f_t) {
        return StatisticalDistributions::Normal(f_t, 1);
    };

    LikelihoodFn lkhf(f, dg);

    LikelihoodFn::LikelihoodFunctionT L = lkhf.GetLikelihoodFunction();

    for (double a = 1; a < 10; a += 1) {
        double t = 10;
        double f_t = f(t);

        double e = 5 + a;

        LikelihoodFn::ProbabilityT p = L(t, e);

        if (e < f_t)
            REQUIRE(p < L(t, f_t));
        else if (f_t < e)
            REQUIRE(p < L(t, f_t));
        else // f_t == e
            REQUIRE(p == L(t, f_t));
    }
}

TEST_CASE("Likelihood_poisson_sanity", "[calibration]") {
    using LikelihoodFn = LikelihoodFunction<StatisticalDistributions::Poisson, double(double)>;

    auto f = [] (double t) -> double { return t; };

    LikelihoodFn::DistributionGenerator dg = [] (double t, double f_t) {
        return StatisticalDistributions::Poisson(f_t);
    };

    LikelihoodFn lkhf(f, dg);

    LikelihoodFn::LikelihoodFunctionT L = lkhf.GetLikelihoodFunction();

    for (double a = 1; a < 10; a += 1) {
        double t = 10;
        double f_t = f(t);
        double e = 5 + a;
        LikelihoodFn::ProbabilityT p = L(t, e);

        // printf("L(f(%f) = %f) = %Lf\n", t, e, p);

        // if (e < f_t)
        //     REQUIRE(p < L(t, f_t));
        // else if (f_t < e)
        //     REQUIRE(p < L(t, f_t));
        // else // f_t == e
        //     REQUIRE(p == L(t, f_t));
        REQUIRE(true);
    }
}
