#pragma once

#include <cmath>
#include <numeric>

// Shorthands used:
//   PrT  = type of probability
//   InTs = type of inputs (variadic) to some variadic function f
//   OutT = type of f's return value

// Given a probability function on '(TT, VT)' (most likely a Likelihood
//   function), and given a function G(t), returns a function P(t) corresponding
//   to L(t, v) where v = G(t).
template <typename PrT, typename OutT, typename... InTs>
inline
std::function<PrT(InTs...)>
CurriedProbabilityOnG(function<PrT(OutT, InTs...)> &L,
                      function<OutT(InTs...)> &G)
{
    return [&L, &G] (InTs... ins) -> PrT {
        return L(
                  G(std::forward<InTs>(ins)...),
                  std::forward<InTs>(ins)...
                );
    };
}

template<size_t... I, typename PrT, typename... InTs>
inline
PrT
ProbabilityLgSum(function<PrT(InTs...)> &P,
                 vector<std::tuple<InTs...>> &onParameters,
                 std::index_sequence<I...>)
{
    auto reducer = [&P] (PrT sum, std::tuple<InTs...> ins) {
        PrT p = P(std::get<I>(ins)...);

        return sum + std::log(p);
    };

    // init = 0
    return std::accumulate(onParameters.begin(),
                           onParameters.end(),
                           (PrT)0,
                           reducer);
}

// Given a function P(t) and a set of time-values 'Ts', calculates
//   the logarithmic product of P(t) evaluated at every 't' in 'Ts'
template<typename PrT, typename... InTs>
inline
PrT
ProbabilityLgSum(function<PrT(InTs...)> &P,
                 vector<std::tuple<InTs...>> &onParameters)
{
    return ProbabilityLgSum (
        std::forward<decltype(P)>(P),
        std::forward<decltype(onParameters)>(onParameters),
        std::index_sequence_for<InTs...>{}
    );
}
