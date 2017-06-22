#include "BirthDeathSim.h"

namespace SimulationLib {
    BirthDeathSim::BirthDeathSim(int _timeMax, int _periodLength, long _nPeople,
                                 double _pDeath, double _pBirth) {
        timeMax      = _timeMax;
        periodLength = _periodLength;
        nPeople      = _nPeople,
        pDeath       = _pDeath;
        pBirth       = _pBirth;

        // Seed random number generator with current time
        rng = new RNG(time(NULL));

        // The memory pointed to by 'bDistribution' and 'dDistribution' are
        //   recycled throughout execution as new Binomial classes are
        //   instantiated on these locations to reflect changes in the size
        //   of the population.
        bDistribution = new Binomial(nPeople, pBirth);
        dDistribution = new Binomial(nPeople, pDeath);

        birthStatistics = new DiscreteTimeStatistic("Births");
        deathStatistics = new DiscreteTimeStatistic("Deaths");
        populationStatistics = new ContinuousTimeStatistic("Population");

        // We assume there are no births or deaths during the period [-inf, 0],
        //   so time0 is set to 1 for 'births' and 'deaths'. Statistics are
        //   collected on every period.
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

    // void printVector(vector<int> *v) {
    //     for (int i = 0; i < v->size(); ++i)
    //     {
    //         printf("x: %2d y: %2d\n", i, (*v)[i]);
    //     }
    //     printf("\n");
    // }

    void BirthDeathSim::Run(void) {
        int nBirths, nDeaths, delta;

        CSVExport<int> exporter("BirthDeathSim-output.csv");

        population->Record(0, nPeople);

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

        births->Close();
        deaths->Close();
        population->Close();

        exporter.AddTimeSeries(population);
        exporter.AddTimeSeries(births);
        exporter.AddTimeSeries(deaths);
        exporter.Write();
    }

    void BirthDeathSim::reportStats(int t, long nPeople, int nBirths, int nDeaths) {
        printf("[%3d] nPeople %5ld | nBirths %2d | nDeaths %2d\n", t, nPeople, nBirths, nDeaths);
    }

    // Create new distributions in-place to reflect changes in the size of
    //   the population.
    void BirthDeathSim::refreshDists(void) {
        bDistribution = new(bDistribution) Binomial(nPeople, pBirth);
        dDistribution = new(dDistribution) Binomial(nPeople, pDeath);
    }
}
