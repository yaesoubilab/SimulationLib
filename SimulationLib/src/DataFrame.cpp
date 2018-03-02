#include "../include/SimulationLib/Param.h"
#include "../include/SimulationLib/DataFrame.h"
#include <iostream>
#include <limits>
#include "Constant.h"
#include "../include/SimulationLib/JSONParameterize.h"

namespace SimulationLib {
	using namespace std;
	using namespace StatisticalDistributions;

	DataFrame::DataFrame(): 
	year(1900), sex(0), age(0), distribution(GeneralStatDist(Constant(0))) {
	}

	double DataFrame::getYear(){
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

	double DataFrame::getAge() {
		return age;
	}

	DataFrameFile::DataFrameFile(){
		//instantiate empty array
		df = 0;
	}


	DataFrameFile::DataFrameFile(json j){
		fillArray(j);
	}

	void DataFrameFile::fillArray(json j){
		// Extract age and time bracket information
		string curr;
		if ((ignoreTime = (j[0]["Year"] == '-' || j[0]["Year"].is_null())){
			timeBracket = 0;
		} else {
			timeBracket = j[0]["Year"];
		}

		if ((ignoreGender = (j[0]["Sex"] == '-' || j[0]["Sex"].is_null()))){

		} 

		if ((ignoreAge = (j[0]["Age"] == '-' || j[0]["Age"].is_null()))){
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
		
		df = new DataFrame[2*(timeCats*ageCats + 10)];

		if (j.size() < 3){
			return; //some type of error
		}
		if (!ignoreTime){
			timeStart = j[2]["Year"];
		}
		if (!ignoreAge){
			ageStart = j[2]["Age"];
		}
		

		int femaleindex = timeCats*ageCats+10;
		for (int index = 2; index < j.size(); ++index){
		
			int s = 0;
			int m = index-2;		// -2 to account for first two info rows in file
			if (j[index]["Sex"] == "F"){
				s = 1;
				m = femaleindex;
				femaleindex++;
			}
			double year = 1990;
			double age = 0;
			if (!ignoreTime){
				year = j[index]["Year"];
			}
			if (!ignoreAge){
				age = j[index]["Age"];
			}
			DataFrame d (year, s, age,
			 mapDistribution(j[index]));

			df[m] = d;
		}
	} 

	double DataFrameFile::getTimeBracket(){
		return timeBracket;
	}   


	double DataFrameFile::getAgeBracket(){
		return ageBracket;
	}   

	int DataFrameFile::getTimeCats(){
		return timeCats;
	} 

	int DataFrameFile::getAgeCats(){
		return ageCats;
	} 

	double DataFrameFile::getAgeStart(){
		return ageStart;
	} 
	double DataFrameFile::getTimeStart(){
		return timeStart;
	} 
	int DataFrameFile::getIndex(double time, int sex, double age){
		int timeBrk = 0;
		if (!ignoreTime){
			timeBrk = (int)((time - timeStart) / timeBracket);
		}

		if (timeBrk >= timeCats){
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
		int index = (int) (timeBrk * ageCats + ageBrk);

		if (sex == 1){
			//adjust for female index;
			index += timeCats*ageCats + 10 ;
		}
		return index;
	}

	void DataFrameFile::printLine(double time, int sex, double age){
		int index = getIndex(time, sex, age);
		cout << "At index " << index << "\n";
		cout << "Year: " << df[index].getYear() << "\n";
		cout << "Sex: " << df[index].getSex() << "\n";
		cout << "Age: " << df[index].getAge() << "\n";
	}

	long double DataFrameFile::getValue(double time, int sex, double age, RNG myRNG){
		int index = getIndex(time, sex, age);
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