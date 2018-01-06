#include "catch.hpp"

#include <fstream>

// Provides "compare_files" function for comparing output of CSVExporter's
//   to reference output
#include "utils/compare_files.h"

#include "../include/SimulationLib/CSVImport.h"

using namespace SimulationLib;
using namespace SimulationLib::CSVImport;
using namespace Catch;

TEST_CASE("4x4 square - columns", "[csvimport]") {
    std::ifstream f0("tests-CSVExport-files/test-csvimport-01.csv", std::ios::in);

    DataByColumns data = CSVToColumns(f0, "header1", "header2");

    f0.close();

    REQUIRE( data.second[0][0] == "1" );
    REQUIRE( data.second[0][1] == "3" );
    REQUIRE( data.second[1][0] == "2" );
    REQUIRE( data.second[1][1] == "4" );
}

TEST_CASE("4x4 square - rows", "[csvimport]") {
    std::ifstream f0("tests-CSVExport-files/test-csvimport-01.csv", std::ios::in);

    DataByRows data = CSVToRows(f0, "header1", "header2");

    f0.close();

    REQUIRE( data.second[0][0] == "1" );
    REQUIRE( data.second[0][1] == "2" );
    REQUIRE( data.second[1][0] == "3" );
    REQUIRE( data.second[1][1] == "4" );
}

TEST_CASE("simple PyramidTimeSeries - columns", "[csvimport]") {
    std::ifstream f0("tests-CSVExport-files/test-csvimport-02.csv", std::ios::in);

    DataByColumns data = CSVToColumns(f0, "Age Group", "Category 0", "Category 1");

    f0.close();

    // CSV file contents:
    //
    // Age Group,Category 0,Category 1
    // 0-10,0,2
    // 10-20,1,0
    // 20-30,0,0
    // 30-inf,150,0

    using StringVec = std::vector<std::string>;

    REQUIRE_THAT(data.first,     Equals(StringVec{"Age Group", "Category 0", "Category 1"}));

    REQUIRE_THAT(data.second[0], Equals(StringVec{"0-10", "10-20", "20-30", "30-inf"}));

    REQUIRE_THAT(data.second[1], Equals(StringVec{"0", "1", "0", "150"}));
    REQUIRE_THAT(data.second[2], Equals(StringVec{"2", "0", "0", "0"}));
}

TEST_CASE("simple PyramidTimeSeries - rows", "[csvimport]") {
    std::ifstream f0("tests-CSVExport-files/test-csvimport-02.csv", std::ios::in);

    DataByRows data = CSVToRows(f0, "Age Group", "Category 0", "Category 1");

    f0.close();

    // CSV file contents:
    //
    // Age Group,Category 0,Category 1
    // 0-10,0,2
    // 10-20,1,0
    // 20-30,0,0
    // 30-inf,150,0

    using StringVec = std::vector<std::string>;

    REQUIRE_THAT(data.first,     Equals(StringVec{"Age Group", "Category 0", "Category 1"}));
    REQUIRE_THAT(data.second[0], Equals(StringVec{"0-10",      "0",          "2"}));
    REQUIRE_THAT(data.second[1], Equals(StringVec{"10-20",     "1",          "0"}));
    REQUIRE_THAT(data.second[2], Equals(StringVec{"20-30",     "0",          "0"}));
    REQUIRE_THAT(data.second[3], Equals(StringVec{"30-inf",    "150",        "0"}));
}

TEST_CASE("empty cells - columns", "[csvimport]") {
    std::ifstream f0("tests-CSVExport-files/test-csvimport-03.csv", std::ios::in);

    DataByColumns data = CSVToColumns(f0, "time", "population 1", "population 2");

    f0.close();

    // CSV file contents:
    //
    // time,population 1, population 2
    // 0,15,25
    // 1,25,35
    // 2,0,
    // 3,,1

    using StringVec = std::vector<std::string>;

    REQUIRE_THAT(data.first,     Equals(StringVec{"time", "population 1", "population 2"}));

    REQUIRE_THAT(data.second[0], Equals(StringVec{"0", "1", "2", "3"}));
    REQUIRE_THAT(data.second[1], Equals(StringVec{"15", "25", "0", ""}));
    REQUIRE_THAT(data.second[2], Equals(StringVec{"25", "35", "", "1"}));
}

TEST_CASE("empty cells - rows", "[csvimport]") {
    std::ifstream f0("tests-CSVExport-files/test-csvimport-03.csv", std::ios::in);

    DataByRows data = CSVToRows(f0, "time", "population 1", "population 2");

    f0.close();

    // CSV file contents:

    // time,population 1, population 2
    // 0,15,25
    // 1,25,35
    // 2,0,
    // 3,,1

    using StringVec = std::vector<std::string>;

    REQUIRE_THAT(data.first,     Equals(StringVec{"time", "population 1", "population 2"}));
    REQUIRE_THAT(data.second[0], Equals(StringVec{"0",    "15",           "25"          }));
    REQUIRE_THAT(data.second[1], Equals(StringVec{"1",    "25",           "35"          }));
    REQUIRE_THAT(data.second[2], Equals(StringVec{"2",    "0",            ""            }));
    REQUIRE_THAT(data.second[3], Equals(StringVec{"3",    "",             "1"           }));
}

