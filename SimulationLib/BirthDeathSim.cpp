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
        deathStatistics = new DiscreteTimeStatistic("Deaths")
        populationStatistics = new ContinuousTimeStatistic("Population");

        births = new IncidenceTimeSeries("Births", 1, timeMax, periodLength, 1, birthStatistics);
        deaths = new IncidenceTimeSeries("Deaths", 1, timeMax, periodLength, 1, deathStatistics);
        population = new PrevalenceTimeSeries("Population", timeMax, periodLength, 1, populationStatistics);
    }

    void BirthDeathSim::Run(void) {
        int nBirths, nDeaths, delta;
        for (int t = 0; t < timeMax; ++t)
        {
            nBirths  = bDistribution->Sample(rng);
            nDeaths  = bDistribution->Sample(rng);
            delta    = nBirths - nDeaths;
            nPeople += delta;

            births->Record(t, nBirths);
            deaths->Record(t, nDeaths);
            population->Record(t, delta);

            refreshDists();
        }
    }

    void BirthDeathSim::refreshDists(long nPeople) {
        bDistribution = new(bDistribution) Binomial(nPeople, pBirth);
        dDistribution = new(dDistribution) Binomial(nPeople, pBirth);
    }
}
