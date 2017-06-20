#pragma once
#include <ctime>
#include "StatisticalDistribution.h"
#include "PrevalenceTimeSeries.h"
#include "IncidenceTimeSeries.h"
#include "ContinuousTimeStatistic.h"
#include "DiscreteTimeStatistic.h"

using namespace std;
using namesapce StatisticalDistributions;

namespace SimulationLib {
    class BirthDeathSim
    {
    public:
        BirthDeathSim(int timeMax, int periodLength, long nPeople,
                      double pDeath, double pBirth);
        void Run();
        void Write(fname);

    private:
        int nPeriods, nPeople, pDeath, pBirth;

        StatisticalDistribution *bDistribution;
        StatisticalDistribution *dDistribution;
        RNG *rng;

        void refreshDists(long nPeople);

        IncidenceTimeSeries   *births;
        IncidenceTimeSeries   *deaths;
        DiscreteTimeStatistic *birthStatistics;
        DiscreteTimeStatistic *deathStatistics;

        PrevalenceTimeSeries *population;
    };
}
