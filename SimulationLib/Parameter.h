#pragma once

#include <memory>
#include "StatisticalDistribution.h"
#include <variant>
#include <random>

namespace SimulationLib {
  using namespace StatisticalDistributions;
  using namespace std;
  class Parameter {
  public: 
    const variant<shared_ptr<const StatisticalDistribution<long> >,
		  shared_ptr<const StatisticalDistribution<long double> >,
		  double> dist;
    const double min, max;
    long double operator()(mt19937_64 &);
    inline Parameter(double d) : dist(d), min(d), max(d) {}
    inline Parameter(shared_ptr<StatisticalDistribution<long> > dist,
		     double min, double max)
      : dist(dist), min(min), max(max) {}
    inline Parameter(shared_ptr<StatisticalDistribution<long double> > dist,
		     double min, double max)
      : dist(dist), min(min), max(max) {}
  };
}
