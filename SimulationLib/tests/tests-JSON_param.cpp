// This file simply injects the Catch testing framework, which then detects
//   all Catch tests available via linkage to other tests-*.cpp files
//   and produces an executable to run all tests

#include "catch.hpp"
#include "../include/SimulationLib/JSONParameterize.h"
#include "../include/SimulationLib/DataFrame.h"
using namespace std;
using namespace SimulationLib;

TEST_CASE("JSON_Parameterize", "[JSON IMPORT]"){
	vector<Param> p;
	string input_file = "input/demographic-settings.json";
	json j = JSONImport::fileToJSON(input_file);
	cout << j[0] << "\n";
	cout << j[1] << "\n";

	json::iterator it;
	DataFrame d;
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

TEST_CASE("JSON_mapshortnames", "[JSON IMPORT]"){
	vector<Param> p;
	string input_file = "input/demographic-settings.json";
	json j = JSONImport::fileToJSON(input_file);
	cout << j[0] << "\n";
	cout << j[1] << "\n";

	map<string, Param> m;
	mapShortNames(j, m);
	//cout << m["pMarriageFailure"].getDescription() << "\n";
	REQUIRE(m["rHouseholdIncrease"].getDescription() == j[0]["description"]);
	REQUIRE(m["pMale"].getDescription() == j[1]["description"]);
	REQUIRE(m["pMarriageFailure"].getDescription() == j[2]["description"]);
	REQUIRE(m["marraigeAgeDiff"].getCalibration() == j[3]["included-in-calibration"]);
}