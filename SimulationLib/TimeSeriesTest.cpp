#include "IncidenceTimeSeries.h"
#include "PrevalenceTimeSeries.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace SimulationLib;

#define SCALAR_DEATH 0.8

void printVector(char *name, vector<int> v)
{
    int i = 0;

    printf("%s:\n", name);
    for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
        printf("\t[%2d]: %2d\n", i++, *it);
    printf("\n");
}

// Parameters:
// 1: Length of the random walk in time units
// 2: Period length of precedence
// 3: Period length of incidence
// 4: time0
// 5: timeMax
// 6: Max number of births in 1 time unit
int main(int argc, char const *argv[])
{
    srand(time(NULL));

    if (argc < 6) {
        printf("Error: Too few paramters\n");
        return 1;
    }

    int i = 0;
    int walk_length = atoi(argv[++i]);
    int period1     = atoi(argv[++i]);
    int period2     = atoi(argv[++i]);
    int time0       = atoi(argv[++i]);
    int timeMax     = atoi(argv[++i]);
    int maxBirths   = atoi(argv[++i]);

    PrevalenceTimeSeries<int>
      population("Population size", timeMax, period1);
    IncidenceTimeSeries<int>
      births("Births per period", time0, timeMax, period2);
    IncidenceTimeSeries<int>
      deaths("Deaths per period", time0, timeMax, period2);

    int numBirths, numDeaths;
    double off1, off2;
    for (int i = 0; i < walk_length; ++i)
    {
        numBirths = rand() % (maxBirths + 1);
        numDeaths = rand() % ((int)(SCALAR_DEATH * maxBirths) + 1);

        off1 = !i ? 0 : (double)(rand() % 100) / 100;
        off2 = !i ? 0 : (double)(rand() % 100) / 100;

        births.Record(i + off1, numBirths);
        deaths.Record(i + off2, numDeaths);
        population.Record(i, numBirths - numDeaths);

        printf("t%4.2f: %3d births\n", i + off1, numBirths);
        printf("t%4.2f: %3d deaths\n\n", i + off2, numDeaths);
    }

    printVector("Total population per period1", population.GetObservations());
    printVector("Births per period2", births.GetObservations());
    printVector("Deaths per period2", deaths.GetObservations());

    return 0;
}
