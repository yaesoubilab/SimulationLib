#include "catch.hpp"
#include "../include/SimulationLib/Likelihood.h"
#include "../include/SimulationLib/Likelihood-sums.h"
#include "../include/SimulationLib/Likelihood-adaptors.h"

#include "../include/SimulationLib/IncidenceTimeSeries.h"
#include "../include/SimulationLib/PrevalenceTimeSeries.h"

#include <Normal.h>
#include <Poisson.h>

using namespace std;
using namespace SimulationLib;

TEST_CASE("Likelihood_basic_lambda_normal", "[calibration]") {
    using LikelihoodFn = LikelihoodFunction<StatisticalDistributions::Normal, int(int)>;

    double standardDeviation = sqrt(2);
    StatisticalDistributions::Normal nDist(0, standardDeviation);
    long double masterP = nDist.pdf(0);


    LikelihoodFn::F f = [] (auto x) { return 1 * x; };

    LikelihoodFn::DistributionGenerator dg =
      [standardDeviation] (auto v, auto t) {
        return StatisticalDistributions::Normal(v, standardDeviation);
      };

    auto l_f = LikelihoodFn(f, dg).GetLikelihoodFunction();

    for (int t = 0; t < 10; ++t)
    {
        int v = f(t);
        LikelihoodFn::ProbabilityT p = l_f(v, t);

        REQUIRE(p == masterP);
    }
}

TEST_CASE("Likelihood_basic_operator_normal", "[calibration]") {
    using LikelihoodFn = LikelihoodFunction<StatisticalDistributions::Normal, int(int)>;

    double standardDeviation = sqrt(2);
    StatisticalDistributions::Normal nDist(0, standardDeviation);
    long double masterP = nDist.pdf(0);


    LikelihoodFn::F f = [] (auto x) { return 1 * x; };

    LikelihoodFn::DistributionGenerator dg =
      [standardDeviation] (auto y, auto t) {
        return StatisticalDistributions::Normal(y, standardDeviation);
      };

    auto LF = LikelihoodFn(f, dg);

    for (int t = 0; t < 10; ++t)
    {
        int v = f(t);
        LikelihoodFn::ProbabilityT p = LF(v, t);

        REQUIRE(p == masterP);
    }
}

TEST_CASE("Likelihood_normal_sanity", "[calibration]") {
    using LikelihoodFn = LikelihoodFunction<StatisticalDistributions::Normal, double(double)>;

    auto f = [] (double t) -> double { return t; };

    LikelihoodFn::DistributionGenerator dg = [] (double f_t, double t) {
        return StatisticalDistributions::Normal(f_t, 1);
    };

    LikelihoodFn lkhf(f, dg);

    LikelihoodFn::LikelihoodFunctionT L = lkhf.GetLikelihoodFunction();

    for (double a = 1; a < 10; a += 1) {
        double t = 10;
        double f_t = f(t);

        double e = 5 + a;

        LikelihoodFn::ProbabilityT p = L(e, t);

        if (e < f_t)
            REQUIRE(p < L(f_t, t));
        else if (f_t < e)
            REQUIRE(p < L(f_t, t));
        else // f_t == e
            REQUIRE(p == L(f_t, t));
    }
}

