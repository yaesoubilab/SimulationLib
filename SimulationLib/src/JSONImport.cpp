#include "../include/SimulationLib/JSONImport.h"
#include <fstream>

namespace SimulationLib {

  json JSONImport::fileToJSON(string fname){
  	//read json from file
  	std::ifstream i(fname);
	json j;
	i >> j;
	if (!JSONImport::validateJSON(j)){
		//return some error
	}
	return j;
  }

  bool JSONImport::validateJSON(json j){
  	// ADD JSON VALIDATION HERE
  	return true;
  }

 }