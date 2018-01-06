#include "../include/SimulationLib/Param.h"
#include <iostream>
#include <limits>


namespace SimulationLib {
  using namespace std;
  using namespace StatisticalDistributions;

  string Param::getDescription(){
  	return description;
  }

	SimulationLib::Type Param::getType() {
		return type;
	}

	GeneralStatDist Param::getDistribution() {
		return distribution;
	}

	long double Param::Sample(RNG &rng) {
		return distribution.Sample(rng);
	}

	string Param::getFileName() {
		return filename;
	}

	bool Param::getCalibration(){
		return calibrate;
	}
}