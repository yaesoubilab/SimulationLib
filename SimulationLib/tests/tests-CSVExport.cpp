#include "catch.hpp"

// Provides "compare_files" function for comparing output of CSVExporter's
//   to reference output
#include <map>
#include <string>
#include "utils/compare_files.h"
#include "../include/SimulationLib/PrevalenceTimeSeries.h"
#include "../include/SimulationLib/PrevalencePyramidTimeSeries.h"
#include "../include/SimulationLib/IncidencePyramidTimeSeries.h"
#include "../include/SimulationLib/DiscreteTimeStatistic.h"

#include "../include/SimulationLib/CSVExport.h"

using namespace std;
using namespace SimulationLib;

TEST_CASE("Basic PrevalenceTimeSeries", "[csv]") {
    PrevalenceTimeSeries<int> *pts1;
    PrevalenceTimeSeries<int> *pts2;
    TimeSeriesExport<int> tse1{"test-t01-out.csv"};
    TimeSeriesExport<int> tse2{"test-t02-out.csv"};

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
    PrevalencePyramidTimeSeries *ptse1;
    PrevalencePyramidTimeSeries *ptse2;
    PyramidTimeSeriesExport tse3{"test-t03-out.csv"};
    PyramidTimeSeriesExport tse4{"test-t04-out.csv"};

    // Initialize
    ptse1 = new PrevalencePyramidTimeSeries{"Population 3", 0, 10, 2, 1, {5,10}};
    ptse2 = new PrevalencePyramidTimeSeries{"Population 4", 0, 30, 10, 2, {10,20,30}};

    // Add data
    // bool UpdateByAge(int time, int category, double age, int increment);

    //ptse1
    REQUIRE( ptse1->UpdateByAge(0,0,4,10) == true );
    REQUIRE( ptse1->UpdateByAge(0,0,8,12) == true );
    REQUIRE( ptse1->UpdateByAge(0,0,12,14) == true );

    REQUIRE( ptse1->UpdateByAge(1,0,4,-1) == true );
    REQUIRE( ptse1->UpdateByAge(1,0,8,+2) == true );
    REQUIRE( ptse1->UpdateByAge(1,0,12,+3) == true );

    REQUIRE( ptse1->UpdateByAge(4,0,4,+2) == true );
    REQUIRE( ptse1->UpdateByAge(4,0,8,-3) == true );
    REQUIRE( ptse1->UpdateByAge(4,0,12,+1) == true );

    REQUIRE( ptse1->UpdateByAge(6,0,4,0) == true );
    REQUIRE( ptse1->UpdateByAge(6,0,8,0) == true );
    REQUIRE( ptse1->UpdateByAge(6,0,12,0) == true );

    //ptse2
    REQUIRE( ptse2->UpdateByAge(0,0,5,10) == true );
    REQUIRE( ptse2->UpdateByAge(0,0,15,10) == true );
    REQUIRE( ptse2->UpdateByAge(0,0,25,10) == true );
    REQUIRE( ptse2->UpdateByAge(0,0,35,10) == true );

    REQUIRE( ptse2->UpdateByAge(0,1,5,10) == true );
    REQUIRE( ptse2->UpdateByAge(0,1,15,10) == true );
    REQUIRE( ptse2->UpdateByAge(0,1,25,10) == true );
    REQUIRE( ptse2->UpdateByAge(0,1,35,10) == true );

    REQUIRE( ptse2->UpdateByAge(5,0,5,+4) == true );
    REQUIRE( ptse2->UpdateByAge(5,0,15,+1) == true );
    REQUIRE( ptse2->UpdateByAge(5,0,25,+2) == true );
    REQUIRE( ptse2->UpdateByAge(5,0,35,+3) == true );

    REQUIRE( ptse2->UpdateByAge(5,1,5,+1) == true );
    REQUIRE( ptse2->UpdateByAge(5,1,15,-2) == true );
    REQUIRE( ptse2->UpdateByAge(5,1,25,+2) == true );
    REQUIRE( ptse2->UpdateByAge(5,1,35,0) == true );

    REQUIRE( ptse2->UpdateByAge(11,0,5,+4) == true );
    REQUIRE( ptse2->UpdateByAge(11,0,15,+3) == true );
    REQUIRE( ptse2->UpdateByAge(11,0,25,-1) == true );
    REQUIRE( ptse2->UpdateByAge(11,0,35,+3) == true );

    REQUIRE( ptse2->UpdateByAge(11,1,5,-2) == true );
    REQUIRE( ptse2->UpdateByAge(11,1,15,-1) == true );
    REQUIRE( ptse2->UpdateByAge(11,1,25,+2) == true );
    REQUIRE( ptse2->UpdateByAge(11,1,35,+1) == true );

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

TEST_CASE("Basic PyramidData", "[csv]") {
    PyramidData<int> *pd;
    PyramidDataExport<int> pde{"test-t09-out.csv"};

    // Initialize
    pd = new PyramidData<int>{2, {}};
    pd->UpdateByAge(0, 0, 1);
    pd->UpdateByAge(1, 100, 2);

    REQUIRE( pde.Add(pd) == true );

    // Write PyramidData
    REQUIRE( pde.Write() == true );

    REQUIRE( compare_files("test-t09-out.csv",
                           "tests-CSVExport-files/test-t09-answer.csv") == true );


}

TEST_CASE("More complicated PyramidData", "[csv]") {
    PyramidData<int> *pd;
    PyramidDataExport<int> pde{"test-t10-out.csv"};

    // Initialize
    pd = new PyramidData<int>{2, {10, 20, 30}};
    pd->UpdateByAge(0, 15, 1);
    pd->UpdateByAge(1, 0, 2);
    pd->UpdateByAge(0, 100, 150);

    REQUIRE( pde.Add(pd) == true );

    // Write PyramidData
    REQUIRE( pde.Write() == true );

    REQUIRE( compare_files("test-t10-out.csv",
                           "tests-CSVExport-files/test-t10-answer.csv") == true );
}

TEST_CASE("More complicated PyramidData with multiple PDs", "[csv]") {
    PyramidData<int> *pd1;
    PyramidData<int> *pd2;
    PyramidDataExport<int> pde{"test-t12-out.csv"};

    // Initialize
    pd1 = new PyramidData<int>{2, {10, 20, 30}};
    pd2 = new PyramidData<int>{2, {10, 20, 30}};

    pd1->UpdateByAge(0, 15, 1);
    pd1->UpdateByAge(1, 0, 2);
    pd1->UpdateByAge(0, 100, 150);

    pd2->UpdateByAge(0, 15, 1);
    pd2->UpdateByAge(1, 0, 2);
    pd2->UpdateByAge(0, 100, 150);

    REQUIRE( pde.Add(pd1) == true );
    REQUIRE( pde.Add(pd2) == true );

    // Write PyramidData
    REQUIRE( pde.Write() == true );

    REQUIRE( compare_files("test-t12-out.csv",
                           "tests-CSVExport-files/test-t12-answer.csv") == true );
}

TEST_CASE("PyramidData<double>", "[csv]") {
    PyramidData<double> *pd;
    PyramidDataExport<double> pde{"test-t11-out.csv"};

    // Initialize
    pd = new PyramidData<double>{2, {10, 20, 30}};
    pd->UpdateByAge(0, 15, 1.5);
    pd->UpdateByAge(1, 0, 2.001);
    pd->UpdateByAge(0, 100, 150.001);

    REQUIRE( pde.Add(pd) == true );

    // Write PyramidData
    REQUIRE( pde.Write() == true );

    REQUIRE( compare_files("test-t11-out.csv",
                           "tests-CSVExport-files/test-t11-answer.csv") == true );
}

TEST_CASE("PrevalenceTimeSeries intermediate values should not be zeroed", "[csv]") {
    PrevalenceTimeSeries<int> *pts5;
    PrevalenceTimeSeries<int> *pts6;
    TimeSeriesExport<int> tse5{"test-t05-out.csv"};
    TimeSeriesExport<int> tse6{"test-t06-out.csv"};

    // Initialize
    pts5 = new PrevalenceTimeSeries<int>{"Population 5", 10, 2};
    pts6 = new PrevalenceTimeSeries<int>{"Population 6", 10, 2};

    // Add data
    REQUIRE( pts5->Record(0.0, 100) == true );
    REQUIRE( pts5->Record(9.0, 2) == true );

    REQUIRE( pts6->Record(0.0, 100) == true );
    REQUIRE( pts6->Record(1.0, 1) == true );
    REQUIRE( pts6->Record(7.0, 2) == true );
    REQUIRE( pts6->Record(9.0, 3) == true );

    // Close TimeSeries
    pts5->Close();
    pts6->Close();

    // Add TimeSeries
    REQUIRE( tse5.Add(pts5) == true );
    REQUIRE( tse6.Add(pts6) == true );

    // Write TimeSeries
    REQUIRE( tse5.Write() == true );
    REQUIRE( tse6.Write() == true );

    REQUIRE( compare_files("test-t05-out.csv",
                           "tests-CSVExport-files/test-t05-answer.csv") == true );
    REQUIRE( compare_files("test-t06-out.csv",
                           "tests-CSVExport-files/test-t06-answer.csv") == true );

    delete pts5;
}

TEST_CASE("PyramidTimeSeries with intermediate zeroes", "[csv]") {
    PrevalencePyramidTimeSeries *ppts1;
    IncidencePyramidTimeSeries *ipts1;
    PyramidTimeSeriesExport tse7{"test-t07-out.csv"};
    PyramidTimeSeriesExport tse8{"test-t08-out.csv"};

    // Initialize
    ppts1 = new PrevalencePyramidTimeSeries{"Population 7", 0, 10, 2, 2, {10,20,30}};
    ipts1 = new IncidencePyramidTimeSeries{"Population 8", 0, 10, 2, 2, {10,20,30}};

    ppts1->UpdateByAge(0, 0, 5, 10);
    ppts1->UpdateByAge(0, 0, 15, 20);
    ppts1->UpdateByAge(0, 0, 25, 30);
    ppts1->UpdateByAge(0, 0, 35, 40);
    ppts1->UpdateByAge(0, 1, 5, 15);
    ppts1->UpdateByAge(0, 1, 15, 25);
    ppts1->UpdateByAge(0, 1, 25, 35);
    ppts1->UpdateByAge(0, 1, 35, 45);

    ppts1->UpdateByAge(7, 0, 5, 1);
    ppts1->UpdateByAge(7, 0, 15, 2);
    ppts1->UpdateByAge(7, 0, 25, 3);
    ppts1->UpdateByAge(7, 0, 35, 4);
    ppts1->UpdateByAge(7, 1, 5, 1);
    ppts1->UpdateByAge(7, 1, 15, 2);
    ppts1->UpdateByAge(7, 1, 25, 3);
    ppts1->UpdateByAge(7, 1, 35, 4);


    ipts1->UpdateByAge(0, 0, 5, 10);
    ipts1->UpdateByAge(0, 0, 15, 20);
    ipts1->UpdateByAge(0, 0, 25, 30);
    ipts1->UpdateByAge(0, 0, 35, 40);
    ipts1->UpdateByAge(0, 1, 5, 15);
    ipts1->UpdateByAge(0, 1, 15, 25);
    ipts1->UpdateByAge(0, 1, 25, 35);
    ipts1->UpdateByAge(0, 1, 35, 45);

    ipts1->UpdateByAge(7, 0, 5, 1);
    ipts1->UpdateByAge(7, 0, 15, 2);
    ipts1->UpdateByAge(7, 0, 25, 3);
    ipts1->UpdateByAge(7, 0, 35, 4);
    ipts1->UpdateByAge(7, 1, 5, 1);
    ipts1->UpdateByAge(7, 1, 15, 2);
    ipts1->UpdateByAge(7, 1, 25, 3);
    ipts1->UpdateByAge(7, 1, 35, 4);


    // Close TimeSeries
    ppts1->Close();
    ipts1->Close();

    // Add TimeSeries
    REQUIRE( tse7.Add(ppts1) == true );
    REQUIRE( tse8.Add(ipts1) == true );


    // Write TimeSeries
    REQUIRE( tse7.Write() == true );
    REQUIRE( tse8.Write() == true );

    REQUIRE( compare_files("test-t07-out.csv",
                           "tests-CSVExport-files/test-t07-answer.csv") == true );
    REQUIRE( compare_files("test-t08-out.csv",
                           "tests-CSVExport-files/test-t08-answer.csv") == true );


}



TEST_CASE("Basic TimeStatistic export", "[csv]") {
    map<TimeStatType, string> columns {
        {TimeStatType::Mean, "Average"}
    };
    DiscreteTimeStatistic *dts = new DiscreteTimeStatistic("Statistics!");
    TimeStatisticsExport tse("test-tstats01-out.csv", columns);

    dts->Record(1);
    dts->Record(2);
    dts->Record(3);

    REQUIRE( tse.Add(dts) == true );
    REQUIRE( tse.Write()  == true );
    REQUIRE( compare_files("test-tstats01-out.csv",
                           "tests-CSVExport-files/test-tstats01-answer.csv") == true );
}
