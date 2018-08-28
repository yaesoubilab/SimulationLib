#include "catch.hpp"
#include "../include/SimulationLib/JSONParameterize.h"
#include "../include/SimulationLib/DataFrame.h"
#include "../include/SimulationLib/Param.h"
#include "RNG.h"
#include "Uniform.h"
#include "JohnsonSu.h"
using namespace std;
using namespace SimulationLib;


TEST_CASE("Annual Mortality Rates - Constant", "[DATAFRAME]"){
	vector<Param> p;
	string input_file = "input/AnMortRates.json";
	json j = JSONImport::fileToJSON(input_file);
	cout << j[0] << "\n";
	cout << j[1] << "\n";
	int seed = 10;
  	RNG myRNG(seed);
	json::iterator it;
	// DataFrameFile d{j, false};
	DataFrameFile d {j};

	REQUIRE(d.getTimeBracket() == 1);
	REQUIRE(d.getAgeBracket() == 5);
	REQUIRE(d.getTimeCats() == 43);
	REQUIRE(d.getAgeCats() == 17);
	REQUIRE(d.getTimeStart() == 1998);
	REQUIRE(d.getAgeStart() == 0);
	REQUIRE(d.nextBracketStart(6.4) == 10);

	// Loops through all values in file to check that d returns correct values
	for (int i = 2; i < j.size(); i++){
		int sex = 0;
		if (j[i]["Sex"] == "F")
			sex  = 1;
		double year = j[i]["Year"];
		double rate = j[i]["Annual mortality rate"];
		double age = j[i]["Age"];
		REQUIRE(d.getValue(year, sex, age, myRNG) - rate < 0.00005);
	}
}
TEST_CASE("Time in Marraige - Distribution", "[DATAFRAME]"){
	vector<Param> p;
	string input_file = "input/TimeInMarraige.json";
	json j = JSONImport::fileToJSON(input_file);
	cout << j[0] << "\n";
	cout << j[1] << "\n";
	int seed = 10;
  	RNG myRNG(seed);
	json::iterator it;

	DataFrameFile d {j};

	REQUIRE(d.getAgeBracket() == 5);
	REQUIRE(d.getAgeCats() == 7);
	REQUIRE(d.getAgeStart() == 15);

	// Uniform
	for (int i = 3; i < j.size(); i++){
		int sex = 0;
		double rate = j[i]["Annual mortality rate"];
		double age = j[i]["Age"];

		// REQUIRE(d.getValue(null, sex, age, myRNG) - rate < 0.00005);
	}
}