#include "DataFrameEyal.h"
namespace SimulationLib {
template<>
  DataFrame<double>::DataFrame(const char *file, bool loopTime)
    : DataFrame(file, loopTime, [](const char *str) {
	double *d = (double *)malloc(sizeof(double));
	*d = atof(str);
	return(shared_ptr<const double>(d));
      }) {}
}
