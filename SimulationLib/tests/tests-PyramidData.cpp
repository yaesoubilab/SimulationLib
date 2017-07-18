#include "catch.hpp"
#include "../include/SimulationLib/PyramidData.h"

using namespace std;
using namespace SimulationLib;

TEST_CASE("PyramidData: weird initializations", "[pyramid]") {
    bool failed;

    // Each section is run independently of one another: see
    //   https://github.com/philsquared/Catch/blob/master/docs/test-cases-and-sections.md
    SECTION( "no categories" ) {
        failed = false;
        vector<double> ageBreaks = {10, 20, 30};

        try {
            PyramidData pd(0, ageBreaks);
        } catch (...) {
            failed = true;
        }

        REQUIRE( failed == true );
    }

    SECTION( "no age breaks" ) {
        failed = false;
        vector<double> ageBreaks = {};

        try {
            PyramidData pd(1, ageBreaks);
        } catch (...) {
            failed = true;
        }

        REQUIRE( failed == false );
    }

    SECTION( "age breaks which make no sense" ) {
        failed = false;
        vector<double> ageBreaks = {0, -1, 3};

        try {
            PyramidData pd(1, ageBreaks);
        } catch (...) {
            failed = true;
        }

        REQUIRE( failed == true );
    }
}

TEST_CASE( "PyramidData: invalid updates" ) {
    int numCategories = 1;
    vector<double> ageBreaks = {10};
    bool threw;

    PyramidData pd(numCategories, ageBreaks);

    REQUIRE( pd.UpdateByAge(0, 5, -10) == false );
    REQUIRE( pd.UpdateByIdx(0, 0, 20) == true );
    REQUIRE( pd.UpdateByIdx(0, 0, -30) == false );

    threw = false;
    try {
        pd.UpdateByIdx(0, 5, 1);
    } catch(...) {
        threw = true;
    }

    REQUIRE( threw == true );
}

TEST_CASE("PyramidData: one category, no age-breaks", "[pyramid]") {
    int numCategories = 1;
    vector<double> ageBreaks = {};

    PyramidData pd(numCategories, ageBreaks);

    REQUIRE( pd.GetTotalInCategory(0) == 0 );
    REQUIRE( pd.GetTotalInAgeGroup(0) == 0 );
    REQUIRE( pd.GetTotal() == 0 );

    pd.UpdateByIdx(0, 0, 1);
    REQUIRE( pd.GetTotal() == 1 );
    pd.UpdateByAge(0, 0, 1);
    REQUIRE( pd.GetTotal() == 2 );
    pd.UpdateByAge(0, 100, 1);
    REQUIRE( pd.GetTotal() == 3 );
}

TEST_CASE("PyramidData: one age-break", "[pyramid]") {
    int numCategories = 1;
    vector<double> ageBreaks = {10};

    PyramidData pd(numCategories, ageBreaks);

    REQUIRE( pd.GetTotalInCategory(0) == 0 );
    REQUIRE( pd.GetTotalInAgeGroup(0) == 0 );
    REQUIRE( pd.GetTotal() == 0 );

    SECTION( "Simple updates to categories: UpdateByIdx" ) {
        pd.UpdateByIdx(0, 0, 17); // 17 people of age [0, 10)
        pd.UpdateByIdx(0, 1, 18); // 18 people of age [10, +inf)

        REQUIRE( pd.GetTotal() == 35 );
        REQUIRE( pd.GetTotalInCategory(0) == 35 );
        REQUIRE( pd.GetTotalInAgeGroup(0) == 17 );
        REQUIRE( pd.GetTotalInAgeGroup(1) == 18 );
    }

    SECTION( "Simple updates to categories: UpdateByAge" ) {
        pd.UpdateByAge(0, 5, 17);  // Add 17 people of age 5
        pd.UpdateByAge(0, 15, 18); // Add 18 people of age 15

        REQUIRE( pd.GetTotal() == 35 );
        REQUIRE( pd.GetTotalInCategory(0) == 35 );
        REQUIRE( pd.GetTotalInAgeGroup(0) == 17 );
        REQUIRE( pd.GetTotalInAgeGroup(1) == 18 );
    }
}

TEST_CASE("PyramidData: moving", "[pyramid]") {
    bool threw;
    int numCategories = 2;
    vector<double> ageBreaks = {10, 20};

    PyramidData pd(numCategories, ageBreaks);

    pd.UpdateByAge(0, 5, 33);
    pd.UpdateByAge(0, 15, 66);

    REQUIRE( pd.GetTotalInAgeGroup(0) == 33 );
    REQUIRE( pd.GetTotalInAgeGroup(1) == 66 );
    REQUIRE( pd.GetTotalInCategory(0) == 99 );

    SECTION( "Moves that should throw exceptions" ) {
        threw = false;

        SECTION( "Old group invalid" ) {
            try {
                pd.MoveByIdx(2, 4, 0, 0, 1);
            } catch(...) {
                threw = true;
            }

            REQUIRE( threw == true );
        }

        SECTION( "New group invalid" ) {
            try {
                pd.MoveByIdx(0, 0, 2, 4, 1);
            } catch(...) {
                threw = true;
            }

            REQUIRE( threw == true );
        }
    }

    SECTION( "Moves that should return false" ) {
        threw = false;

        try {
            pd.MoveByIdx(0, 0, 0, 0, -1);
        } catch(...) {
            threw = true;
        }

        // Moving negative num
        REQUIRE( threw == true );

        // Would result in -1 individuals in original group
        REQUIRE( pd.MoveByIdx(0, 0, 0, 1, 34) == false );
    }

    SECTION( "Moves that should work" ) {
        // Removing all individuals from a population group
        REQUIRE( pd.MoveByAge(0, 5, 0, 15, 33) == true );
        REQUIRE( pd.MoveByAge(0, 15, 0, 25, 99) == true );
    }
}
