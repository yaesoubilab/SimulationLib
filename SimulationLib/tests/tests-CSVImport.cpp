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
