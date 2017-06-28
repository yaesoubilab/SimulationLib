#include "catch.hpp"
#include "../PrevalencePyramidTimeSeries.h"
#include "../IncidencePyramidTimeSeries.h"

using namespace std;
using namespace SimulationLib;

// Initializations:
// - time0/timeMax mismatch
// - time0 = timeMax (throws b/c periodLength > 0)
// - time0 < 0
// - periodLength < timeMax-time0
// - periodLength = timeMax-time0
// - ageBreaks{}
// - nCategories = 0
// - nCategories < 0
TEST_CASE( "Initializations" ) {
    // Because PrevalencePyramidTimeSeries and IncidencePyramidTimeSeries both
    //   are instantiated by the same function, for now we will only test
    //   bad initializations on PrevalencePyramidTimeSeries.
    bool threw = false;

    SECTION( "time0/timeMax mismatch" ) {
        vector<double> ageBreaks = {10};

        try {
            // timeMax < time0)
            PrevalencePyramidTimeSeries ppts \
              ("Test", 5, 4, 1, 1, ageBreaks);

        } catch (...) {
            threw = true;
        }

        REQUIRE( threw == true );
    }

    SECTION( "time0 = timeMax" ) {
        vector<double> ageBreaks = {10};

        try {
            PrevalencePyramidTimeSeries ppts \
              ("Test", 0, 0, 0, 1, ageBreaks);
        } catch (...) {
            threw = true;
        }

        REQUIRE( threw == true );
    }

    SECTION( "time0 < 0" ) {
        vector<double> ageBreaks = {10};

        try {
            PrevalencePyramidTimeSeries ppts \
              ("Test", -1, 2, 2, 1, ageBreaks);
        } catch (...) {
            threw = true;
        }

        REQUIRE( threw == true );
    }

    SECTION( "periodLength < timeMax-time0" ) {
        vector<double> ageBreaks = {10};

        try {
            PrevalencePyramidTimeSeries ppts \
              ("Test", 0, 3, 4, 1, ageBreaks);
        } catch (...) {
            threw = true;
        }

        REQUIRE( threw == true );
    }

    SECTION( "periodLength = timeMax-time0" ) {
        vector<double> ageBreaks = {10};

        try {
            PrevalencePyramidTimeSeries ppts \
              ("Test", 0, 3, 3, 1, ageBreaks);
        } catch (...) {
            threw = true;
        }

        REQUIRE( threw == false );
    }

    SECTION( "ageBreaks{}" ) {
        vector<double> ageBreaks = {};

        try {
            PrevalencePyramidTimeSeries ppts \
              ("Test", 0, 3, 1, 1, ageBreaks);
        } catch (...) {
            threw = true;
        }

        REQUIRE( threw == false );
    }

    SECTION( "nCategories = 0" ) {
        vector<double> ageBreaks = {10};

        try {
            PrevalencePyramidTimeSeries ppts \
              ("Test", 0, 3, 1, 0, ageBreaks);
        } catch (...) {
            threw = true;
        }

        REQUIRE( threw == true );
    }

    SECTION( "nCategories < 0" ) {
        vector<double> ageBreaks = {10};

        try {
            PrevalencePyramidTimeSeries ppts \
              ("Test", 0, 3, 1, -1, ageBreaks);
        } catch (...) {
            threw = true;
        }

        REQUIRE( threw == true );
    }
}

