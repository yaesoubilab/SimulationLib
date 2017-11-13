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
CurriedProbabilityOnG(std::function<PrT(OutT, InTs...)> &L,
                      std::function<OutT(InTs...)> &G)
{
    // Capture L and G by reference
    return [&L, &G] (InTs... ins) -> PrT {
        return L(
                  G(std::forward<InTs>(ins)...),
                  std::forward<InTs>(ins)...
                );
    };
}

template <typename PTpl, typename F, size_t... I>
double
ProbabilityLgSum_impl(F f, std::vector<PTpl> pv,
                      std::index_sequence<I...>)
{
    double sum {0.};

    for (size_t i = 0; i < pv.size(); i++)
        sum += std::log( f(std::get<I>(pv[i])...) );

    return sum;
}


template <typename PTpl, typename F>
double
ProbabilityLgSum(F f, std::vector<PTpl> pv)
{
    auto i = std::tuple_size<PTpl>{};

    return ProbabilityLgSum_impl(std::forward<F>(f),
                                 std::forward<decltype(pv)>(pv),
                                 std::make_index_sequence<i>{});
}
