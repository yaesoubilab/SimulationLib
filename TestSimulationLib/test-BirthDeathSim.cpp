#include "BirthDeathSim.h"

using namespace SimulationLib;

// Parameters:
// 1: timeMax:
//      largest value of t to run simulation to
// 2: periodLength:
//      length of period, in units of t (integer)
// 3: nPeople:
//      initial size of population
// 4: pDeath:
//      individual probability of death over 1 time unit
// 5: pBirth:
//      individual probability of single reproduction over 1 time unit
int main(int argc, char const *argv[])
{
    int timeMax, periodLength;
    long nPeople;
    double pDeath, pBirth;

    BirthDeathSim *bdsim;

    if (argc < 6) {
        printf("Error: too few arguments\n");
        exit(1);
    }

    int i = 0;
    timeMax      = atoi(argv[++i]);
    periodLength = atoi(argv[++i]);
    nPeople      = atol(argv[++i]);
    pDeath       = stof(argv[++i], NULL);
    pBirth       = stof(argv[++i], NULL);

    printf("Args:\n\ttimeMax=%d\n\tperiodLength=%d\n\tnPeople=%ld\n\tpDeath=%4.4f\n\tpBirth=%4.4f\n\n", timeMax, periodLength, nPeople, pDeath, pBirth);

    bdsim = new BirthDeathSim(timeMax, periodLength, nPeople, pDeath, pBirth);

    bdsim->Run();

    delete bdsim;

    return 0;
}
