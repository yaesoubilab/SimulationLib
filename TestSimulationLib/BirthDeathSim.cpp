#include "BirthDeathSim.h"

namespace SimulationLib {
    BirthDeathSim::BirthDeathSim(string _fileName, int _nTrials, int _timeMax,
                                 int _periodLength, long _nPeople,
                                 double _pDeath, double _pBirth) {

        fileName     = _fileName;
        nTrials      = _nTrials;
        timeMax      = _timeMax;
        periodLength = _periodLength;
        nPeople      = _nPeople,
        pDeath       = _pDeath;
        pBirth       = _pBirth;

        birthsArr               = new IncidenceTimeSeries<int>*[nTrials];
        deathsArr               = new IncidenceTimeSeries<int>*[nTrials];
        populationArr           = new PrevalenceTimeSeries<int>*[nTrials];

        birthStatisticsArr      = new DiscreteTimeStatistic*[nTrials];
        deathStatisticsArr      = new DiscreteTimeStatistic*[nTrials];
        populationStatisticsArr = new ContinuousTimeStatistic*[nTrials];

        bDistributionArr        = new Binomial*[nTrials];
        dDistributionArr        = new Binomial*[nTrials];

        if (_fileName.empty()) {
            printf("Error: No fileName specified\n");
            return;
        }

        if (nTrials == 0) {
            printf("Error: nTrials = 0\n");
            return;
        }

        if (nPeople == 0) {
            printf("Error: nPeople = 0\n");
        }

        if (periodLength == 0) {
            printf("Error: periodLength = 0\n");
        }

        // Seed random number generator with current time
        rng = new RNG(time(NULL));

        for (int i = 0; i < nTrials; ++i)
        {
            string b = string("births[");
            string d = string("deaths[");
            string p = string("population[");
            string s = to_string(i) + string("]");

            birthStatisticsArr[i]      = new DiscreteTimeStatistic(b + s);
            deathStatisticsArr[i]      = new DiscreteTimeStatistic(d + s);
            populationStatisticsArr[i] = new ContinuousTimeStatistic(p + s);


            // We assume there are no births or deaths during the period [-inf, 0],
            //   so time0 is set to 1 for 'births' and 'deaths'. Statistics are
            //   collected on every period.
            birthsArr[i] = new IncidenceTimeSeries<int>(b + s, \
                                                        1, timeMax, periodLength, \
                                                        1, birthStatisticsArr[i]);
            deathsArr[i] = new IncidenceTimeSeries<int>(d + s, \
                                                        1, timeMax, periodLength, \
                                                        1, deathStatisticsArr[i]);
            populationArr[i] = \
              new PrevalenceTimeSeries<int>(p + s, \
                                            (double)timeMax, (double)periodLength, \
                                            1, populationStatisticsArr[i]);

            // The memory pointed to by 'bDistribution' and 'dDistribution' are
            //   recycled throughout execution as new Binomial classes are
            //   instantiated on these locations to reflect changes in the size
            //   of the population.
            bDistributionArr[i] = new Binomial(nPeople, pBirth);
            dDistributionArr[i] = new Binomial(nPeople, pBirth);
        }

    }

    BirthDeathSim::~BirthDeathSim(void) {
        // Free elements of each array
        for (int i = 0; i < nTrials; ++i)
        {
            delete bDistributionArr[i];
            delete dDistributionArr[i];

            delete birthStatisticsArr[i];
            delete deathStatisticsArr[i];
            delete populationStatisticsArr[i];

            delete birthsArr[i];
            delete deathsArr[i];
            delete populationArr[i];
        }

        // Free arrays themselves
        delete bDistributionArr;
        delete dDistributionArr;

        delete birthStatisticsArr;
        delete deathStatisticsArr;
        delete populationStatisticsArr;

        delete birthsArr;
        delete deathsArr;
        delete populationArr;

        // Free RNG
        delete rng;
    }

    // void printVector(vector<int> *v) {
    //     for (int i = 0; i < v->size(); ++i)
    //     {
    //         printf("x: %2d y: %2d\n", i, (*v)[i]);
    //     }
    //     printf("\n");
    // }

    void BirthDeathSim::Run(void) {
        CSVExport<int>     exportBirths(fileName + string("-births.csv"));
        CSVExport<int>     exportDeaths(fileName + string("-deaths.csv"));
        CSVExport<int> exportPopulation(fileName + string("-population.csv"));

        for (int i = 0; i < nTrajectories; ++i)
        {
            _runTrajectory(birthsArr[i],
                           deathsArr[i],
                           populationArr[i],
                           bDistributionArr[i],
                           dDistributionArr[i],
                           nPeople);
        }

        for (int i = 0; i < nTrajectories; ++i)
        {
            exportBirths.AddTimeSeries(birthsArr[i]);
            exportDeaths.AddTimeSeries(deathsArr[i]);

            exportPopulation.AddTimeSeries(populationArr[i])
        }

        exporter.Write();
    }

    void BirthDeathSim::_runTrajectory(IncidenceTimeSeries<int>  *births, \
                                       IncidenceTimeSeries<int>  *deaths, \
                                       PrevalenceTimeSeries<int> *population,
                                       Binomial *bDistribution,
                                       Binomial *dDistribution,
                                       int nPeople) {
        int nBirths, nDeaths, delta;

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
            refreshDists(nPeople, &bDistribution, &dDistribution);
        }

        births->Close();
        deaths->Close();
        population->Close();
    }

    void BirthDeathSim::reportStats(int t, long nPeople, int nBirths, int nDeaths) {
        printf("[%3d] nPeople %5ld | nBirths %2d | nDeaths %2d\n", t, nPeople, nBirths, nDeaths);
    }

    // Create new distributions in-place to reflect changes in the size of
    //   the population.
    void BirthDeathSim::refreshDists(int nPeople,
                                     Binomial **bDistribution,
                                     Binomial **dDistribution) {
        *bDistribution = new(*bDistribution) Binomial(nPeople, pBirth);
        *dDistribution = new(*dDistribution) Binomial(nPeople, pDeath);
    }
}
