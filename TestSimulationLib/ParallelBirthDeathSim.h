#pragma once
#include <ctime>
#include <string>
#include <future>

#include <PrevalenceTimeSeries.h>
#include <IncidenceTimeSeries.h>
#include <ContinuousTimeStatistic.h>
#include <DiscreteTimeStatistic.h>
#include <CSVExport.h>

#include <Binomial.h>

using namespace std;
using namespace StatisticalDistributions;

namespace SimulationLib {
    class ParallelBirthDeathSim
    {
    public:
        ParallelBirthDeathSim(string fileName, int nTrials, int timeMax, \
                      int periodLength, long nPeople, double pDeath,     \
                      double pBirth);
        ~ParallelBirthDeathSim(void);
        void Run(void);

    private:
        string fileName;
        int nTrajectories, timeMax, periodLength;
        long nPeople;
        double pDeath, pBirth;

        RNG *rng;

        void _runTrajectory(IncidenceTimeSeries<int>  *births, \
                            IncidenceTimeSeries<int>  *deaths, \
                            PrevalenceTimeSeries<int> *population,
                            Binomial *bDistribution,
                            Binomial *dDistribution,
                            int nPeople);

        void refreshDists(int nPeople, Binomial **bDistribution, Binomial **dDistribution);
        void reportStats(int t, long nPeople, int nBirths, int nDeaths);

        Binomial                   **bDistributionArr;
        Binomial                   **dDistributionArr;
        IncidenceTimeSeries<int>   **birthsArr;
        IncidenceTimeSeries<int>   **deathsArr;
        DiscreteTimeStatistic      **birthStatisticsArr;
        DiscreteTimeStatistic      **deathStatisticsArr;
        PrevalenceTimeSeries<int>  **populationArr;
        ContinuousTimeStatistic    **populationStatisticsArr;
    };
}
