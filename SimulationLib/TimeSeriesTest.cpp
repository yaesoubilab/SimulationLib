#include "IncidenceTimeSeries.h"
#include "PrevalenceTimeSeries.h"
#include "DiscreteTimeStatistic.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace SimulationLib;

typedef struct _Point {
    double time;
    int value;
} Point;


Point newPoint(double timeMin, int timeRange, int valueMin, int valueMax) {
    Point newPoint;

    newPoint.time = timeMin + (double)timeRange * ((double)(rand() % 100) / 100);
    newPoint.value = (valueMax - valueMin) * ((double)(rand() % 100) / 100) + valueMin;

    return newPoint;
}

void report(DiscreteTimeStatistic *stats)
{
    double sum, count, mean, variance, min, max;

    sum      = stats->GetSum();
    count    = stats->GetCount();
    mean     = stats->GetMean();
    variance = stats->GetVariance();
    min      = stats->GetMin();
    max      = stats->GetMax();

    printf("sum=%4.2f, count=%4.2f, mean=%4.2f, variance=%4.2f, min=%4.2f, max=%4.2f\n", \
           sum, count, mean, variance, min, max);
}

// Parameters:
// 1: Length of random walk in time units
// 2: Aggregation period length
// 3: Min number of births/deaths per incident
// 4: Max number of births/deaths per incident

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    if (argc < 5) {
        printf("Error: Too few parameters\n");
        return 1;
    }

    int i = 0;
    int walk_length   = atoi(argv[++i]);
    int period_length = atoi(argv[++i]);
    int min_change    = atoi(argv[++i]);
    int max_change    = atoi(argv[++i]);

    DiscreteTimeStatistic stats("Population size stats");
    IncidenceTimeSeries<int> population("Population size", 0,
                                        (double)walk_length, period_length,
                                        RECORD_ON_ALL, &stats);

    double lastTime = 0;
    Point pt;
    for (int i = 0; i < walk_length; ++i)
    {
        pt = newPoint(lastTime, 1, min_change, max_change);

        printf("pt: t=%4.2f, v=%2d\n", pt.time, pt.value);

        population.Record(pt.time, pt.value);

        report(&stats);

        lastTime = pt.time;
    }

    return 0;
}


// #define SCALAR_DEATH 0.8

// void printVector(char *name, vector<int> v)
// {
//     int i = 0;

//     printf("%s:\n", name);
//     for (vector<int>::iterator it = v.begin(); it != v.end(); ++it)
//         printf("\t[%2d]: %2d\n", i++, *it);
//     printf("\n");
// }

// Parameters:
// 1: Length of the random walk in time units
// 2: Period length of precedence
// 3: Period length of incidence
// 4: time0
// 5: timeMax
// 6: Max number of births in 1 time unit
// int main(int argc, char const *argv[])
// {
//     srand(time(NULL));

//     if (argc < 6) {
//         printf("Error: Too few paramters\n");
//         return 1;
//     }

//     int i = 0;
//     int walk_length = atoi(argv[++i]);
//     int period1     = atoi(argv[++i]);
//     int period2     = atoi(argv[++i]);
//     int time0       = atoi(argv[++i]);
//     int timeMax     = atoi(argv[++i]);
//     int maxBirths   = atoi(argv[++i]);

//     PrevalenceTimeSeries<int>
//       population("Population size", timeMax, period1);
//     IncidenceTimeSeries<int>
//       births("Births per period", time0, timeMax, period2);
//     IncidenceTimeSeries<int>
//       deaths("Deaths per period", time0, timeMax, period2);

//     int numBirths, numDeaths;
//     double off1, off2;
//     for (int i = 0; i < walk_length; ++i)
//     {
//         numBirths = rand() % (maxBirths + 1);
//         numDeaths = rand() % ((int)(SCALAR_DEATH * maxBirths) + 1);

//         off1 = !i ? 0 : (double)(rand() % 100) / 100;
//         off2 = !i ? 0 : (double)(rand() % 100) / 100;

//         births.Record(i + off1, numBirths);
//         deaths.Record(i + off2, numDeaths);
//         population.Record(i, numBirths - numDeaths);

//         printf("t%4.2f: %3d births\n", i + off1, numBirths);
//         printf("t%4.2f: %3d deaths\n\n", i + off2, numDeaths);
//     }

//     printVector("Total population per period1", population.GetObservations());
//     printVector("Births per period2", births.GetObservations());
//     printVector("Deaths per period2", deaths.GetObservations());

//     return 0;
// }
