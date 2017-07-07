#include "catch.hpp"
#include "utils/compare_files.h"
#include "../PrevalenceTimeSeries.h"

// REMEMBER: remove "-new" when replacing old implementation
#include "../CSVExport.h"

using namespace std;
using namespace SimulationLib;

TEST_CASE("Basic PrevalenceTimeSeries", "[csv]") {
    PrevalenceTimeSeries<int> *pts1;
    PrevalenceTimeSeries<int> *pts2;
    TimeSeriesCSVExport<int> tse1{"test-t01-out.csv"};
    TimeSeriesCSVExport<int> tse2{"test-t02-out.csv"};

    // Initialize
    pts1 = new PrevalenceTimeSeries<int>{"Population 1", 10, 5};
    pts2 = new PrevalenceTimeSeries<int>{"Population 2", 5, 5};

    // Add data
    REQUIRE( pts1->Record(0.0, 1) == true );
    REQUIRE( pts1->Record(4.0, 2) == true );
    REQUIRE( pts1->Record(7.0, 3) == true );
    REQUIRE( pts1->Record(10.0, 4) == true );

    REQUIRE( pts2->Record(0.0, 150) == true );
    REQUIRE( pts2->Record(2.5, 101) == true );
    REQUIRE( pts2->Record(5.0, 100) == true );

    // Close TimeSeries
    pts1->Close();
    pts2->Close();

    // Add TimeSeries
    REQUIRE( tse1.Add(pts1) == true );

    REQUIRE( tse2.Add(pts1) == true );
    REQUIRE( tse2.Add(pts2) == true );

    // Write TimeSeries
    REQUIRE( tse1.Write() == true );
    REQUIRE( tse2.Write() == true );

    REQUIRE( compare_files("test-t01-out.csv",
                           "tests-CSVExport-files/test-t01-answer.csv") == true );

    REQUIRE( compare_files("test-t02-out.csv",
                           "tests-CSVExport-files/test-t02-answer.csv") == true );

    delete pts1;
    delete pts2;
}