// Behavior:
// (Legal)
// - One age group, one category: adding stuff and checking it
// - One age group, two categories: things getting separated right
// - Borders: Adding things onto borders (0, and on an agebreak)
// - Moving things into the population they came from
// - Moving things such that the old group is brought to 0
// - Updating to bring to zero
TEST_CASE( "Legal behavior" ) {

    SECTION( "One age group, one category: adding stuff and checking it" ) {
        vector<double> ageBreaks = {};

        PrevalencePyramidTimeSeries ppts \
          ("Test", 0, 10, 2, 1, ageBreaks);

        IncidencePyramidTimeSeries ipts\
          ("Test", 0, 10, 2, 1, ageBreaks);

        ppts.UpdateByAge(0, 0, 100, 1);
        ipts.UpdateByAge(0, 0, 100, 1);
        ipts.UpdateByIdx(0, 0, 0, 1);
        ppts.UpdateByIdx(0, 0, 0, 1);

        REQUIRE( ppts.GetTotalAtPeriod(0) == 2 );
        REQUIRE( ipts.GetTotalAtPeriod(0) == 2 );
        REQUIRE( ppts.GetTotalInAgeGroupAtPeriod(0, 0) == 2 );
        REQUIRE( ipts.GetTotalInAgeGroupAtPeriod(0, 0) == 2 );
    }

    SECTION( "One age group, two categories: things getting separated right" ) {
        vector<double> ageBreaks = {};

        PrevalencePyramidTimeSeries ppts \
          ("Test", 0, 10, 5, 2, ageBreaks);

        IncidencePyramidTimeSeries ipts\
          ("Test", 0, 10, 5, 2, ageBreaks);

        ppts.UpdateByAge(0, 0, 100, 17);
        ipts.UpdateByAge(0, 0, 100, 17);
        ppts.UpdateByAge(0, 1, 100, 18);
        ipts.UpdateByAge(0, 1, 100, 18);

        REQUIRE( ppts.GetTotalInCategoryAtPeriod(0, 0) == 17);
        REQUIRE( ipts.GetTotalInCategoryAtPeriod(0, 0) == 17);
        REQUIRE( ppts.GetTotalInCategoryAtPeriod(0, 1) == 18);
        REQUIRE( ipts.GetTotalInCategoryAtPeriod(0, 1) == 18);
    }

    SECTION( "Borders: Adding things onto borders of age breaks" ) {
        vector<double> ageBreaks = {5};

        PrevalencePyramidTimeSeries ppts \
          ("Test", 0, 10, 2, 1, ageBreaks);

        IncidencePyramidTimeSeries ipts\
          ("Test", 0, 10, 2, 1, ageBreaks);

        ppts.UpdateByAge(0, 0, 5, 1);
        ipts.UpdateByAge(0, 0, 5, 1);

        REQUIRE(ppts.GetTotalInAgeGroupAtPeriod(0, 1) == 1);
        REQUIRE(ipts.GetTotalInAgeGroupAtPeriod(0, 1) == 1);
        REQUIRE(ppts.GetTotalInAgeGroupAtPeriod(0, 0) == 0);
        REQUIRE(ipts.GetTotalInAgeGroupAtPeriod(0, 0) == 0);
    }

    // PrevalencePyramidTimeSeries should show x individuals at period0
    // IncidencePyramidTimeSeries should show x + y individuals at period0
    SECTION( "Borders: Adding things onto time=0" ) {
        vector<double> ageBreaks = {};
        int x = 13;
        int y = 7;

        PrevalencePyramidTimeSeries ppts \
          ("Test", 0, 10, 2, 1, ageBreaks);

        IncidencePyramidTimeSeries ipts\
          ("Test", 0, 10, 2, 1, ageBreaks);

        ppts.UpdateByAge(0, 0, 5, x); // x individuals at time=0
        ipts.UpdateByAge(0, 0, 5, x);
        ppts.UpdateByAge(1, 0, 5, y); // y individuals at time=1
        ipts.UpdateByAge(1, 0, 5, y);

        REQUIRE(ppts.GetTotalAtPeriod(0) == x);
        REQUIRE(ipts.GetTotalAtPeriod(0) == x + y);
    }

    SECTION( "Moving things into the population they came from" ) {
        vector<double> ageBreaks = {};

        PrevalencePyramidTimeSeries ppts \
          ("Test", 0, 10, 2, 1, ageBreaks);

        IncidencePyramidTimeSeries ipts\
          ("Test", 0, 10, 2, 1, ageBreaks);

        ppts.UpdateByAge(0, 0, 100, 5);
        ipts.UpdateByAge(0, 0, 100, 5);
        ppts.MoveByAge(0, 0, 100, 0, 99, 4);
        ipts.MoveByAge(0, 0, 100, 0, 99, 4);

        REQUIRE(ppts.GetTotalInAgeGroupAtPeriod(0, 0) == 5);
        REQUIRE(ipts.GetTotalInAgeGroupAtPeriod(0, 0) == 5);
    }

    SECTION( "Moving things such that the old group is brought to 0" ) {
        vector<double> ageBreaks = {5};

        PrevalencePyramidTimeSeries ppts \
          ("Test", 0, 10, 2, 2, ageBreaks);

        IncidencePyramidTimeSeries ipts\
          ("Test", 0, 10, 2, 2, ageBreaks);

        ppts.UpdateByAge(0, 0, 100, 13);
        ipts.UpdateByAge(0, 0, 100, 13);
        ppts.MoveByAge(0, 0, 100, 1, 100, 13);
        ipts.MoveByAge(0, 0, 100, 1, 100, 13);

        REQUIRE(ppts.GetTotalInCategoryAtPeriod(0, 0) == 0);
        REQUIRE(ipts.GetTotalInCategoryAtPeriod(0, 0) == 0);
        REQUIRE(ppts.GetTotalInCategoryAtPeriod(0, 1) == 13);
        REQUIRE(ipts.GetTotalInCategoryAtPeriod(0, 1) == 13);
    }

    SECTION( "Updating to bring to zero" ) {
        vector<double> ageBreaks = {};

        PrevalencePyramidTimeSeries ppts \
          ("Test", 0, 10, 2, 1, ageBreaks);

        IncidencePyramidTimeSeries ipts\
          ("Test", 0, 10, 2, 1, ageBreaks);

        ppts.UpdateByAge(0, 0, 100, 13);
        ipts.UpdateByAge(0, 0, 100, 13);
        ppts.UpdateByAge(0, 0, 100, -13);
        ipts.UpdateByAge(0, 0, 100, -13);

        REQUIRE(ppts.GetTotalInAgeGroupAtPeriod(0, 0) == 0);
        REQUIRE(ipts.GetTotalInAgeGroupAtPeriod(0, 0) == 0);
    }
}

