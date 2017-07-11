#include "catch.hpp"

// Provides "compare_files" function for comparing output of CSVExporter's
//   to reference output
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

}

TEST_CASE("Basic PyramidTimeSeries", "[csv]") {
    PrevalencePyramidTimeSeries *ptse1;
    PrevalencePyramidTimeSeries *ptse2;
    PyramidTimeSeriesCSVExport tse3{"test-t03-out.csv"};
    PyramidTimeSeriesCSVExport tse4{"test-t04-out.csv"};

    // Initialize
    ptse1 = new PrevalencePyramidTimeSeries{"Population 3", 0, 10, 2, 1, {5,10}};
    ptse2 = new PrevalencePyramidTimeSeries{"Population 4", 0, 30, 10, 2, {10,20,30}};

    // Add data
    // bool UpdateByAge(int time, int category, double age, int increment);

    //ptse1
    REQUIRE( ptse1->UpdateByAge(0,0,4,10) == true );
    REQUIRE( ptse1->UpdateByAge(0,0,8,12) == true );
    REQUIRE( ptse1->UpdateByAge(0,0,12,14) == true );

    REQUIRE( ptse1->UpdateByAge(1,0,4,9) == true );
    REQUIRE( ptse1->UpdateByAge(1,0,8,14) == true );
    REQUIRE( ptse1->UpdateByAge(1,0,12,17) == true );

    REQUIRE( ptse1->UpdateByAge(4,0,4,11) == true );
    REQUIRE( ptse1->UpdateByAge(4,0,8,11) == true );
    REQUIRE( ptse1->UpdateByAge(4,0,12,18) == true );

    //ptse2
    REQUIRE( ptse2->UpdateByAge(0,0,5,10) == true );
    REQUIRE( ptse2->UpdateByAge(0,0,15,10) == true );
    REQUIRE( ptse2->UpdateByAge(0,0,25,10) == true );
    REQUIRE( ptse2->UpdateByAge(0,0,35,10) == true );

    REQUIRE( ptse2->UpdateByAge(0,1,5,10) == true );
    REQUIRE( ptse2->UpdateByAge(0,1,15,10) == true );
    REQUIRE( ptse2->UpdateByAge(0,1,25,10) == true );
    REQUIRE( ptse2->UpdateByAge(0,1,35,10) == true );

    REQUIRE( ptse2->UpdateByAge(5,0,5,14) == true );
    REQUIRE( ptse2->UpdateByAge(5,0,15,11) == true );
    REQUIRE( ptse2->UpdateByAge(5,0,25,12) == true );
    REQUIRE( ptse2->UpdateByAge(5,0,35,13) == true );

    REQUIRE( ptse2->UpdateByAge(5,1,5,11) == true );
    REQUIRE( ptse2->UpdateByAge(5,1,15,8) == true );
    REQUIRE( ptse2->UpdateByAge(5,1,25,12) == true );
    REQUIRE( ptse2->UpdateByAge(5,1,35,10) == true );

    REQUIRE( ptse2->UpdateByAge(11,0,5,18) == true );
    REQUIRE( ptse2->UpdateByAge(11,0,15,14) == true );
    REQUIRE( ptse2->UpdateByAge(11,0,25,11) == true );
    REQUIRE( ptse2->UpdateByAge(11,0,35,16) == true );

    REQUIRE( ptse2->UpdateByAge(11,1,5,9) == true );
    REQUIRE( ptse2->UpdateByAge(11,1,15,7) == true );
    REQUIRE( ptse2->UpdateByAge(11,1,25,14) == true );
    REQUIRE( ptse2->UpdateByAge(11,1,35,11) == true );

    // Close TimeSeries
    ptse1->Close();
    ptse2->Close();

    // Add TimeSeries
    REQUIRE( tse3.Add(ptse1) == true );
    REQUIRE( tse4.Add(ptse2) == true );


    // Write TimeSeries
    REQUIRE( tse3.Write() == true );
    REQUIRE( tse4.Write() == true );

    REQUIRE( compare_files("test-t03-out.csv",
                           "tests-CSVExport-files/test-t03-answer.csv") == true );
    REQUIRE( compare_files("test-t04-out.csv",
                           "tests-CSVExport-files/test-t04-answer.csv") == true );


}
