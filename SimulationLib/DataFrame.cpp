#include "../include/SimulationLib/Param.h"
#include <iostream>
#include <limits>
#include "Constant.h"
#include "../include/SimulationLib/JSONParameterize.h"

namespace SimulationLib {
	  using namespace std;
	  using namespace StatisticalDistributions;

	  DataFrame::DataFrame(): year(1900), sex(0), age(0), distribution(GeneralStatDist(Constant(0))) {
	  }

	  int DataFrame::getYeastd::map<key, value> map;r(){
		return year;
	  }

	int DataFrame::getSex() {
		return sex;
	}

	GeneralStatDist DataFrame::getDistribution() {
		return distribution;
	}

	long double DataFrame::Sample(RNG &rng) {
		return distribution.Sample(rng);
	}

	int DataFrame::getAge() {
		return age;
	}

	DataFrameFile::DataFrameFile(){
		looptime = false;
		df = 0;
	}


	DataFrameFile::DataFrameFile(json j, bool lt){
		looptime = lt;
		fillArray(j);
	}

	DataFrameFile::fillArray(json j){
		char * c;
		if ((ignoreTime = (j[0]["Year"] == '-')){
			timeBracket = 0;
		} else {
			timeBracket = j[0]["Year"];
		}

		if ((ignoreGender = (j[0]["Sex"] == '-')){

		}

		if ((ignoreAge = (j[0]["Age"] == '-'))){
			ageBracket = 0;
		} else{
			ageBracket = j[0]["Age"];
		}

		if (ignoreTime){
			timeCats = 1;
		} else{
			timeCats = j[1]["Year"];
		}

		if (ignoreAge){
			ageCats = 1;
		} else{
			ageCats = j[1]["Age"];
		}
		
		df = new DataFrame[(timeCats*ageCats + 10)];

		if (j.size() < 3){
			return; //some type of error
		}
		timeStart = j[2]["Year"];
		ageStart = j[2]["Age"];
		// Read file
		for (int i = 2; i < j.size(); i++){
			DataFrame d (j[i]["Year"],j[i]["Sex"],j[i]["Age"], mapDistribution(j[i]));
			df[i-2] = d;
		}
	}    

	int DataFrameFile::getIndex(double time, int sex, double age){

		int timeBrk = 0;
		if (!ignoreTime){
			timeBrk = (int)((time - timeStart) / timeBracket);
		}
		if (loopTime){
			timeBrk %= timeCats;
			if (timeBrk < 0)
				timeBrk += timeCats;
		}
		else if (timeBrk >= timeCats){
			timeBrk = timeCats - 1;
		}
		else if (timeBrk < 0){
			timeBrk = 0;
		}

		int ageBrk = 0;
		if (!ignoreAge){
			ageBrk = (int)((age - ageStart) / ageBracket);
		}
		if (ageBrk < 0)
			ageBrk = 0;
		if (ageBrk >= ageCats)
			ageBrk = ageCats - 1;
		int index = (int) (timeBrk * ageBracket + ageBrk);
		return index;
	}

	long double DataFrameFile::getValue(double time, int sex, double age, int seed){
		int index = getIndex(time, sex, age);
		RNG myRNG(seed);
		return df[index].Sample(myRNG);
	}

	double DataFrameFile::nextBracketStart(double age){
		if (ignoreAge || (age > ageBracket * (ageCats - 1) + ageStart))
			return -1;

		if (age < ageStart)
			return ageStart + ageBracket;
		return (((int)((age - ageStart) / ageBracket) + 1) * ageBracket + ageStart);
	}

	double DataFrameFile::timeToNextBracket(double age){
		return (nextBracketStart(age) - age);
	}

}