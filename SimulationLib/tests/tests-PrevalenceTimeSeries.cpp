#include "catch.hpp"
#include "../PrevalenceTimeSeries.h"
#include "../DiscreteTimeStatistic.h"

using namespace std;
using namespace SimulationLib;

TEST_CASE("PrevalenceTimeSeries: initialization w/o statistics", "[statistics]") {
    PrevalenceTimeSeries<int> pts("Test", 10, 5);

    REQUIRE( (*pts.GetVector())[0] == 0 );
    REQUIRE( pts.GetCurrentPrevalence() == 0 );
    REQUIRE( pts.GetTime0() == 0 );
    REQUIRE( pts.GetName() == "Test" );
    REQUIRE( pts.IsWritable() == true );

    SECTION( "Closing the vector makes it unwritable" ) {
        pts.Close();

        REQUIRE( pts.IsWritable() == false );
    }
}

TEST_CASE("PrevalenceTimeSeries: initialization w statistics", "[statistics]") {
    DiscreteTimeStatistic *dts = new DiscreteTimeStatistic("Stats");
    PrevalenceTimeSeries<int> pts("Test", 10, 5, 1, dts);

    REQUIRE( (*pts.GetVector())[0] == 0 );
    REQUIRE( pts.GetCurrentPrevalence() == 0 );
    REQUIRE( pts.GetTime0() == 0 );
    REQUIRE( pts.GetName() == "Test" );
    REQUIRE( pts.IsWritable() == true );

    SECTION( "Closing the vector makes it unwritable" ) {
        pts.Close();

        REQUIRE( pts.IsWritable() == false );
    }

    delete dts;
}

TEST_CASE("PrevalenceTimeSeries: DTS on every entry", "[statistics]") {
    DiscreteTimeStatistic *dts = new DiscreteTimeStatistic("Stats");
    PrevalenceTimeSeries<int> pts("Test", 10, 5, RECORD_ON_ALL, dts);

    pts.Record(1, 1.0);

    REQUIRE( dts->GetSum() == 1 );
    REQUIRE( dts->GetCount() == 1 );
    REQUIRE( dts->GetMean() == 1 );
    REQUIRE( dts->GetVariance() == 0 );
    REQUIRE( dts->GetMin() == 1 );
    REQUIRE( dts->GetMax() == 1 );

    delete dts;
}
