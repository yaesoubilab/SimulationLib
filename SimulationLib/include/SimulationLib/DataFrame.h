#pragma once
#include <memory>
#include "GeneralStatDist.h"
#include <random>
#include "RNG.h"
#include <json.hpp>

// Holds a parameter to the model.
// Christina: Simpler implementation of dataframe class.
// Does not use variant or require C++17.

namespace SimulationLib {

  using namespace std;
  using namespace StatisticalDistributions;
  using json = nlohmann::json;
  // The type of the 'distribution'. Might just be a long double.
  class DataFrame {
  private:
    double year;
    int sex; //0 is male, 1 is female
    double age; 
    GeneralStatDist distribution;

  public:
    //must creates json_obj
    // Param(JSON_OBJ);
    DataFrame();
    DataFrame(double yr, int sx, double a, GeneralStatDist dist) 
      : year{yr}, sex{sx}, age{a}, distribution{dist} {

    }

    double getYear();
    int getSex();
    double getAge();
    GeneralStatDist getDistribution();
    long double Sample(RNG &rng);
  };

  class DataFrameFile{
  private:
    bool ignoreTime, ignoreAge, ignoreGender;
    double timeBracket, ageBracket, timeStart, ageStart;
    int timeCats, ageCats;
    void fillArray(json j);
    int getIndex(double time, int sex, double age);
    DataFrame *df; 
  public:
    DataFrameFile();
    DataFrameFile(json j);
    long double getValue(double time, int sex, double age, RNG myRNG);
    double nextBracketStart(double age);
    double timeToNextBracket(double age);
    double getTimeBracket();
    int getTimeCats();
    double getAgeBracket();
    int getAgeCats();
    double getTimeStart();
    double getAgeStart();
    void printLine(double time, int sex, double age);
  };


}
