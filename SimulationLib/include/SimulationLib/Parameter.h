#pragma once

#include <memory>
#include <StatisticalDistribution.h>
#include <variant>
#include <random>

// Holds a parameter to the model.
// Marcus: This is where a verbose explanation of what the variant type is,
//  and how its constructors function is sorely needed. This is a crucial component
//  of the entire Config system, and thus even if it's a relatively simple header,
// 
//  you need to be very clear and detailed in exactly how it works.
//

// variant: C++17 or more. Basically a typesafe union.
// Marcus: Also, in another file I spoke about type aliasing. 'dist''s type
//   is a prime candidate for type aliasing. Please implement this and document
//   the alias thoroughly.

namespace SimulationLib {
  using namespace StatisticalDistributions;
  using namespace std;
  // The type of the 'distribution'. Might just be a long double.
  using dist_type
    = variant<shared_ptr<const StatisticalDistribution<long> >,
              shared_ptr<const StatisticalDistribution<long double> >,
              long double>;
  class Parameter {
  public:
    const dist_type dist;
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
