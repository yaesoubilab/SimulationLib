#pragma once
#include <ctime>
#include <string>
#include "../../StatisticalDistributionsLib/StatisticalDistributionsLib/Binomial.h"
#include "PrevalenceTimeSeries.h"
#include "IncidenceTimeSeries.h"
#include "ContinuousTimeStatistic.h"
#include "DiscreteTimeStatistic.h"

using namespace std;
using namespace StatisticalDistributions;

namespace SimulationLib {
    class BirthDeathSim
    {
    public:
        BirthDeathSim(int timeMax, int periodLength, long nPeople,
                      double pDeath, double pBirth);
        void Run(void);
        void Write(string fname);

    private:
        int timeMax, periodLength;
        long nPeople;
        double pDeath, pBirth;

        Binomial *bDistribution;
        Binomial *dDistribution;
        RNG *rng;

        void refreshDists(void);

        IncidenceTimeSeries<int>  *births;
        IncidenceTimeSeries<int>  *deaths;
        DiscreteTimeStatistic   *birthStatistics;
        DiscreteTimeStatistic   *deathStatistics;

        PrevalenceTimeSeries<int> *population;
        ContinuousTimeStatistic *populationStatistics;
    };
}
