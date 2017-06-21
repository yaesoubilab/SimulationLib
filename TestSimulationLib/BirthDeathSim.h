#pragma once
#include <ctime>
#include <string>
#include "../../StatisticalDistributionsLib/StatisticalDistributionsLib/Binomial.h"
#include "../SimulationLib/PrevalenceTimeSeries.h"
#include "../SimulationLib/IncidenceTimeSeries.h"
#include "../SimulationLib/ContinuousTimeStatistic.h"
#include "../SimulationLib/DiscreteTimeStatistic.h"
#include "../SimulationLib/CSVExport.h"

using namespace std;
using namespace StatisticalDistributions;

namespace SimulationLib {
    class BirthDeathSim
    {
    public:
        BirthDeathSim(int timeMax, int periodLength, long nPeople,
                      double pDeath, double pBirth);
        ~BirthDeathSim(void);
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
        void reportStats(int t, long nPeople, int nBirths, int nDeaths);

        IncidenceTimeSeries<int>  *births;
        IncidenceTimeSeries<int>  *deaths;
        DiscreteTimeStatistic   *birthStatistics;
        DiscreteTimeStatistic   *deathStatistics;

        PrevalenceTimeSeries<int> *population;
        ContinuousTimeStatistic *populationStatistics;
    };
}
