#pragma once

#include <cmath>
#include <numeric>

// Shorthands used:
//   PrT  = type of probability returned (likely long double)
//   InTs = type of inputs (variadic) to some variadic function L
//   OutT = type of L's return value

// Given a probability function L on parameters (OutT, InTs...), returns a
//   function P(InTs...) equivalent to L(G(InTs...), InTs...)
template <typename PrT = long double, typename OutT, typename... InTs>
inline
std::function<PrT(InTs...)>
CurriedProbabilityOnG(std::function<PrT(OutT, InTs...)> &L,
                      std::function<OutT(InTs...)> &G)
{
    // Capture L and G by reference
    return [&L, &G] (InTs... ins) -> PrT {
        // printf("G(%f) = %d\n", ins..., G(ins...));

        return L( G(ins...), ins... );
    };
}

template <typename PTpl, typename F, size_t... I>
long double
ProbabilityLgSum_impl(F f, std::vector<PTpl> pv,
                      std::index_sequence<I...>)
{
    long double sum {0.};
    for (size_t i = 0; i < pv.size(); i++) {
      // printf("Running P(%f)\n", (double)std::get<I>(pv[i])...);
      auto val = std::max(f(std::get<I>(pv[i])...), std::numeric_limits< long double >::min());
      sum += std::log(val);
    }

    return sum;
}


template <typename PTpl, typename F>
long double
ProbabilityLgSum(F f, std::vector<PTpl> pv)
{
    auto i = std::tuple_size<PTpl>{};

    return ProbabilityLgSum_impl(std::forward<F>(f),
                                 std::forward<decltype(pv)>(pv),
                                 std::make_index_sequence<i>{});
}
