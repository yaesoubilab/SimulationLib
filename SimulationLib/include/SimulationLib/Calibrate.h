#pragma once

#include <algorithm>
#include <exception>

#include "TimeSeries.h"
#include "Likelihood.h"
#include "Likelihood-sums.h"
#include "Likelihood-adaptors.h"

namespace SimulationLib {

// CalculateLikelihood:
//     TimeSeries model, TimeSeries data -> product of likelihoods (using
//                                            Likelihood-sums.h)
template<typename ValT,
         typename PrT,
         typename Distribution,
         typename TimeT = double>
inline
PrT
CalculateLikelihood(TimeSeries<ValT> &model,
                    LikelihoodFunction<Distribution, ValT(TimeT)>
                      ::DistributionGenerator dg,
                    TimeSeries<ValT> &data)
{
    double Time0        = std::max(model.GetTime0(),   data.GetTime0());
    double TimeMax      = std::min(model.GetTimeMax(), data.GetTimeMax());
    int    PeriodLength = data.GetPeriodLength();

    if (model.GetPeriodLength() != data.GetPeriodLength())
        throw std::domain_error("Period length of model and data mismatched");

    // ---------------------------

    using LiFn = LikelihoodFunction<Distribution, ValT(TimeT)>;

    LiFn lifn  = LikelihoodOnTimeSeries<LiFn>(model, dg);
    auto L_t_v = lifn.GetLikelihoodFunction();

    auto G     = [&data] (TimeT t) -> ValT { return data(t); };
    auto L_t   = CurriedProbabilityOnG(L, G);

    // ---------------------------
    vector<TimeT> Ts{};
    for (double t = Time0; t < TimeMax; t += PeriodLength)
        Ts.emplace_back(t);

    return ProbabilityLgSum<PrT, TimeT>(L_t, Ts);
}

// Bonus function

}
