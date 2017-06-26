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
        REQUIRE( pts.Record(0.0, 1) == false );
    }

    SECTION( "Vector is writable by default" ) {
        REQUIRE( pts.IsWritable() == true );
        REQUIRE( pts.Record(0.0, 1) == true );
    }

    SECTION( "time > timeMax fails" ) {
        REQUIRE( pts.Record(10.01, 1) == false);
    }

    SECTION( "time = timeMax succeeds" ) {
        REQUIRE( pts.Record(10.0, 1) == true );
    }

    SECTION( "successively decreasing times fail" ) {
        pts.Record(1.0, 1);
        REQUIRE( pts.Record(0.99, 1) == false );
    }

    SECTION( "monotonically non-decreasing times succeed" ) {
        pts.Record(0.0, 1);
        REQUIRE( pts.Record(0.0, 2) == true );
    }

    SECTION( "Negative time fails" ) {
        REQUIRE( pts.Record(-0.01, 1) == false );
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

    pts.Record(1, -1.0);
    pts.Record(1.1, 2.0);
    pts.Record(2, 3.0);
    pts.Record(3, 1.0);

    // population is: 1, 0, 2, 5, 6
    REQUIRE( dts->GetSum() == 14 );
    REQUIRE( dts->GetCount() == 5 );
    REQUIRE( dts->GetMean() == 2.8 );
    REQUIRE( dts->GetVariance() == 6.7 );
    REQUIRE( dts->GetMin() == 0 );
    REQUIRE( dts->GetMax() == 6 );

    delete dts;
}

TEST_CASE("PrevalenceTimeSeries: DTS on each period", "[statistics]") {
    DiscreteTimeStatistic *dts = new DiscreteTimeStatistic("Stats");

    // maxtime=10, periodlength=5, record on every period
    PrevalenceTimeSeries<int> pts("Test", 10, 5, 1, dts);

    SECTION("easy") {
        pts.Record(0.0, 1);
        pts.Record(3.5, 1);
        pts.Record(7, 1);
        pts.Close();

        REQUIRE( dts->GetSum() == 6 );
        REQUIRE( dts->GetCount() == 3 );
        REQUIRE( dts->GetMean() == 2 );
        REQUIRE( dts->GetVariance() == 1 );
        REQUIRE( dts->GetMin() == 1 );
        REQUIRE( dts->GetMax() == 3 );
    }


    delete dts;
}
