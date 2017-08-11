#pragma once

#include <cmath>
#include <numeric>

// Shorthands used:
//   PrT  = type of probability returned (likely long double)
//   InTs = type of inputs (variadic) to some variadic function L
//   OutT = type of L's return value

// Given a probability function L on parameters (OutT, InTs...), returns a
//   function P(InTs...) equivalent to L(G(InTs...), InTs...)
template <typename PrT, typename OutT, typename... InTs>
inline
std::function<PrT(InTs...)>
CurriedProbabilityOnG(function<PrT(OutT, InTs...)> &L,
                      function<OutT(InTs...)> &G)
{
    // Capture L and G by reference
    return [&L, &G] (InTs... ins) -> PrT {
        return L(
                  G(std::forward<InTs>(ins)...),
                  std::forward<InTs>(ins)...
                );
    };
}

// This overload is intended to be called by the overload defined
//   below - third argument is an index sequence for InTs... to allow
//   template-based expansion of of elements of onParameters during call to
//   'P'
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

// Given a function P(InTs...) and a vector of parameters of type 'InTs',
//   calculates the logarithmic product of P(InTs...) evaluated at every
//   parameter set in the 'onParameters' vector.
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
