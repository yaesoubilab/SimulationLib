#pragma once
#include <ctime>
#include <string>
#include "../../StatisticalDistributionsLib/StatisticalDistributionsLib/Binomial.h"
#include "../../StatisticalDistributionsLib/StatisticalDistributionsLib/Bernoulli.h"
#include "../../StatisticalDistributionsLib/StatisticalDistributionsLib/RNG.h"
#include "../SimulationLib/PrevalenceTimeSeries.h"
#include "../SimulationLib/IncidenceTimeSeries.h"
#include "../SimulationLib/PrevalencePyramidTimeSeries.h"
#include "../SimulationLib/IncidencePyramidTimeSeries.h"
#include "../SimulationLib/ContinuousTimeStatistic.h"
#include "../SimulationLib/DiscreteTimeStatistic.h"
#include "../SimulationLib/CSVExport.h"

using namespace std;
using namespace StatisticalDistributions;

namespace SimulationLib {
    class PyramidBirthDeathSim
    {
    public:
        PyramidBirthDeathSim(string fileName, int nTrials, int timeMax, int periodLength, \
                             long nPeople, double pDeath, double pBirth);
        ~PyramidBirthDeathSim(void);
        void Run(void);

    private:
        string fileName;
        int nTrajectories, timeMax, periodLength;
        long nPeople;
        double pDeath, pBirth;

        RNG *rng;

        void _runTrajectory(IncidenceTimeSeries<int>  *births,          \
                            IncidencePyramidTimeSeries *birthsPyr,      \
                            IncidenceTimeSeries<int>  *deaths,          \
                            IncidencePyramidTimeSeries *deathsPyr,      \
                            PrevalenceTimeSeries<int> *population,      \
                            PrevalencePyramidTimeSeries *populationPyr, \
                            Binomial  *bDistribution,                   \
                            Bernoulli *dDistribution,                   \
                            int nPeople);

        void refreshDist(int nPeople, Binomial **bDistribution);
        void reportStats(int t, long nPeople, int nBirths, int nDeaths);

        Binomial                    **bDistributionArr;
        Bernoulli                    *dDistribution;
        IncidenceTimeSeries<int>    **birthsArr;
        IncidencePyramidTimeSeries  **birthsPyrArr;
        IncidenceTimeSeries<int>    **deathsArr;
        IncidencePyramidTimeSeries  **deathsPyrArr;
        DiscreteTimeStatistic       **birthStatisticsArr;
        DiscreteTimeStatistic       **deathStatisticsArr;
        PrevalenceTimeSeries<int>   **populationArr;
        PrevalencePyramidTimeSeries **populationPyrArr;
        ContinuousTimeStatistic     **populationStatisticsArr;
    };
}
