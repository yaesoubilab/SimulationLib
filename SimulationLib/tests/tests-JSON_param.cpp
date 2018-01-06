// This file simply injects the Catch testing framework, which then detects
//   all Catch tests available via linkage to other tests-*.cpp files
//   and produces an executable to run all tests

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../include/SimulationLib/JSONParameterize.h"
using namespace std;
using namespace SimulationLib;

TEST_CASE("JSON_Parameterize", "[JSON IMPORT]"){
	vector<Param> p;
	string input_file = "input/demographic-settings.json";
	json j = JSONImport::fileToJSON(input_file);
	cout << j[0] << "\n";
	cout << j[1] << "\n";

	json::iterator it;

	for (it = j.begin(); it != j.end(); ++it){
		p.push_back(parameterize(*it));
	}

	REQUIRE(p.at(0).getDescription() == j[0]["description"]);
	REQUIRE(p.at(1).getType() == 
		(j[1]["type"] == "v" ? Type::val_type : Type::file_type));
	REQUIRE(p.at(5).getFileName() == j[5]["distribution"]);
	REQUIRE(p.at(2).getCalibration() == j[2]["included-in-calibration"]);
	REQUIRE(p.at(4).getCalibration() == false);
}
