#include "PyramidBirthDeathSim.h"

namespace SimulationLib {
    PyramidBirthDeathSim::PyramidBirthDeathSim(string _fileName, int _nTrajectories, int _timeMax,
                                               int _periodLength, long _nPeople,
                                               double _pDeath, double _pBirth) {

        fileName      = _fileName;
        nTrajectories = _nTrajectories;
        timeMax       = _timeMax;
        periodLength  = _periodLength;
        nPeople       = _nPeople,
        pDeath        = _pDeath;
        pBirth        = _pBirth;

        birthsArr               = new IncidenceTimeSeries<int>*[nTrajectories];
        deathsArr               = new IncidenceTimeSeries<int>*[nTrajectories];
        populationArr           = new PrevalenceTimeSeries<int>*[nTrajectories];

        birthsPyrArr            = new IncidencePyramidTimeSeries<int>*[nTrajectories];
        deathsPyrArr            = new IncidencePyramidTimeSeries<int>*[nTrajectories];
        populationPyrArr        = new PrevalencePyramidTimeSeries<int>*[nTrajectories];

        birthStatisticsArr      = new DiscreteTimeStatistic*[nTrajectories];
        deathStatisticsArr      = new DiscreteTimeStatistic*[nTrajectories];
        populationStatisticsArr = new ContinuousTimeStatistic*[nTrajectories];

        bDistributionArr        = new Binomial*[nTrajectories];
        dDistribution           = new Bernoulli(pDeath);

        defaultAgeBreaks        = vector<int>{20, 40, 60, 80};

        if (_fileName.empty()) {
            printf("Error: No fileName specified\n");
            return;
        }

        if (nTrajectories == 0) {
            printf("Error: nTrajectories = 0\n");
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

        for (int i = 0; i < nTrajectories; ++i)
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

            birthsPyrArr[i] = \
              new IncidencePyramidTimeSeries(b + s, 0, timeMax, periodLength, \
                                             2, defaultAgeBreaks);
            deathsPyrArr[i] = \
              new IncidencePyramidTimeSeries(d + s, 0, timeMax, periodLength, \
                                             2, defaultAgeBreaks);
            populationPyrArr[i] = \
              new PrevalencePyramidTimeSeries(p + s, 0, timeMax, periodLength, \
                                              2, defaultAgeBreaks);

            // The memory pointed to by 'bDistribution' and 'dDistribution' are
            //   recycled throughout execution as new Binomial classes are
            //   instantiated on these locations to reflect changes in the size
            //   of the population.
            bDistributionArr[i] = new Binomial(nPeople, pBirth);
            // dDistributionArr[i] = new Binomial(nPeople, pBirth);
        }

    }

    PyramidBirthDeathSim::~PyramidBirthDeathSim(void) {
        // Free elements of each array
        for (int i = 0; i < nTrajectories; ++i)
        {
            delete bDistributionArr[i];

            delete birthStatisticsArr[i];
            delete deathStatisticsArr[i];
            delete populationStatisticsArr[i];

            delete birthsArr[i];
            delete deathsArr[i];
            delete populationArr[i];

            delete birthsPyrArr[i];
            delete deathsPyrArr[i];
            delete populationPyrArr[i];
        }

        // Free arrays themselves
        delete bDistributionArr;
        delete dDistribution;

        delete birthStatisticsArr;
        delete deathStatisticsArr;
        delete populationStatisticsArr;

        delete birthsArr;
        delete deathsArr;
        delete populationArr;

        delete birthsPyrArr;
        delete deathsPyrArr;
        delete populationPyrArr;

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

    void PyramidBirthDeathSim::Run(void) {
        CSVExport<int>     exportBirths(fileName + string("-births.csv"));
        CSVExport<int>     exportDeaths(fileName + string("-deaths.csv"));
        CSVExport<int> exportPopulation(fileName + string("-population.csv"));
        // Pending implementation:
        //   PyramidCSVExport     exportBirthsPyr(fileName + string("-pyramid-births.csv"));
        //   PyramidCSVExport     exportDeathsPyr(fileName + string("-pyramid-deaths.csv"));
        //   PyramidCSVExport exportPopulationPyr(fileName + string("-pyramid-population.csv"));
        //
        // Also pending implementation:
        //   Exporting of statistics for each trajectory
        //   Exporting of summary statistics across each trajectory

        for (int i = 0; i < nTrajectories; ++i)
        {
            printf("Trajectory %d:\n", i);
            _runTrajectory(birthsArr[i],
                           birthsPyrArr[i],
                           deathsArr[i],
                           deathsPyrArr[i],
                           populationArr[i],
                           populationPyrArr[i],
                           bDistributionArr[i],
                           dDistribution,
                           nPeople);
            printf("\n");
        }

        for (int i = 0; i < nTrajectories; ++i)
        {
            exportBirths.AddTimeSeries(birthsArr[i]);
            exportDeaths.AddTimeSeries(deathsArr[i]);
            exportPopulation.AddTimeSeries(populationArr[i]);

            // Pending implementation:
            //   exportBirthsPyr.Add(birthsPyrArr[i]);
            //   exportDeathsPyr.Add(deathsPyrArr[i]);
            //   exportPopulationPyr.Add(populationPyrArr[i]);
            // This is also likely where statistics for each trajectory would be
            // run.
        }

        // This is likely where summary statistics would be run

        exportBirths.Write();
        exportDeaths.Write();
        exportPopulation.Write();

        exportBirthsPyr.Write();
        exportDeathsPyr.Write();
        exportPopulationPyr.Write();
    }

    void PyramidBirthDeathSim::_runTrajectory(IncidenceTimeSeries<int>    *births,        \
                                              IncidencePyramidTimeSeries  *birthsPyr,     \
                                              IncidenceTimeSeries<int>    *deaths,        \
                                              IncidencePyramidTimeSeries  *deathsPyr,     \
                                              PrevalenceTimeSeries<int>   *population,    \
                                              PrevalencePyramidTimeSeries *populationPyr, \
                                              Binomial  *bDistribution,                   \
                                              Bernoulli *dDistribution,                   \
                                              int nPeople); {
        /////////////////////////////
        // LEFT OFF HERE
        /////////////////////////////
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

    void PyramidBirthDeathSim::reportStats(int t, long nPeople, int nBirths, int nDeaths) {
        printf("[%3d] nPeople %5ld | nBirths %2d | nDeaths %2d\n", t, nPeople, nBirths, nDeaths);
    }

    // Create new distributions in-place to reflect changes in the size of
    //   the population.
    void PyramidBirthDeathSim::refreshDists(int nPeople,
                                     Binomial **bDistribution,
                                     Binomial **dDistribution) {
        *bDistribution = new(*bDistribution) Binomial(nPeople, pBirth);
        *dDistribution = new(*dDistribution) Binomial(nPeople, pDeath);
    }
}
