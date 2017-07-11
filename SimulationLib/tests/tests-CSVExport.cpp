#include "catch.hpp"

// Provides "compare_files" function for comparing output of CSVExporter's
//   to reference output
#include <map>
#include <string>
#include "utils/compare_files.h"
#include "../PrevalenceTimeSeries.h"
#include "../PrevalencePyramidTimeSeries.h"

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

TEST_CASE("Basic PyramidTimeSeries", "[csv]") {
    PrevalencePyramidTimeSeries *pts1;
    PyramidTimeSeriesCSVExport ptse1{"test-t03-out.csv"};

    // Initialize
    pts1 = new PrevalencePyramidTimeSeries{"Population 3", 0, 10,              \
                                 2, 1,                \
                                 {5,10}};

    // Add data
    // bool UpdateByAge(int time, int category, double age, int increment);

    REQUIRE( pts1->UpdateByAge(0,0,4,10) == true );
    REQUIRE( pts1->UpdateByAge(0,0,8,12) == true );
    REQUIRE( pts1->UpdateByAge(0,0,12,14) == true );

    REQUIRE( pts1->UpdateByAge(1,0,4,9) == true );
    REQUIRE( pts1->UpdateByAge(1,0,8,14) == true );
    REQUIRE( pts1->UpdateByAge(1,0,12,17) == true );

    REQUIRE( pts1->UpdateByAge(4,0,4,11) == true );
    REQUIRE( pts1->UpdateByAge(4,0,8,11) == true );
    REQUIRE( pts1->UpdateByAge(4,0,12,18) == true );

    // Close TimeSeries
    pts1->Close();

    // Add TimeSeries
    REQUIRE( ptse1.Add(pts1) == true );


    // Write TimeSeries
    REQUIRE( ptse1.Write() == true );

    REQUIRE( compare_files("test-t03-out.csv",
                           "tests-CSVExport-files/test-t03-answer.csv") == true );


}

TEST_CASE("Basic TimeStatistic export", "[csv]") {
    map<TimeStatType, string> columns {
        {TimeStatType::Mean, "Average"}
    };
    DiscreteTimeStatistic dts = new DiscreteTimeStatistic("Statistics!");
    TimeStatisticCSVExporter tse("test-tstats01-out.csv", columns);

    dts->Record(1);
    dts->Record(2);
    dts->Record(3);

    REQUIRE( tse.Add(dts) == true );
    REQUIRE( tse.Write()  == true );
    REQUIRE( compare_files("test-tstats01-out.csv",
                           "tests-CSVExport-files/test-tstats01-answer.csv") == true );
}
