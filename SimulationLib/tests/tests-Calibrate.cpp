#include <iostream>

#include "catch.hpp"
#include "../include/SimulationLib/Calibrate.h"

#include "../include/SimulationLib/IncidenceTimeSeries.h"
#include "../include/SimulationLib/PrevalenceTimeSeries.h"

#include <Normal.h>
#include <Poisson.h>
#include <UniformDiscrete.h>

using namespace SimulationLib;
using namespace StatisticalDistributions;

// The following test cases are commented as work on the calibration
// suite continues.

// TEST_CASE("Hello_world_calibrate", "[CalculateLikelihood]") {
// 	auto model = PrevalenceTimeSeries<int>("test", 2, 1);
// 	auto obs   = PrevalenceTimeSeries<int>("test", 2, 1);

// 	model.Record(0., 1);
// 	// model.Record(1, 2);
// 	// model.Record(2, 3);

// 	obs.Record(0., 1);
// 	// obs.Record(1, 2);
// 	// obs.Record(2, 3);

//     using DG = std::function<UniformDiscrete(double,double)>;
// 	DG dg = [] (double v, double t) {
// 		return UniformDiscrete(v-1, v+1);
// 	};

// 	using Params = std::vector<std::tuple<double>>;

// 	Params ps {std::make_tuple(0)}; // t=0

// 	long double result = CalculateLikelihood(model, obs, ps, dg);

// 	REQUIRE(result == Approx(std::log(1./2)));
// }

// TEST_CASE("Hello_world_calibrate 2", "[CalculateLikelihood]") {
// 	auto model = PrevalenceTimeSeries<int>("test", 2, 1);
// 	auto obs   = PrevalenceTimeSeries<int>("test", 2, 1);

// 	model.Record(0., 1);
// 	// model.Record(1, 2);
// 	model.Record(2, 3);

// 	obs.Record(0., 1);
// 	// obs.Record(1, 2);
// 	obs.Record(2, 3);

//     using DG = std::function<UniformDiscrete(double,double)>;
// 	DG dg = [] (double v, double t) {
// 		return UniformDiscrete(0, 10);
// 	};

// 	using Params = std::vector<std::tuple<double>>;

// 	Params ps {std::make_tuple(2)}; // t=0, 2

// 	long double result = CalculateLikelihood(model, obs, ps, dg);

// 	REQUIRE(result == Approx(std::log(1./10)));
// }

// TEST_CASE("Hello_world_calibrate 3", "[CalculateLikelihood]") {
// 	auto model = PrevalenceTimeSeries<int>("test", 2, 1);
// 	auto obs   = PrevalenceTimeSeries<int>("test", 2, 1);

// 	model.Record(0., 1);
// 	obs.Record(0., 2);

//     using DG = std::function<Normal(double,double)>;
// 	DG dg = [] (double v, double t) {
// 		return Normal(v, v);
// 	};

// 	using Params = std::vector<std::tuple<double>>;

// 	Params ps {std::make_tuple(0)}; // t=0

// 	long double result = CalculateLikelihood(model, obs, ps, dg);
// 	long double answer = std::log(Normal(1, 1).pdf(2));

// 	REQUIRE(result == Approx(answer));
// }

// TEST_CASE("Hello_world_calibrate_4", "[CalculateLikelihood]") {
// 	auto model = PrevalenceTimeSeries<int>("test", 364, 7);
// 	auto obs   = PrevalenceTimeSeries<int>("test", 364, 7);

// 	model.Record(0., 1);
// 	// model.Record(1., 1);
// 	// model.Record(2., 1);

// 	obs.Record(0., 1);
// 	// obs.Record(1., 2000);
// 	// obs.Record(2., 2);

//     using DG = std::function<Normal(double,double)>;
// 	DG dg = [] (double v, double t) {
// 		return Normal(v, v);
// 	};

// 	using Params = std::vector<std::tuple<double>>;

// 	Params ps {std::make_tuple(0)}; // t=0

// 	long double result = CalculateLikelihood(model, obs, ps, dg);
// 	long double actual = Normal(1, 1).pdf(1);

// 	REQUIRE(result == Approx(std::log(actual)));
// }

// TEST_CASE("Hello_world_calibrate 5", "[CalculateLikelihood]") {
// 	auto model = PrevalenceTimeSeries<int>("test", 364, 7);
// 	auto obs   = PrevalenceTimeSeries<int>("test", 364, 7);

// 	model.Record(0., 1);
// 	model.Record(7., 1);
// 	model.Record(14., 1);

// 	obs.Record(0., 2);
// 	obs.Record(7., 2);
// 	obs.Record(14., 2);

//     using DG = std::function<Normal(double,double)>;
// 	DG dg = [] (double v, double t) {
// 		return Normal(v, v);
// 	};

// 	using Params = std::vector<std::tuple<double>>;

// 	Params ps {std::make_tuple(0),
// 			   std::make_tuple(7),
// 			   std::make_tuple(14)}; // t=0,1,2

// 	long double result = CalculateLikelihood(model, obs, ps, dg);
// 	long double actual {0};
// 	actual += std::log(Normal(1,1).pdf(2));
// 	actual += std::log(Normal(2,2).pdf(4));
// 	actual += std::log(Normal(3,3).pdf(6));

// 	REQUIRE(result == Approx(actual));
// }

// TEST_CASE("Hello_world_calibrate 6", "[CalculateLikelihood]") {
// 	using Params = std::vector<std::tuple<double>>;
//     using DG = std::function<Normal(double,double)>;
	
// 	auto model = PrevalenceTimeSeries<int>("test", 364, 7);
// 	auto obs   = PrevalenceTimeSeries<int>("test", 364, 7);
// 	Params ps {}; // t=0,1,2

// 	for (int i=0; i<364; i++) {
// 		model.Record(i+1, 1);
// 		obs.Record(i+1, 2);
// 		ps.push_back(std::make_tuple(i+1));
// 	}

// 	DG dg = [] (double v, double t) {
// 		return Normal(v, v);
// 	};


// 	long double result = CalculateLikelihood(model, obs, ps, dg);
// 	long double actual {0};
// 	for (int i=0; i<364; i++) {
// 		auto modelVal = model(i+1);
// 		auto obsVal = obs(i+1);
// 		actual += std::log(Normal(modelVal,modelVal).pdf(obsVal));
// 	}

// 	REQUIRE(result == Approx(actual));
// }