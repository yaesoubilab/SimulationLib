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
		  long double> dist;
    const long double min, max;
    const bool calibrate;
    long double operator()(mt19937_64 &) const;
    inline Parameter(double d)
      : dist(d), min(d), max(d), calibrate(false) {}
    inline Parameter(shared_ptr<StatisticalDistribution<long> > dist,
		     long double min, long double max, bool c)
      : dist(dist), min(min), max(max), calibrate(c) {}
    inline Parameter(shared_ptr<StatisticalDistribution<long double> > dist,
		     long double min, long double max, bool c)
      : dist(dist), min(min), max(max), calibrate(c) {}
  };
}
