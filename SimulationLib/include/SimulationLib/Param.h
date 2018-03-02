#pragma once
#include <memory>
#include "GeneralStatDist.h"
#include <random>


// Holds a parameter to the model.
// Christina: Simpler implementation of parameter class.
// Does not use variant or require C++17.

namespace SimulationLib {

  enum class Type {
    val_type=1, file_type=2, inv_type=3
  };

  using namespace std;
  using namespace StatisticalDistributions;
  // The type of the 'distribution'. Might just be a long double.
  class Param {
  private:
    string description;
    SimulationLib::Type type;
    GeneralStatDist distribution;
    string filename;
    bool calibrate;

  public:
    //must creates json_obj
    // Param(JSON_OBJ);
    Param();
    Param(string desc, SimulationLib::Type t, GeneralStatDist dist, string fn, bool cal) 
      : description{desc}, type{t}, distribution{dist}, filename{fn}, calibrate{cal} {

    }
    // ~Param();

    string getDescription();
    SimulationLib::Type getType();
    GeneralStatDist getDistribution();
    long double Sample(RNG &rng);
    string getFileName();
    bool getCalibration();
  };


}
