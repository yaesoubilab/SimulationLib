#include "Parameter.h"

namespace SimulationLib {
  using namespace std;
  using namespace StatisticalDistributions;
  long double Parameter::operator()(mt19937_64 &g) {
    if(holds_alternative<double>(dist))
      return(get<double>(dist));
    if(holds_alternative<shared_ptr<StatisticalDistribution<long> > >(dist))
      return(*(get<shared_ptr<StatisticalDistribution<long> > >(dist))(g));
    if(holds_alternative<shared_ptr<StatisticalDistribution<long double> > >
       (dist))
      return(*(get<shared_ptr<StatisticalDistribution<long double> > >(dist))
	     (g));
  }