TEST_CASE("Basic PrevalenceTimeSeries Import", "[csvimport]") {
    std::ifstream f0("tests-CSVExport-files/test-t01-answer.csv", std::ios::in);
    std::ifstream f1("tests-CSVExport-files/test-t02-answer.csv", std::ios::in);
    DataByRows data1 = CSVToRows(f0, "Period", "Population 1");
    DataByRows data2 = CSVToRows(f1, "Period", "Population 1", "Population 2");
    f0.close();
    f1.close();
    // CSV file contents:

    // Period,Population 1,Population 2
    // 0,1,150
    // 1,3,351
    // 2,10,

    using StringVec = std::vector<std::string>;

    REQUIRE_THAT(data1.first,     Equals(StringVec{"Period", "Population 1"}));
    REQUIRE_THAT(data1.second[0], Equals(StringVec{"0",    "1"}));
    REQUIRE_THAT(data1.second[1], Equals(StringVec{"1",    "3"}));
    REQUIRE_THAT(data1.second[2], Equals(StringVec{"2",    "10"}));

    REQUIRE_THAT(data2.first,     Equals(StringVec{"Period", "Population 1", "Population 2"}));
    REQUIRE_THAT(data2.second[0], Equals(StringVec{"0",    "1", "150"}));
    REQUIRE_THAT(data2.second[1], Equals(StringVec{"1",    "3", "351"}));
    REQUIRE_THAT(data2.second[2], Equals(StringVec{"2",    "10", ""}));
   
}

TEST_CASE("Basic PyramidTimeSeries Import", "[csvimport]") {
    std::ifstream f0("tests-CSVExport-files/test-t03-answer.csv", std::ios::in);
    std::ifstream f1("tests-CSVExport-files/test-t04-answer.csv", std::ios::in);
    DataByRows data1 = CSVToRows(f0, "Period", "Age Group", "[Population 3]Category 0");
    DataByRows data2 = CSVToRows(f1, "Period", "Age Group", "[Population 4]Category 0", "[Population 4]Category 1");
    f0.close();
    f1.close();
    // CSV file contents:

    // Period,Age Group,[Population 3]Category 0
    // 0,0-5,10
    // 0,5-10,12
    // 0,10-inf,14
    // 1,0-5,9
    // 1,5-10,14
    // 1,10-inf,17
    // 2,0-5,11
    // 2,5-10,11
    // 2,10-inf,18
    // 3,0-5,11
    // 3,5-10,11
    // 3,10-inf,18
    // 4,0-5,0
    // 4,5-10,0
    // 4,10-inf,0
    // 5,0-5,0
    // 5,5-10,0
    // Period,Age Group,[Population 4]Category 0,[Population 4]Category 1
    // 0,0-10,10,10
    // 0,10-20,10,10
    // 0,20-30,10,10
    // 0,30-inf,10,10
    // 1,0-10,14,11
    // 1,10-20,11,8
    // 1,20-30,12,12
    // 1,30-inf,13,10
    // 2,0-10,18,9
    // 2,10-20,14,7
    // 2,20-30,11,14
    // 2,30-inf,16,11
    // 3,0-10,0,0
    // 3,10-20,0,0
    // 3,20-30,0,0
    // 3,30-inf,0,0


    using StringVec = std::vector<std::string>;

    REQUIRE_THAT(data1.first,     Equals(StringVec{"Period", "Age Group", "[Population 3]Category 0"}));
    REQUIRE_THAT(data1.second[0], Equals(StringVec{"0",    "0-5",           "10"}));
    REQUIRE_THAT(data1.second[1], Equals(StringVec{"0",    "5-10",          "12"}));
    REQUIRE_THAT(data1.second[2], Equals(StringVec{"0",    "10-inf",        "14"}));
    REQUIRE_THAT(data1.second[3], Equals(StringVec{"1",    "0-5",           "9"}));
    REQUIRE_THAT(data1.second[4], Equals(StringVec{"1",    "5-10",          "14"}));
    REQUIRE_THAT(data1.second[5], Equals(StringVec{"1",    "10-inf",        "17"}));
    REQUIRE_THAT(data1.second[6], Equals(StringVec{"2",    "0-5",           "11"}));
    REQUIRE_THAT(data1.second[7], Equals(StringVec{"2",    "5-10",          "11"}));
    REQUIRE_THAT(data1.second[8], Equals(StringVec{"2",    "10-inf",        "18"}));
    REQUIRE_THAT(data1.second[9], Equals(StringVec{"3",    "0-5",           "11"}));
    REQUIRE_THAT(data1.second[10], Equals(StringVec{"3",    "5-10",          "11"}));
    REQUIRE_THAT(data1.second[11], Equals(StringVec{"3",    "10-inf",        "18"}));
    REQUIRE_THAT(data1.second[12], Equals(StringVec{"4",    "0-5",           "0"}));
    REQUIRE_THAT(data1.second[13], Equals(StringVec{"4",    "5-10",          "0"}));
    REQUIRE_THAT(data1.second[14], Equals(StringVec{"4",    "10-inf",        "0"}));
    REQUIRE_THAT(data1.second[15], Equals(StringVec{"5",    "0-5",           "0"}));
    REQUIRE_THAT(data1.second[16], Equals(StringVec{"5",    "5-10",          "0"}));
    REQUIRE_THAT(data1.second[17], Equals(StringVec{"5",    "10-inf",        "0"}));

    REQUIRE_THAT(data2.first,     Equals(StringVec{"Period", "Age Group", "[Population 4]Category 0", "[Population 4]Category 1"}));
    REQUIRE_THAT(data2.second[0], Equals(StringVec{"0",    "0-10",           "10",                      "10"}));
    REQUIRE_THAT(data2.second[1], Equals(StringVec{"0",    "10-20",          "10",                      "10"}));
    REQUIRE_THAT(data2.second[2], Equals(StringVec{"0",    "20-30",          "10",                      "10"}));
    REQUIRE_THAT(data2.second[3], Equals(StringVec{"0",    "30-inf",         "10",                      "10"}));
    REQUIRE_THAT(data2.second[4], Equals(StringVec{"1",    "0-10",           "14",                      "11"}));
    REQUIRE_THAT(data2.second[5], Equals(StringVec{"1",    "10-20",          "11",                       "8"}));
    REQUIRE_THAT(data2.second[6], Equals(StringVec{"1",    "20-30",          "12",                      "12"}));
    REQUIRE_THAT(data2.second[7], Equals(StringVec{"1",    "30-inf",         "13",                      "10"}));
    REQUIRE_THAT(data2.second[8], Equals(StringVec{"2",    "0-10",           "18",                       "9"}));
    REQUIRE_THAT(data2.second[9], Equals(StringVec{"2",    "10-20",          "14",                       "7"}));
    REQUIRE_THAT(data2.second[10], Equals(StringVec{"2",    "20-30",          "11",                      "14"}));
    REQUIRE_THAT(data2.second[11], Equals(StringVec{"2",    "30-inf",         "16",                      "11"}));
    REQUIRE_THAT(data2.second[12], Equals(StringVec{"3",    "0-10",            "0",                       "0"}));
    REQUIRE_THAT(data2.second[13], Equals(StringVec{"3",    "10-20",           "0",                       "0"}));
    REQUIRE_THAT(data2.second[14], Equals(StringVec{"3",    "20-30",           "0",                       "0"}));
    REQUIRE_THAT(data2.second[15], Equals(StringVec{"3",    "30-inf",          "0",                       "0"}));
}