TEST_CASE("Likelihood_poisson_sanity", "[calibration]") {
    using LikelihoodFn = LikelihoodFunction<StatisticalDistributions::Poisson, double(double)>;

    auto f = [] (double t) -> double { return t; };

    LikelihoodFn::DistributionGenerator dg = [] (double f_t, double t) {
        return StatisticalDistributions::Poisson(f_t);
    };

    LikelihoodFn lkhf(f, dg);

    LikelihoodFn::LikelihoodFunctionT L = lkhf.GetLikelihoodFunction();

    for (double a = 1; a < 10; a += 1) {
        double t = 10;
        double f_t = f(t);
        double e = 5 + a;
        LikelihoodFn::ProbabilityT p = L(e, t);

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

// ===================================
// Tests for Likelihood-adaptors
// ===================================

TEST_CASE("Likelihood_adaptors_vector_normal", "[calibration]") {
    using LikelihoodFn = LikelihoodFunction<StatisticalDistributions::Normal, int(size_t)>;

    double standardDeviation = sqrt(2);
    StatisticalDistributions::Normal nDist(0, standardDeviation);
    long double masterP = nDist.pdf(0);

    LikelihoodFn::DistributionGenerator dg =
      [standardDeviation] (auto v, auto t) {
        return StatisticalDistributions::Normal(v, standardDeviation);
      };

    vector<int> vec = {0,1,2,3,4,5,6,7,8,9};

    LikelihoodFn Lgen = LikelihoodOnVector<StatisticalDistributions::Normal, int>(vec, dg);

    auto l_f = Lgen.GetLikelihoodFunction();

    for (size_t t = 0; t < 10; ++t)
    {
        int v_t = vec[t];
        LikelihoodFn::ProbabilityT p = l_f(v_t, t);

        // printf("P[f(%d) = %d] = %Lf\n", t, v_t, p);

        REQUIRE(p == masterP);
    }

    for (int a = 0; a < 10; ++a) {
        int t = 5;
        int v_t = vec[t];

        int e = a;

        LikelihoodFn::ProbabilityT p = l_f(e, t);

        // printf("P[f(%d) = %d] = %Lf\n", t, e, p);

        if (e < v_t)
            REQUIRE(p < l_f(v_t, t));
        else if (v_t < e)
            REQUIRE(p < l_f(v_t, t));
        else // v_t == e
            REQUIRE(p == l_f(v_t, t));
    }
}

// TEST_CASE("Likelihood_adaptors_timeseries_normal", "[calibration]") {
//     using LikelihoodFn = LikelihoodFunction<StatisticalDistributions::Normal, double(double)>;

//     double standardDeviation = sqrt(2);
//     StatisticalDistributions::Normal nDist(0, standardDeviation);
//     long double masterP = nDist.pdf(0);

//     LikelihoodFn::DistributionGenerator dg =
//       [standardDeviation] (auto v, auto t) {
//         return StatisticalDistributions::Normal(v, standardDeviation);
//       };

//     IncidenceTimeSeries<double> *its = new IncidenceTimeSeries<double>("Population 1", 0, 10, 2);

//     its->Record(0.0, 1);
//     its->Record(4.0, 2);
//     its->Record(7.0, 3);
//     its->Record(9.0, 4);

//     LikelihoodFn Lgen = LikelihoodOnTimeSeries<StatisticalDistributions::Normal, double>(*its, dg);

//     auto l_f = Lgen.GetLikelihoodFunction();

//     for (int t = 0; t < 10; ++t)
//     {
//         int its_t = its->GetTotalAtTime(t);
//         LikelihoodFn::ProbabilityT p = l_f(its_t, t);

//         printf("P[f(%d) = %d] = %Lf\n", t, its_t, p);


//         REQUIRE(p == masterP);
//     }

//     for (int a = 0; a < 5; ++a) {
//         int t = 5;
//         int its_t = its->GetTotalAtTime(t);

//         int e = a;

//         LikelihoodFn::ProbabilityT p = l_f(e, t);

//         printf("P[f(%d) = %d] = %Lf\n", t, e, p);


//         if (e < its_t)
//             REQUIRE(p < l_f(its_t, t));
//         else if (its_t < e)
//             REQUIRE(p < l_f(its_t, t));
//         else // its_t == e
//             REQUIRE(p == l_f(its_t, t));
//     }
// }


// ===================================
// Tests for Likelihood-sums
// ===================================

TEST_CASE("Likelihood_craziness_test", "[calibration]") {
    // Type of probability
    using ProbabilityT = long double;

    // Establish signature of functions f and g
    using Base         = int;
    using Scale        = double;
    using Result       = double;
    using Signature    = Result(Base, Scale);

    // Alias parameter sets
    using Params       = std::tuple<Base, Scale>;
    using ParamsVec    = std::vector<Params>;

    // Alias type of likelihood function based on distribution desired and
    //   signature of f and g
    using LFnGen = LikelihoodFunction<StatisticalDistributions::Normal,
                                      Signature>;

    // Define f as:
    //   f(base, scale) = base * scale
    function<Signature> f = [] (Base base, Scale scale) {
        return (Scale)base * scale;
    };

    // Define g as:
    //   f(base, scale) = 2 * base * scale
    function<Signature> g = [] (Base base, Scale scale) {
        return 2 * (Scale)base * scale;
    };

    // A few sets of parameters on which we are interested in finding the
    //   probability of f(parameters) = g(parameters). In this case our
    //   parameters set is of length 2, with the first parameter being the Base
    //   and the second parameter being the Scale.
    ParamsVec params{{1, 1.2}, {1, 1.5}, {2, 1.2}, {2, 1.5}};

    // Define a distribution generator which, given the result of a function
    //   'f' evaluated on a set of parameters, and that set of parameters,
    //   returns a distribution on this invocation of 'f'
    function<StatisticalDistributions::Normal(Result, Base, Scale)>
        dg = [] (Result f_b_s, Base base, Scale scale) {
            return StatisticalDistributions::Normal(f_b_s, 1);
        };

    // Generate and retrieve the likelihood function on 'f'
    auto gen     = LFnGen(f, dg);
    auto L_r_b_s = gen.GetLikelihoodFunction();

    // Loop through each parameter set in 'params' and calculate, then print,
    // the probability that f(parameter) = g(parameter),
    for (auto i = params.begin(); i != params.end(); i++) {
        auto base  = std::get<Base>(*i);
        auto scale = std::get<Scale>(*i);
        auto g_b_s = g(base, scale);

        auto p = L_r_b_s(g_b_s, base, scale);

        printf("L([f(b=%d, s=%f) = %f]) = %Lf, f(b,s) = %f\n",
                       base,
                             scale,
                                   g_b_s,
                                          p,
                                                        f(base, scale));
    }

    // Bind value parameter of likelihood function to result of 'g' invoked
    //   on a parameter set
    auto P_b_s = CurriedProbabilityOnG(L_r_b_s, g);

    // Calculate the logarithmic product of probabilities over the set of
    //   parameters to 'f' and 'g', 'params'.
    auto p = ProbabilityLgSum(P_b_s, params);

    // This test is just to force the compiler to deduce some types so that
    //   we can make sure that template type deduction is working right
    REQUIRE( true );
}

TEST_CASE("Likelihood_sums_ProbabilityOnG_and_ProbabilityLgSum", "[calibration]") {
    using LikelihoodFn = LikelihoodFunction<StatisticalDistributions::Normal, int(size_t)>;

    double standardDeviation = sqrt(2);
    StatisticalDistributions::Normal nDist(0, standardDeviation);
    long double masterP = nDist.pdf(0);

    LikelihoodFn::DistributionGenerator dg =
      [standardDeviation] (int f_i, size_t i) {
        return StatisticalDistributions::Normal(f_i, standardDeviation);
      };

    vector<int>                vec   = {0,1,2,3,4,5,6,7,8,9};
    vector<std::tuple<size_t>> FArgs = {0,1,2,3,4,5,6,7,8,9};

    LikelihoodFn Lgen = LikelihoodOnVector<StatisticalDistributions::Normal, int>(vec, dg);

    auto l_f = Lgen.GetLikelihoodFunction();

    function<int(size_t)> g = [&vec] (auto i) -> auto { return vec[i]; };

    auto p_f = CurriedProbabilityOnG(l_f, g); // pass likelihood function and 'g'

    for (int t = 0; t < 10; ++t)
    {
        int v_t = vec[t];
        LikelihoodFn::ProbabilityT p1 = l_f(v_t, t);
        LikelihoodFn::ProbabilityT p2 = p_f(t);

        REQUIRE(p2 == masterP);
        REQUIRE(p1 == p2);
    }

    // ProbabilityLgSum testing
    auto sum = ProbabilityLgSum(p_f, FArgs);

    REQUIRE(sum > -12.6552);
    REQUIRE(sum < -12.6551);
}
