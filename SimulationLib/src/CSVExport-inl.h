#include "../include/SimulationLib/CSVExport.h"

namespace SimulationLib {

/////////////////////////////////////////
// General CSV Exporter
/////////////////////////////////////////
template <typename T>
CSVExport<T>::CSVExport(string _fname) {
    fname = _fname;
}

// Empty destructor, for now.
template <typename T>
CSVExport<T>::~CSVExport() {}

template <typename T>
bool
CSVExport<T>::Write(void) {
    int stringSize;                    // Size of the string to be written to the file buffer
    string buf;                        // Output buffer
    string comma, newline;             // Strings for commonly used symbols
    FILE *fs;                          // File buffer
    CellSpecItrs columnItrs, rowItrs;  // Iterators against rows and columns

    buf        = string("");
    comma      = string(",");
    newline    = string("\n");

    columnItrs = getColumnIters();     // Get iterators from the methods of the
    rowItrs    = getRowIters();        //   child class (via virtual functions).

    // Make sure that there are in fact rows and columns to write
    if (columnItrs.begin == columnItrs.end) {
        printf("Error: No columns to write\n");
        return false;
    }

    if (rowItrs.begin == rowItrs.end) {
        printf("Error: No rows to write\n");
        return false;
    }

    // By default we append to files, meaning that if 'fname' already exists,
    //   it will be appended to, rather than overwritten.
    if (!(fs = fopen(fname.c_str(), "a"))) {
        printf("Error: fopen() returned NULL pointer\n");
        fclose(fs);
        return false;
    }

    if (isRowHeader())
        buf += comma;

    // Print column header, if it exists
    if (isColumnHeader()) {

        // Iterate over each column
        for (CellSpecItr itr = columnItrs.begin; \
             itr != columnItrs.end;              \
             itr = next(itr)) {

            // Add comma, if not the first column
            if (itr != columnItrs.begin)
                buf += comma;

            // Add column name
            buf += getColumnName(*itr);
        }

        // Terminate line
        buf += newline;
    }

    // Iterate over each row
    for (CellSpecItr rItr = rowItrs.begin; \
         rItr != rowItrs.end;              \
         rItr = next(rItr)) {

        // Print row header if exists
        if (isRowHeader())
            buf += getRowName(*rItr) + comma;

        // Iterate over each column
        for (CellSpecItr cItr = columnItrs.begin;
             cItr != columnItrs.end;
             cItr = next(cItr)) {

            // Add comma if not the first column
            if (cItr != columnItrs.begin)
                buf += comma;

            // Retrieve the contents of this (row, column) pair and add
            //   it to the output buffer.
            buf += getCell(*rItr, *cItr);
        }

        // Terminate line
        buf += newline;
    }

    // Perform write
    stringSize = buf.size();
    if (fwrite(buf.c_str(), sizeof(char), stringSize, fs) < (size_t)stringSize) {
        printf("Error: std::fwrite returned an error\n");
        fclose(fs);
        return false;
    }

    // Free memory for file pointer and return victorious
    fclose(fs);
    return true;
}

/////////////////////////////////////////
// TimeSeries CSV Exporter
/////////////////////////////////////////

template <typename T>
bool
TimeSeriesCSVExport<T>::Add(TimeSeries<T> *ts) {
    vector<T> *tsPointer;
    double     tsTime0;
    double     tsTimeMax;
    string     tsName;
    long       tsSize;

    // Make sure TimeSeries exists
    if (ts == nullptr) {
        printf("Error: TimeSeries pointer was NULL\n");
        return false;
    }

    // Make sure ::Close() has already been called on the time series
    if (ts->IsWritable()) {
        printf("Error: TimeSeries is still writable\n");
        return false;
    }

    // Make sure that the period length of the time series being added
    //   is identical to the period length of other time series which
    //   have already been queued for export.
    if (nTimeSeries != 0 && ts->GetPeriodLength() != tsPeriodLength) {
        printf("Error: All TimeSeries must have the same periodLength\n");
        return false;
    } else if (nTimeSeries == 0) {
        tsPeriodLength = ts->GetPeriodLength();
        printf("::Add(): tsPeriodLength=%d\n", tsPeriodLength);
    }

    // GetTime0 returns a double, but it needs to be an integer, so that
    //   we can test which value of 't' the associated vector should first
    //   show up at. If GetTime0 returns 3.5, then the first element in
    //   its vector representa data collected over the period [3.5, 4).
    // Therefore, we use the ceil() function on Time0, which yields
    //   the value of 't' at which elements of this vector should begin
    //   appearing in CSV output.
    // Note that ceil(0) = 0, so TimeSeries which begin at t=0 (for instance,
    //   population size) will still be represented for in the CSV file at
    //   t=0.
    tsTime0   = ceil(ts->GetTime0());
    tsTimeMax = ts->GetTimeMax();
    tsPointer = ts->GetVector();
    tsName    = ts->GetName();
    tsSize    = tsPointer->size();

    rows      = nullptr;
    columns   = nullptr;

    tsTime0s.push_back(tsTime0);
    tsTimeMaxs.push_back(tsTimeMax);
    tsVectors.push_back(tsPointer);
    tsNames.push_back(tsName);
    tsSizes.push_back(tsSize);

    // Update the largest value of 't' that will be written to CSV
    // file, and the number of TimeSeries subject to export.
    tMax =   tsTimeMax > tMax \
           ? tsTimeMax : tMax;

    nTimeSeries += 1;

    return true;
}

// Utility function for debugging: prints a vector of integers by taking
//   a pointer to said vector.
void printVector(vector<int> *v) {
    for (int i = 0; i < v->size(); ++i)
    {
        printf("x: %2d y: %2d\n", i, (*v)[i]);
    }
    printf("\n");
}

// POTENTIAL MEMORY LEAK: reallocation of 'columns' variable over successive
//   calls to ::getColumnIters, ::getRowIters may lead to wasted memory
//   as pointer to previous vector referenced by 'columns' and 'rows' is
//   lost.
template <typename T>
CellSpecItrs
TimeSeriesCSVExport<T>::getColumnIters(void) {
    // "+1" is to account for the extra column we need to represent time.
    // Therefore, column 0 is for time, not value.
    columns = new vector<CellSpec>(nTimeSeries + 1);
    CellSpecItrs cellSpecItrs;

    iota(columns->begin(), columns->end(), (CellSpec)0);

    printf("nTimeSeries=%d\n", nTimeSeries);
    printf("printing columnIters vector:\n");
    printVector(columns);

    cellSpecItrs.begin = columns->begin();
    cellSpecItrs.end   = columns->end();

    return cellSpecItrs;
}

// See above
template <typename T>
CellSpecItrs
TimeSeriesCSVExport<T>::getRowIters(void) {
    int nPeriods;
    CellSpecItrs cellSpecItrs;

    nPeriods = (int)ceil(tMax / (double)tsPeriodLength) + 1;

    rows = new vector<CellSpec>(nPeriods);

    printf("tMax=%f, tsPeriodLength=%d, nPeriods=%d\n", tMax, tsPeriodLength, nPeriods);
    iota(rows->begin(), rows->end(), (CellSpec)0);

    printf("Printing rowIters vector:\n");
    printVector(rows);

    cellSpecItrs.begin = rows->begin();
    cellSpecItrs.end   = rows->end();

    return cellSpecItrs;
}

template <typename T>
bool
TimeSeriesCSVExport<T>::isColumnHeader(void) {
    // Really?
    return true;
}

template <typename T>
bool
TimeSeriesCSVExport<T>::isRowHeader(void) {
    // We don't represent time/period using a row header, so in this case
    //   we return false.
    return false;
}

template <typename T>
string
TimeSeriesCSVExport<T>::getRowName(CellSpec rowSpec) {
    // While ::getRowName(CellSpec) should never be called by CSVExport,
    //   we return a dummy string just to be safe.
    return string("");
}

template <typename T>
string
TimeSeriesCSVExport<T>::getColumnName(CellSpec columnSpec) {
    // Right now, we call the first column "Period". In the future, this
    //   may be changed to "Time," in which case columnSpecs > 0 will
    //   have to return a different value than they currently due, assuming
    //   periodLength != 1.
    string timeHeader = string("Period");

    if (columnSpec == 0)
        return timeHeader;
    else
        return tsNames[columnSpec - 1];
}

template <typename T>
string
TimeSeriesCSVExport<T>::getCell(CellSpec rowSpec, CellSpec columnSpec) {
    // 'rowSpec' corresponds to period because the first row is period=0.
    // 'columnSpec'-1 corresponds to the index of the TimeSeries being
    //   referenced.
    int period, tsIdx, tsTime0, tsTimeMax;
    T cellVal;
    string empty;

    // Return period # if first column
    if (columnSpec == 0)
        return to_string(rowSpec);

    empty     = string("");

    period    = rowSpec;           // Which period is being requested
    tsIdx     = columnSpec - 1;    // Which timeSeries is being requested
    tsTime0   = tsTime0s[tsIdx];   // time0 for this timeSeries
    tsTimeMax = tsTimeMaxs[tsIdx]; // timeMax for this timeSeries

    // If time is below time0 or above timeMax, output empty string
    if ( (period * tsPeriodLength) < tsTime0   || \
         (period * tsPeriodLength) > tsTimeMax     )
        return empty;

    // Otherwise, retrieve value from the timeSeries, convert to a string,
    //   and return.
    cellVal  = tsVectors[tsIdx]->at(period);

    return to_string(cellVal);
}

/////////////////////////////////////////
// PyramidTimeSeries CSV Exporter
/////////////////////////////////////////



PyramidTimeSeriesCSVExport/*<T>*/::~PyramidTimeSeriesCSVExport()
{
// Deallocate any memory allocated on instantiation
}

bool
PyramidTimeSeriesCSVExport/*<T>*/::Add(PyramidTimeSeries/*<T>*/ *ptse) {

    // Make sure PyramidTimeSeries exists
    if (ptse == nullptr) {
        printf("Error: PyramidTimeSeries pointer was NULL\n");
        return false;
    }

    // Make sure ::Close() has already been called on the time series
    if (ptse->IsWritable()) {
        printf("Error: PyramidTimeSeries is still writable\n");
        return false;
    }

    // Make sure that data has not been added before
    if (hasData) {
        printf("Error: PyramidTimeSeries data has already been added. Only 1 PyramidTimeSeries data can be added.\n");
        return false;
    }
    else
    {
        hasData = true;
    }

    rows      = nullptr;
    columns   = nullptr;

    nCategories = ptse->GetNumberCategories();
    ageBreaks = ptse->GetAgeBreaks();

    ptsePeriodLength = ptse->GetPeriodLength();
    tMax             = ptse->GetTimeMax();
    time0            = ptse->GetTime0();


    PTSptr = ptse;

    return true;
}


CellSpecItrs
PyramidTimeSeriesCSVExport/*<T>*/::getColumnIters(void) {
    // TODO


    // The first "+1" is to account for the extra column we need to represent time.
    // The second "+1" is to account for the extra column needed to represent the age group.
    // Therefore, column 0 is for time, not value, and column 1 is for the age group.
    columns = new vector<CellSpec>(1 + 1 + nCategories);
    CellSpecItrs cellSpecItrs;

    iota(columns->begin(), columns->end(), (CellSpec)0);

    printVector(columns);

    cellSpecItrs.begin = columns->begin();
    cellSpecItrs.end   = columns->end();

    return cellSpecItrs;

}

CellSpecItrs
PyramidTimeSeriesCSVExport/*<T>*/::getRowIters(void) {
    int nPeriods, nAgeGroups;
    CellSpecItrs cellSpecItrs;

    nPeriods = (int)ceil(tMax / (double)ptsePeriodLength) + 1;

    nAgeGroups = ageBreaks.size() + 1;

    rows = new vector<CellSpec>(nPeriods * nAgeGroups);

    iota(rows->begin(), rows->end(), (CellSpec)0);

    cellSpecItrs.begin = rows->begin();
    cellSpecItrs.end   = rows->end();

    return cellSpecItrs;
}

bool
PyramidTimeSeriesCSVExport/*<T>*/::isColumnHeader(void) {
    // Really?
    return true;
}

bool
PyramidTimeSeriesCSVExport/*<T>*/::isRowHeader(void) {
    // Really?
    return false;
}

string
PyramidTimeSeriesCSVExport/*<T>*/::getRowName(CellSpec rowSpec) {
    return string("");
}

string
PyramidTimeSeriesCSVExport/*<T>*/::getColumnName(CellSpec columnSpec) {
    string timeHeader = string("Period");

    if (columnSpec == 0)
        return timeHeader;
    else if (columnSpec == 1)
        return string("Age Group");
    else
        return string("Category ") + to_string(columnSpec-2);
}

string
PyramidTimeSeriesCSVExport/*<T>*/::getCell(CellSpec rowSpec, CellSpec columnSpec) {

    int period, categoryIdx, nAgeGroups, ageGroupIdx, time0, timeMax, cellVal;

    string empty, ageRange;

    // there is one more age group than there are age breaks
    nAgeGroups = ageBreaks.size() + 1;

    // calculate period by finding the quotient of rowSpec and nAgeGroups
    period = rowSpec/nAgeGroups;

    // Find age group index (0,1,...)
    ageGroupIdx = rowSpec % nAgeGroups;

    // Return period # if first column
    if (columnSpec == 0)
        return to_string(period);

    // Return age-range if second column
    if (columnSpec == 1)
    {

        // Return different strings depending on age group index
        if (ageGroupIdx == 0)
        {
            ageRange = string("0-" + to_string((int)ageBreaks[0]));
        }
        else if (ageGroupIdx == nAgeGroups - 1)
        {
            ageRange = string(to_string((int)ageBreaks[ageGroupIdx-1]) + "-inf");
        }
        else
        {
            ageRange = string(to_string((int)ageBreaks[ageGroupIdx-1]) + "-" + to_string((int)ageBreaks[ageGroupIdx]));
        }

        return ageRange;
    }

    empty = string("");

    categoryIdx = columnSpec - 2;
    time0 = 0;
    timeMax = tMax;

    if ( (period * ptsePeriodLength) < time0   || \
         (period * ptsePeriodLength) > timeMax     )
        return empty;

    cellVal = PTSptr->GetTotalInAgeGroupAndCategoryAtPeriod(period, ageGroupIdx, categoryIdx);

    return to_string(cellVal);
}


/////////////////////////////////////////
// TimeStatistic CSV Exporter
/////////////////////////////////////////

TimeStatisticsCSVExport::~TimeStatisticsCSVExport()
{
    delete _columns;
    delete _rows;
}


bool
TimeStatisticsCSVExport::Add(TimeStatistic *tst) {

    if (tst == nullptr) {
        printf("Error: TimeStatistics pointer was NULL\n");
        return false;
    }

    stats.push_back(tst);
    nStats += 1;

    return true;
}


CellSpecItrs
TimeStatisticsCSVExport::getColumnIters(void) {
    _columns = new vector<CellSpec>(columns.size());
    CellSpecItrs cellSpecItrs;

    iota(_columns->begin(), _columns->end(), (CellSpec) 0);

    cellSpecItrs.begin = _columns->begin();
    cellSpecItrs.end   = _columns->end();

    return cellSpecItrs;
}


CellSpecItrs
TimeStatisticsCSVExport::getRowIters(void) {
    _rows = new vector<CellSpec>(nStats);
    CellSpecItrs cellSpecItrs;

    iota(_rows->begin(), _rows->end(), (CellSpec) 0);

    cellSpecItrs.begin = _rows->begin();
    cellSpecItrs.end   = _rows->end();

    return cellSpecItrs;
}

bool
TimeStatisticsCSVExport::isColumnHeader(void) {
    // Really?
    return true;
}

bool
TimeStatisticsCSVExport::isRowHeader(void) {
    // Really?
    return true;
}


string
TimeStatisticsCSVExport::getRowName(CellSpec rowSpec) {
    return stats.at(rowSpec)->name;
}


string
TimeStatisticsCSVExport::getColumnName(CellSpec columnSpec) {
    int j;
    string rowName;

    // Look through the columns map in the default sorted order. When map entry
    //   #'columnSpec' is found, return the string associated with this map entry.
    j = 0;
    for (auto i = columns.cbegin(); \
         i != columns.cend();                 \
         ++i, ++j)
        if (j == (int)columnSpec)
            return i->second;

    // Should not reach this point
    throw out_of_range("columnSpec was >= columns.size()");

    return string("");
}


string
TimeStatisticsCSVExport::getCell(CellSpec rowSpec, CellSpec columnSpec) {
    TimeStatistic *tst;               // Pointer to TimeStatistic referenced by
                                      //   rowSpec
    TimeStatType statType;            // Holds the TimeStatType implied by
                                      //   'columnSpec'
    double result;                    // Holds result of call to TimeStatistic
                                      //   getter class
    int j;                            // To keep track of numeric index on columns
                                      //   std::map
    bool found;                       // Indicates whether columnSpec specified
                                      //   a valid column

    j     = 0;
    found = false;
    for (auto i = columns.cbegin(); \
         i != columns.cend();       \
         ++i, ++j)
        if (j == (int)columnSpec) {
            statType = i->first;
            found    = true;
        }

    // Did the loop fail to find the right TimeStatType?
    if (!found) {
        throw out_of_range("columnSpec failed to match with an enum!");
        return string("");
    }

    // Identify the TimeStatistic class from which data will be drawn
    try { tst = stats.at(rowSpec); } catch (...) {
        throw out_of_range("rowSpec was >= stats.size()");
    }

    /* SKETCHY!!!!! */
    switch (statType) {
        case TimeStatType::Sum      : result = -100;
                                      break;
        case TimeStatType::Count    : result = tst->GetCount();
                                      break;
        case TimeStatType::Mean     : result = tst->GetMean();
                                      break;
        case TimeStatType::Variance : result = tst->GetVariance();
                                      break;
        case TimeStatType::Min      : result = tst->GetMin();
                                      break;
        case TimeStatType::Max      : result = tst->GetMax();
                                      break;
        default                     : result = -100;
                                      break;
    }

    if (result == -100) {
        throw out_of_range("Unsupported TimeStatType");
        return string("");
    }

    return to_string(result);
}


}
