#include <string>
#include "SerialBirthDeathSim.h"

using namespace SimulationLib;

// Parameters:
// 1: fileName:
//      Prefix of .csv file name (do not specify extension). Four files will
//        be created: [fileName]-pyramid.csv, [fileName]-births.csv,
//        [fileName]-deaths.csv, [filename]-population.csv
// 2: nTrajectories:
//      number of trajectories to run under the following parameters:
// 3: timeMax:
//      largest value of t to run simulation to
// 4: periodLength:
//      length of period, in units of t (integer)
// 5: nPeople:
//      initial size of population
// 6: pDeath:
//      individual probability of death over 1 time unit
// 7: pBirth:
//      individual probability of single reproduction over 1 time unit
// 8: ageMin:
//      minimum age of an individual in the population
// 9: ageMax:
//      maximum age of an individual in the population
int main(int argc, char const *argv[])
{
    int i;
    string fileName;
    int nTrajectories, timeMax, periodLength;
    long nPeople;
    double pDeath, pBirth;
    int ageMin, ageMax;

    SerialBirthDeathSim *bdsim;

    if (argc < 8) {
        printf("Error: too few arguments\n");
        exit(1);
    }

    i = 0;
    fileName      = string(argv[++i]);
    nTrajectories = atoi(argv[++i]);
    timeMax       = atoi(argv[++i]);
    periodLength  = atoi(argv[++i]);
    nPeople       = atol(argv[++i]);
    pDeath        = stof(argv[++i], NULL);
    pBirth        = stof(argv[++i], NULL);
    ageMin        = atoi(argv[++i]);
    ageMax        = atoi(argv[++i]);

    printf("Args:\n\tfileName=%s\n\tnTrajectories=%d\n\ttimeMax=%d\n\tperiodLength=%d\n\tnPeople=%ld\n\tpDeath=%4.4f\n\tpBirth=%4.4f\n\n", fileName.c_str(), nTrajectories, timeMax, periodLength, nPeople, pDeath, pBirth);

    bdsim = new PyramidBirthDeathSim \
                  (fileName, nTrajectories, timeMax, periodLength, nPeople, \
                   pDeath, pBirth, ageMin, ageMax);
    bdsim->Run();
    delete bdsim;

    return 0;
}
