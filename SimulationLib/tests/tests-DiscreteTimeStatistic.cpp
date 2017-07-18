#include <limits>
#include "catch.hpp"
#include "../include/SimulationLib/DiscreteTimeStatistic.h"

using namespace std;
using namespace SimulationLib;

TEST_CASE("DiscreteTimeStatistic: No observations", "[statistics]") {
    DiscreteTimeStatistic dts("Test Statistic");

    vector<double> emptyVec(0, 0);

    REQUIRE( dts.GetSum() == 0 );
    REQUIRE( dts.GetCount() == 0 );
    REQUIRE( dts.GetMean() == 0 );
    REQUIRE( dts.GetVariance() == 0 );
    REQUIRE( dts.GetMin() == numeric_limits<double>::max() );
    REQUIRE( dts.GetMax() == numeric_limits<double>::min() );
    REQUIRE( dts.GetObservations() == emptyVec );
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
}
