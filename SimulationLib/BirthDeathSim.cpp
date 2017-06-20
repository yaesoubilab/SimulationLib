#include "BirthDeathSim.h"

namespace SimulationLib {
    BirthDeathSim::BirthDeathSim(int _timeMax, int _periodLength, long _nPeople,
                                 double _pDeath, double _pBirth) {
        timeMax      = _timeMax;
        periodLength = _periodLength;
        nPeople      = _nPeople,
        pDeath       = _pDeath;
        pBirth       = _pBirth;

        rng = new RNG(time(NULL));
        bDistribution = new Binomial(nPeople, pBirth);
        dDistribution = new Binomial(nPeople, pDeath);

        birthStatistics = new DiscreteTimeStatistic("Births");
        deathStatistics = new DiscreteTimeStatistic("Deaths");
        populationStatistics = new ContinuousTimeStatistic("Population");

        births = new IncidenceTimeSeries<int>("Births", 1, timeMax, periodLength, 1, birthStatistics);
        deaths = new IncidenceTimeSeries<int>("Deaths", 1, timeMax, periodLength, 1, deathStatistics);
        population = new PrevalenceTimeSeries<int>("Population", (double)timeMax, (double)periodLength, 1, populationStatistics);
    }

    BirthDeathSim::~BirthDeathSim(void) {
        delete rng;

        delete bDistribution;
        delete dDistribution;

        delete birthStatistics;
        delete deathStatistics;
        delete populationStatistics;

        delete births;
        delete deaths;
        delete population;
    }

    void BirthDeathSim::Run(void) {
        int nBirths, nDeaths, delta;
        for (int t = 1; t < timeMax; ++t)
        {
            nBirths  = (int)bDistribution->Sample(*rng);
            nDeaths  = (int)dDistribution->Sample(*rng);
            delta    = nBirths - nDeaths;
            nPeople += delta;

            births->Record(t, nBirths);
            deaths->Record(t, nDeaths);
            population->Record(t, delta);

            reportStats(t, nPeople, nBirths, nDeaths);
            refreshDists();
        }
    }

    void BirthDeathSim::reportStats(int t, long nPeople, int nBirths, int nDeaths) {
        printf("[%3d] nPeople %5ld | nBirths %2d | nDeaths %2d\n", t, nPeople, nBirths, nDeaths);
    }

    void BirthDeathSim::refreshDists(void) {
        bDistribution = new(bDistribution) Binomial(nPeople, pBirth);
        dDistribution = new(dDistribution) Binomial(nPeople, pDeath);
    }
}
