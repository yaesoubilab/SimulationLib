#include "IncidenceTimeSeries.h"
#include "PrevalenceTimeSeries.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace SimulationLib;

#define SCALAR_DEATH 0.8

// Parameters:
// 1: Length of the random walk in time units
// 2: Aggregation period
// 3: time0 for births
// 4: time0 for deaths
// 5: Max number of births in a time unit
// (6:) "-v" flag produces verbose output
int main(int argc, char const *argv[])
{
    srand(time(NULL));

    if (argc < 6) {
        printf("Error: Too few paramters\n");
        return 1;
    }

    int i = 0;
    int walk_length        = atoi(argv[++i]);
    int aggregation_period = atoi(argv[++i]);
    int time0_births       = atoi(argv[++i]);
    int time0_deaths       = atoi(argv[++i]);
    int max_births         = atoi(argv[++i]);

    // Verbose flag?
    bool verbose = (++i < argc && !strcmp(argv[i], "-v")) ? true : false;

    PrevalenceTimeSeries population("Population size");
    IncidenceTimeSeries  births("Births per month", time0_births, aggregation_period);
    IncidenceTimeSeries  deaths("Deaths per month", time0_deaths, aggregation_period);

    int numBirths;
    int numDeaths;
    for (int i = 0; i < walk_length; ++i)
    {
        numBirths = rand() % (max_births + 1);
        numDeaths = rand() % ((int)(SCALAR_DEATH*max_births) + 1);

        births.Record(i, numBirths);
        deaths.Record(i, numDeaths);

        printf("t%3d: %3d births, %3d deaths\n", i, numBirths, numDeaths);

        if ((i+1) % aggregation_period == 0) {
            // int agBirths = births.GetLastObservation<int>();
            // int agDeaths = deaths.GetLastObservation<int>();
            int agBirths = births.GetLastObservation();
            int agDeaths = deaths.GetLastObservation();

            population.Record(i / aggregation_period, agBirths - agDeaths);

            printf("t%3d: AGGREGATE(%3d): %3d Δ, %3d total\n",
                    i, i/aggregation_period, agBirths-agDeaths, \
                    population.GetCurrentPrevalence());
        }
    }

    return 0;
}