TEST_CASE("Floats Import", "[csvimport]") {
    std::ifstream f0("tests-CSVExport-files/test-t11-answer.csv", std::ios::in);

    DataByRows data1 = CSVToRows(f0, "Age Group", "Category 0", "Category 1");

    f0.close();
    
    // CSV file contents:

    // Age Group,Category 0,Category 1
    // 0-10,0.000000,2.001000
    // 10-20,1.500000,0.000000
    // 20-30,0.000000,0.000000
    // 30-inf,150.001000,0.000000

    using StringVec = std::vector<std::string>;

    REQUIRE_THAT(data1.first,     Equals(StringVec{"Age Group", "Category 0", "Category 1"}));
    REQUIRE_THAT(data1.second[0], Equals(StringVec{"0-10",    "0.000000",     "2.001000"}));
    REQUIRE_THAT(data1.second[1], Equals(StringVec{"10-20",    "1.500000",    "0.000000"}));
    REQUIRE_THAT(data1.second[2], Equals(StringVec{"20-30",    "0.000000",    "0.000000"}));
    REQUIRE_THAT(data1.second[3], Equals(StringVec{"30-inf",   "150.001000",  "0.000000"}));
   
}

TEST_CASE("Random Chars Import", "[csvimport]") {
    std::ifstream f0("tests-CSVExport-files/test-csvimport-04.csv", std::ios::in);

    DataByRows data1 = CSVToRows(f0, "time", "population 1", "population 2");

    f0.close();
    
    // CSV file contents:

    // Age Group,Category 0,Category 1
    // 0-10,0.000000,2.001000
    // 10-20,1.500000,0.000000
    // 20-30,0.000000,0.000000
    // 30-inf,150.001000,0.000000

    using StringVec = std::vector<std::string>;

    REQUIRE_THAT(data1.first,     Equals(StringVec{"time", "population 1", "population 2"}));
    REQUIRE_THAT(data1.second[0], Equals(StringVec{"-{}+","#$^&*()","!~<>/?"}));
    REQUIRE_THAT(data1.second[1], Equals(StringVec{"123","|=+","::;"}));
    
}


