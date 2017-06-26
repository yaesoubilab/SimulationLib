#include "Parameter.h"
#include <iostream>

namespace SimulationLib {
  using namespace std;
  using namespace StatisticalDistributions;
  long double Parameter::operator()(mt19937_64 &g) {
    if(holds_alternative<long double>(dist))
      return(get<long double>(dist));
    if(holds_alternative<shared_ptr<const StatisticalDistribution<long> > >(dist))
      return((*get<shared_ptr<const StatisticalDistribution<long> > >(dist))(g));
    if(holds_alternative<shared_ptr<const StatisticalDistribution<long double> > >
       (dist))
      return((*get<shared_ptr<const StatisticalDistribution<long double> > >(dist))
	     (g));
    cerr << "CAN'T HAPPEN: " << __FILE__ << ", line " << __LINE__ << endl;
    exit(1);
  }
}
