#include "catch.hpp"
#include "utils/compare_files.h"
#include "../PrevalenceTimeSeries.h"

// REMEMBER: remove "-new" when replacing old implementation
#include "../CSVExport-new.h"

using namespace std;
using namespace SimulationLib;

TEST_CASE("Initialization", "[csv]") {
    PrevalenceTimeSeries<int> *pts;
    TimeSeriesCSVExport<int> tse{"test-out.csv"};

    pts = new PrevalenceTimeSeries<int>{"Test", 10, 5};

    pts->Record(0.0, 1);
    pts->Record(4.0, 2);
    pts->Record(7.0, 3);
    pts->Record(10.0, 4);

    tse.Add(pts);
    REQUIRE( tse.Write() == true );
}
