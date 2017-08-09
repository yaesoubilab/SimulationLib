#include "Calibrate-inl.h"

template<typename ValsT, typename PrT, typename Distribution, typename TimeT = double>
PrT CalculateLikelihood(TimeSeries<ValsT> &model,
                        LikelihoodFunction<Distribution, ValsT(TimeT)>
                          ::DistributionGenerator dg,
                        TimeSeries<ValsT> &data);
