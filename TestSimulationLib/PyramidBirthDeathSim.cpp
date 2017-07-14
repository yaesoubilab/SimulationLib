#include "PyramidBirthDeathSim.h"

namespace SimulationLib {
    PyramidBirthDeathSim::PyramidBirthDeathSim \
      (string _fileName, int _nTrajectories, int _timeMax, int _periodLength, \
       long _nPeople, double _pDeath, double _pBirth, int _ageMin, int _ageMax) {

        fileName      = _fileName;
        nTrajectories = _nTrajectories;
        timeMax       = _timeMax;
        periodLength  = _periodLength;
        nPeople       = _nPeople,
        pDeath        = _pDeath;
        pBirth        = _pBirth;
        ageMin        = _ageMin;
        ageMax        = _ageMax;

        birthsArr               = new IncidenceTimeSeries<int>*[nTrajectories];
        deathsArr               = new IncidenceTimeSeries<int>*[nTrajectories];
        populationArr           = new PrevalenceTimeSeries<int>*[nTrajectories];

        birthsPyrArr            = new IncidencePyramidTimeSeries*[nTrajectories];
        deathsPyrArr            = new IncidencePyramidTimeSeries*[nTrajectories];
        populationPyrArr        = new PrevalencePyramidTimeSeries*[nTrajectories];

        birthStatisticsArr      = new DiscreteTimeStatistic*[nTrajectories];
        deathStatisticsArr      = new DiscreteTimeStatistic*[nTrajectories];
        populationStatisticsArr = new ContinuousTimeStatistic*[nTrajectories];

        bDistributionArr        = new Binomial*[nTrajectories];
        dDistribution           = new Bernoulli(pDeath);
        aDistribution           = new Uniform(ageMin, ageMax);
        sDistribution           = new Bernoulli(0.50); // Assuming 50% chance male/female

        defaultAgeBreaks        = vector<double>{20, 40, 60, 80};

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
                                                        0, timeMax, periodLength, \
                                                        1, birthStatisticsArr[i]);
            deathsArr[i] = new IncidenceTimeSeries<int>(d + s, \
                                                        0, timeMax, periodLength, \
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

            // The memory pointed to by 'bDistribution' is
            //   recycled throughout execution as new Binomial classes are
            //   instantiated on the same mem location to reflect changes in the size
            //   of the population.
            bDistributionArr[i] = new Binomial(nPeople, pBirth);
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
        delete aDistribution;
        delete sDistribution;

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
        TimeSeriesCSVExport<int>     exportBirths(fileName + string("-births.csv"));
        TimeSeriesCSVExport<int>     exportDeaths(fileName + string("-deaths.csv"));
        TimeSeriesCSVExport<int> exportPopulation(fileName + string("-population.csv"));

        PyramidTimeSeriesCSVExport     exportBirthsPyr(fileName + string("-pyramid-births.csv"));
        PyramidTimeSeriesCSVExport     exportDeathsPyr(fileName + string("-pyramid-deaths.csv"));
        PyramidTimeSeriesCSVExport exportPopulationPyr(fileName + string("-pyramid-population.csv"));

        map<TimeStatType, string> columnsBirths {
            {TimeStatType::Sum, "Total births"},
            {TimeStatType::Mean, "Average # births"},
            {TimeStatType::Min, "Minimum # births"},
            {TimeStatType::Max, "Maximum # births"}
        };

        map<TimeStatType, string> columnsDeaths {
            {TimeStatType::Sum, "Total deaths"},
            {TimeStatType::Mean, "Average # deaths"},
            {TimeStatType::Min, "Minimum # deaths"},
            {TimeStatType::Max, "Maximum # deaths"}
        };

        map<TimeStatType, string> columnsPopulation {
            {TimeStatType::Mean, "Average population size"},
            {TimeStatType::Min, "Minimum population size"},
            {TimeStatType::Max, "Maximum population size"}
        };

        TimeStatisticsCSVExport      exportBirthStats(fileName + string("-stats-births.csv"), columnsBirths);
        TimeStatisticsCSVExport      exportDeathStats(fileName + string("-stats-deaths.csv"), columnsDeaths);
        TimeStatisticsCSVExport exportPopulationStats(fileName + string("-stats-population.csv"), columnsPopulation);

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
                           aDistribution,
                           sDistribution,
                           nPeople);
            printf("\n");
        }

        for (int i = 0; i < nTrajectories; ++i)
        {
            exportBirths.Add(birthsArr[i]);
            exportDeaths.Add(deathsArr[i]);
            exportPopulation.Add(populationArr[i]);

            exportBirthsPyr.Add(birthsPyrArr[i]);
            exportDeathsPyr.Add(deathsPyrArr[i]);
            exportPopulationPyr.Add(populationPyrArr[i]);

            exportBirthStats.Add(birthStatisticsArr[i]);
            exportDeathStats.Add(deathStatisticsArr[i]);
            exportPopulationStats.Add(populationStatisticsArr[i]);
        }

        // This is likely where summary statistics would be run

        exportBirths.Write();
        exportDeaths.Write();
        exportPopulation.Write();

        exportBirthsPyr.Write();
        exportDeathsPyr.Write();
        exportPopulationPyr.Write();

        exportBirthStats.Write();
        exportDeathStats.Write();
        exportPopulationStats.Write();

    }

    void PyramidBirthDeathSim::_runTrajectory(IncidenceTimeSeries<int>    *births,        \
                                              IncidencePyramidTimeSeries  *birthsPyr,     \
                                              IncidenceTimeSeries<int>    *deaths,        \
                                              IncidencePyramidTimeSeries  *deathsPyr,     \
                                              PrevalenceTimeSeries<int>   *population,    \
                                              PrevalencePyramidTimeSeries *populationPyr, \
                                              Binomial   *bDistribution,                  \
                                              Bernoulli  *dDistribution,                  \
                                              Uniform    *aDistribution,                  \
                                              Bernoulli  *sDistribution,                  \
                                              int nPeople) {

        vector<Individual> individuals{};
        int nBirths, nDeaths, delta;

        auto sexN = [](Individual i) { switch(i.sex) { case Sex::Male   : return 0;
                                                       case Sex::Female : return 1; } };

        // Generate all individuals
        for (int i = 0; i < nPeople; ++i)
        {
            Individual idv = newIndividual(rng);

            individuals.push_back(idv);
            populationPyr->UpdateByAge(0, sexN(idv), idv.age, 1);
        }

        population->Record(0, nPeople);
        delta = 0;

        for (int t = 1; t < timeMax; ++t)
        {
            printf("t = %d\n", t);
            nDeaths = 0;

            // Iterate through each individual and decide if they're going to
            // die.
            auto it = individuals.cbegin();
            while (it != individuals.cend()) {

                // If the individual is going to die.
                // This call returns a 'long' valued at 0 or 1
                if ((bool)dDistribution->Sample(*rng) == 1) {

                    // Increase the number of deaths, decrement the population
                    //   the individual belongs to by 1, increment the number
                    //   of deaths in that population by one, and erase the
                    //   individual from the 'individuals' vector
                    nDeaths += 1;
                    populationPyr->UpdateByAge(t, sexN(*it), (*it).age, -1);
                    deathsPyr->UpdateByAge(t, sexN(*it), (*it).age, +1);
                    it = individuals.erase(it);
                } else it = next(it);
            }

            // For each in nBirths
                // Get a new individual
                // Push them onto the individuals vector
                // Add them to the population pyramid
                // Add them to the births pyramid
            nBirths  = (int)bDistribution->Sample(*rng);
            for (int i = 0; i < nBirths; ++i)
            {
                Individual idv = newIndividual(rng);
                idv.age = 0; // They were just born so they are 0 years old

                individuals.push_back(idv);
                populationPyr->UpdateByAge(t, sexN(idv), idv.age, +1);
                birthsPyr->UpdateByAge(t, sexN(idv), idv.age, +1);
            }

            // Update nPeople
            delta    = nBirths - nDeaths;
            nPeople += delta;

            // Update birth, death, population timeseries
            births->Record(t, nBirths);
            deaths->Record(t, nDeaths);
            population->Record(t, delta);

            printf("t=%2d: %d births, %d deaths, %d population size\n", t, nBirths, nDeaths, nPeople);

            // Refresh the birth distribution to reflect changes in population
            // size.
            refreshDists(nPeople, &bDistribution);
        }

        births->Close();
        deaths->Close();
        population->Close();

        birthsPyr->Close();
        deathsPyr->Close();
        populationPyr->Close();
    }

    Individual PyramidBirthDeathSim::newIndividual(RNG *rng) {
        Individual idv;

        // Assign sex
        if ((bool)sDistribution->Sample(*rng))
            idv.sex = Sex::Male;
        else
            idv.sex = Sex::Female;

        // Assign age
        idv.age = (int)aDistribution->Sample(*rng);

        return idv;
    }

    // Create new distributions in-place to reflect changes in the size of
    //   the population.
    void PyramidBirthDeathSim::refreshDists(int nPeople,
                                            Binomial **bDistribution) {
        *bDistribution = new(*bDistribution) Binomial(nPeople, pBirth);
    }
}
