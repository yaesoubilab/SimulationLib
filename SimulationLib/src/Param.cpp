#include "../include/SimulationLib/Param.h"
#include <iostream>
#include <limits>
#include "Constant.h"

namespace SimulationLib {
  using namespace std;
  using namespace StatisticalDistributions;

  Param::Param(): distribution(GeneralStatDist(Constant(0))), 
  description(""), 
  type(Type::inv_type),
  filename(""),
  calibrate(false) {
  }
  // Param::~Param(){

  // }
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

	long double Param::pdf(long double val) {
		return distribution.pdf(val);
	}

	string Param::getFileName() {
		return filename;
	}

	bool Param::getCalibration(){
		return calibrate;
	}
}