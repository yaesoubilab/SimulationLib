#include <limits>
#include "catch.hpp"
#include "../include/SimulationLib/DiscreteTimeStatistic.h"

using namespace std;
using namespace SimulationLib;

TEST_CASE("DiscreteTimeStatistic: No observations", "[statistics]") {
    DiscreteTimeStatistic dts("Test Statistic");

    REQUIRE( dts.GetSum() == 0 );
    REQUIRE( dts.GetCount() == 0 );
    REQUIRE( dts.GetMean() == 0 );
    REQUIRE( dts.GetVariance() == 0 );
    REQUIRE( dts.GetMin() == numeric_limits<double>::max() );
    REQUIRE( dts.GetMax() == numeric_limits<double>::min() );
}

TEST_CASE("DiscreteTimeStatistic: 1 observation", "[statistics]") {
    DiscreteTimeStatistic dts("Test Statistic");

    dts.Record(1.0);

    REQUIRE( dts.GetSum() == 1 );
    REQUIRE( dts.GetCount() == 1 );
    REQUIRE( dts.GetMean() == 1 );
    REQUIRE( dts.GetVariance() == 0 );
    REQUIRE( dts.GetMin() == 1 );
    REQUIRE( dts.GetMax() == 1 );
}

TEST_CASE("DiscreteTimeStatistic: 2 observations", "[statistic]") {
    DiscreteTimeStatistic dts("Tests Statistic");

    dts.Record(1.0);
    dts.Record(-1.0);

    REQUIRE( dts.GetSum() == 0 );
    REQUIRE( dts.GetCount() == 2 );
    REQUIRE( dts.GetMean() == 0 );
    REQUIRE( dts.GetVariance() == 2 );
    REQUIRE( dts.GetMin() == -1 );
    REQUIRE( dts.GetMax() == 1 );
}

TEST_CASE("DiscreteTimeStatistic: 5 observations", "[statistic]") {
    DiscreteTimeStatistic dts("Tests Statistic");

    dts.Record(1.0);
    dts.Record(-1.0);
    dts.Record(2.0);
    dts.Record(3.0);
    dts.Record(1.0);

    REQUIRE( dts.GetSum() == 6 );
    REQUIRE( dts.GetCount() == 5 );
    REQUIRE( dts.GetMean() == 1.2 );
    REQUIRE( dts.GetVariance() == 2.2 );
    REQUIRE( dts.GetMin() == -1 );
    REQUIRE( dts.GetMax() == 3 );
    REQUIRE( dts.GetPercentile(1.0) == 3.0);
    REQUIRE( dts.GetPercentile(0.5) == 1.0);
}

TEST_CASE("DiscreteTimeStatistic: Percentiles easy", "[statistic]") {
    DiscreteTimeStatistic dts("Tests Statistic");

    for (int i = 1; i <= 100; ++i) {
        dts.Record(i);
    }
    
    REQUIRE (dts.GetPercentile(0.01) == 1.0);
    REQUIRE (dts.GetPercentile(0.02) == 2.0);
    REQUIRE (dts.GetPercentile(1.0) == 100);
}

TEST_CASE("DiscreteTimeStatistic: Percentiles harder", "[statistic]") {
    DiscreteTimeStatistic dts("Tests Statistic");

    dts.Record(1.0);
    dts.Record(-1.0);
    dts.Record(2.0);
    dts.Record(3.0);
    dts.Record(-2.0);
    
    REQUIRE (dts.GetPercentile(0.01) == -2.0);
    REQUIRE (dts.GetPercentile(0.02) == -2.0);
    REQUIRE (dts.GetPercentile(0.20) == -2.0);
    REQUIRE (dts.GetPercentile(0.21) == -1.0);
    REQUIRE (dts.GetPercentile(0.22) == -1.0);
    REQUIRE (dts.GetPercentile(0.40) == -1.0);
    REQUIRE (dts.GetPercentile(0.41) == 1.0);
    REQUIRE (dts.GetPercentile(0.42) == 1.0);
    REQUIRE (dts.GetPercentile(0.60) == 1.0);
    REQUIRE (dts.GetPercentile(0.61) == 2.0);
    REQUIRE (dts.GetPercentile(0.62) == 2.0);
    REQUIRE (dts.GetPercentile(0.80) == 2.0);
    REQUIRE (dts.GetPercentile(0.81) == 3.0);
    REQUIRE (dts.GetPercentile(0.82) == 3.0);
    REQUIRE (dts.GetPercentile(1.00) == 3.0);
}

TEST_CASE("DiscreteTimeStatistic: Percentiles out of range", "[statistic]") {
    bool threw = false;

    DiscreteTimeStatistic dts("Tests Statistic");

    for (int i = 1; i <= 100; ++i) {
        dts.Record(i);
    }

    SECTION( "Alpha = 0" ) {
        try {
            dts.GetPercentile(0);

        } catch (...) {
            threw = true;
        }

        REQUIRE( threw == true );
    }

    SECTION( "Alpha < 0" ) {
        try {
            dts.GetPercentile(-0.01);

        } catch (...) {
            threw = true;
        }

        REQUIRE( threw == true );
    }

    SECTION( "Alpha > 1" ) {
        try {
            dts.GetPercentile(1.01);

        } catch (...) {
            threw = true;
        }

        REQUIRE( threw == true );
    }
}
