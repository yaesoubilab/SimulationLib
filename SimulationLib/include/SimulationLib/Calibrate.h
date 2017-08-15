#pragma once

#include <algorithm>
#include <exception>

namespace SimulationLib {

// CalculateLikelihood:
//     Container &model, Container &data, vector<QueryType>, DistributionGenerator
//       -> product of likelihoods (using Likelihood-sums.h)
//
// template
// <class Container,
//  class Distribution,
//  typename PrT       = long double,
//  typename QuerySig  = typename Container::query_signature,
//  typename QueryType = typename Container::query_type,
//  typename Params    = std::vector<QueryType>,
//  class LFn          = class LikelihoodFunction<Distribution, QuerySig>,
//  typename DG        = LFn::DistributionGenerator>

// inline
// PrT
// CalculateLikelihood(Container &model,
//                     Container &data,
//                     Params     params,
//                     DG         dg)
// {

// }


// template<typename ValT,
//          typename Distribution,
//          typename PrT   = long double,
//          typename TimeT = double>
// inline
// PrT
// CalculateLikelihood(TimeSeries<ValT> &model,
//                     typename LikelihoodFunction<Distribution, ValT(TimeT)>
//                       ::DistributionGenerator dg,
//                     TimeSeries<ValT> &data)
// {
//     using Signature = ValT(TimeT);
//     using Params    = std::tuple<TimeT>;
//     using ParamsVec = std::vector<Params>;
//     using LiFn      = LikelihoodFunction<Distribution, Signature>;

//     TimeT Time0        = std::max(model.GetTime0(),   data.GetTime0());
//     TimeT TimeMax      = std::min(model.GetTimeMax(), data.GetTimeMax());
//     int   PeriodLength = data.GetPeriodLength();

//     if (model.GetPeriodLength() != data.GetPeriodLength())
//         throw std::domain_error("Period length of model and data mismatched");

//     // ---------------------------
//     auto gen   = LikelihoodOn(model, dg);
//     auto L_v_t = gen.GetLikelihoodFunction();

//     auto G     = [&data] (TimeT t) -> ValT { return data(t); };
//     auto L_t   = CurriedProbabilityOnG(L_v_t, G);

//     // ---------------------------
//     ParamsVec Ts{};
//     for (double t = Time0; t < TimeMax; t += PeriodLength)
//         Ts.emplace_back(t);

//     return ProbabilityLgSum<PrT, TimeT>(L_t, Ts);
// }

// Bonus function

}