// Illegal:
// - Out of bounds age group updating
// - Out of bounds category updating
// - Out of bounds moving (just write 1 test and call it good)
// - Updating to bring below zero
// - Moving to bring old group below zero
// - Trying to move negative numbers of individuals
TEST_CASE("Illegal behavior") {
    bool threwP = false;
    bool threwI = false;

    SECTION( "Out of bounds age group updating" ) {
        vector<double> ageBreaks = {5};

        PrevalencePyramidTimeSeries ppts \
          ("Test", 0, 10, 2, 1, ageBreaks);

        IncidencePyramidTimeSeries ipts \
          ("Test", 0, 10, 2, 1, ageBreaks);

        try {
            ppts.UpdateByIdx(0, 0, 2, 1);
        } catch (...) { threwP = true; }

        try {
            ipts.UpdateByIdx(0, 0, 2, 1);
        } catch (...) { threwI = true; }

        REQUIRE(threwP == true);
        REQUIRE(threwI == true);
    }

    SECTION( "Out of bounds category updating" ) {
        vector<double> ageBreaks = {};

        PrevalencePyramidTimeSeries ppts \
          ("Test", 0, 10, 2, 3, ageBreaks);

        IncidencePyramidTimeSeries ipts \
          ("Test", 0, 10, 2, 3, ageBreaks);

        try {
            ppts.UpdateByAge(0, 3, 100, 1);
        } catch (...) { threwP = true; }

        try {
            ipts.UpdateByAge(0, 3, 100, 1);
        } catch (...) { threwI = true; }

        REQUIRE(threwP == true);
        REQUIRE(threwI == true);
    }

    SECTION( "Out of bounds moving (just write 1 test and call it good)" ) {
        vector<double> ageBreaks = {10};

        PrevalencePyramidTimeSeries ppts \
          ("Test", 0, 10, 2, 2, ageBreaks);

        IncidencePyramidTimeSeries ipts \
          ("Test", 0, 10, 2, 2, ageBreaks);

        ppts.UpdateByAge(0, 1, 100, 1);
        ipts.UpdateByAge(0, 1, 100, 1);

        try {
            ppts.MoveByIdx(0, 1, 100, 2, 2, 1);
        } catch (...) { threwP = true; }

        try {
            ipts.MoveByIdx(0, 1, 100, 2, 2, 1);
        } catch (...) { threwI = true; }

        REQUIRE(threwP == true);
        REQUIRE(threwI == true);
    }

    SECTION( "Updating to bring below zero" ) {
        vector<double> ageBreaks = {};

        PrevalencePyramidTimeSeries ppts \
          ("Test", 0, 10, 2, 1, ageBreaks);

        IncidencePyramidTimeSeries ipts \
          ("Test", 0, 10, 2, 1, ageBreaks);

        ppts.UpdateByAge(0, 0, 100, 13);
        ipts.UpdateByAge(0, 0, 100, 13);

        REQUIRE( ppts.UpdateByAge(0, 0, 100, -14) == false );
        REQUIRE( ipts.UpdateByAge(0, 0, 100, -14) == false );
    }

    SECTION( "Moving to bring old group below zero" ) {
        vector<double> ageBreaks = {10};

        PrevalencePyramidTimeSeries ppts \
          ("Test", 0, 10, 2, 2, ageBreaks);

        IncidencePyramidTimeSeries ipts \
          ("Test", 0, 10, 2, 2, ageBreaks);

        ppts.UpdateByAge(0, 0, 100, 13);
        ipts.UpdateByAge(0, 0, 100, 13);

        REQUIRE( ppts.MoveByAge(0, 0, 100, 1, 100, 14) == false );
        REQUIRE( ipts.MoveByAge(0, 0, 100, 1, 100, 14) == false );
    }

    SECTION( "Trying to move negative numbers of individuals" ) {
        vector<double> ageBreaks = {};

        PrevalencePyramidTimeSeries ppts \
          ("Test", 0, 10, 2, 2, ageBreaks);

        IncidencePyramidTimeSeries ipts \
          ("Test", 0, 10, 2, 2, ageBreaks);

        ppts.UpdateByAge(0, 0, 100, 13);
        ipts.UpdateByAge(0, 0, 100, 13);

        try {
            ppts.MoveByAge(0, 0, 100, 1, 100, -12);
        } catch (...) { threwP = true; }

        try {
            ipts.MoveByAge(0, 0, 100, 1, 100, -12);
        } catch (...) { threwI = true; }

        REQUIRE(threwP == true);
        REQUIRE(threwI == true);
    }

}
