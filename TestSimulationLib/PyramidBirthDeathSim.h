#pragma once
#include <ctime>
#include <string>
#include <map>

#include <Binomial.h>
#include <Bernoulli.h>
#include <RNG.h>
#include <Uniform.h>

#include <PrevalenceTimeSeries.h>
#include <IncidenceTimeSeries.h>
#include <PrevalencePyramidTimeSeries.h>
#include <IncidencePyramidTimeSeries.h>
#include <ContinuousTimeStatistic.h>
#include <DiscreteTimeStatistic.h>
#include <CSVExport.h>

using namespace std;
using namespace StatisticalDistributions;

namespace SimulationLib {
    enum class Sex {Male=0, Female=1};

    using Individual =
      struct {
        int age;
        Sex sex;
      };

    class PyramidBirthDeathSim
    {
    public:
        PyramidBirthDeathSim(string fileName, int nTrials, int timeMax, int periodLength, \
                             long nPeople, double pDeath, double pBirth, int ageMin, int ageMax);
        ~PyramidBirthDeathSim(void);
        void Run(void);

    private:
        string fileName;
        int nTrajectories, timeMax, periodLength;
        long nPeople;
        double pDeath, pBirth;
        int ageMin, ageMax;

        RNG *rng;

        void _runTrajectory(IncidenceTimeSeries<int>  *births,          \
                            IncidencePyramidTimeSeries *birthsPyr,      \
                            IncidenceTimeSeries<int>  *deaths,          \
                            IncidencePyramidTimeSeries *deathsPyr,      \
                            PrevalenceTimeSeries<int> *population,      \
                            PrevalencePyramidTimeSeries *populationPyr, \
                            Binomial  *bDistribution,                   \
                            Bernoulli *dDistribution,                   \
                            Uniform   *aDistribution,                   \
                            Bernoulli *sDistribution,                   \
                            int nPeople);

        void refreshDists(int nPeople, Binomial **bDistribution);
        Individual newIndividual(RNG *rng);

        Binomial                    **bDistributionArr; // Birth distributions
                                                        //   (nPeople-dependent)
        Bernoulli                    *dDistribution;    // Death distribution
        Uniform                      *aDistribution;    // Age distribution
        Bernoulli                    *sDistribution;    // Sex distribution

        IncidenceTimeSeries<int>    **birthsArr;
        IncidencePyramidTimeSeries  **birthsPyrArr;
        DiscreteTimeStatistic       **birthStatisticsArr;

        IncidenceTimeSeries<int>    **deathsArr;
        IncidencePyramidTimeSeries  **deathsPyrArr;
        DiscreteTimeStatistic       **deathStatisticsArr;


        PrevalenceTimeSeries<int>   **populationArr;
        PrevalencePyramidTimeSeries **populationPyrArr;
        ContinuousTimeStatistic     **populationStatisticsArr;

        vector<double>                 defaultAgeBreaks;
    };
}
