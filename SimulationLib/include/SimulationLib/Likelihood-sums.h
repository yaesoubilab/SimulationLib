#pragma once

#include <cmath>
#include <numeric>

// Shorthands used:
//   PrT = type of probability
//   TT  = type of 'time' in any time-valued functions involved
//   VT  = type of 'value' in any time-valued functions, or functions
//           which take the output of a time-valued function as a parameter

// ProbabilityAtTime: A unary function at returns some probability (type PrT)
//   at some time (type TT)
template <typename PrT, typename TT>
using ProbabilityAtTime = function<PrT(TT)>;

// Given a probability function on '(TT, VT)' (most likely a Likelihood
//   function), and given a function g(t), returns a function P(t) corresponding
//   to L(t, v) where v = g(t).
template <typename PrT, typename TT, typename VT>
ProbabilityAtTime<PrT, TT>
CurriedProbabilityOnG(function<PrT(TT, VT)> &L,
                      function<VT(TT)> &g)
{
    return [&L, &g] (TT t) {
        return L(t, g(t));
    };
}

// Given a function P(t) and a set of time-values 'Ts', calculates
//   the logarithmic product of P(t) evaluated at every 't' in 'Ts'
template<typename PrT, typename TT>
PrT
ProbabilityLgSum(ProbabilityAtTime<PrT,TT> &P, vector<TT> &Ts)
{
    auto reducer = [&P] (PrT sum, TT t) {
        PrT p_t = P(t);
        return sum + std::log(p_t);
    };

    // init = log(0)
    return std::accumulate(Ts.begin(), Ts.end(), std::log( (PrT)0 ));
}
