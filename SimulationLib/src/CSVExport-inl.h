#include "../include/SimulationLib/CSVExport.h"

namespace SimulationLib {

/////////////////////////////////////////
// General CSV Exporter
/////////////////////////////////////////
CSVExportEngine::CSVExportEngine(string _fname) {
    fname = _fname;
}

// Empty destructor, for now.
CSVExportEngine::~CSVExportEngine() {}

bool CSVExportEngine::Write(void) {
    string comma, newline;             // Strings for commonly used symbols
    CellSpecItrs columnItrs, rowItrs;  // Iterators against rows and columns

    comma      = string(",");
    newline    = string("\n");

    columnItrs = getColumnIters();     // Get iterators from the methods of the
    rowItrs    = getRowIters();        //   child class (via virtual functions).

    // Create output buffer
    ofstream fout(fname);

    // Make sure that there are in fact rows and columns to write
    if (columnItrs.begin == columnItrs.end) {
        printf("Error: No columns to write\n");
        return false;
    }

    if (rowItrs.begin == rowItrs.end) {
        printf("Error: No rows to write\n");
        return false;
    }

    // Print column header, if it exists
    if (isColumnHeader()) {

        // Iterate over each column
        for (CellSpecItr itr = columnItrs.begin; \
             itr != columnItrs.end;              \
             itr = next(itr)) {

            // Add comma, if not the first column
            if (itr != columnItrs.begin)
                fout << comma;

            // Add column name
            fout << getColumnName(*itr);
        }

        // Terminate line
        fout << newline;
    }

    // Iterate over each row
    for (CellSpecItr rItr = rowItrs.begin; \
         rItr != rowItrs.end;              \
         rItr = next(rItr)) {

        // Print row header if exists
        if (isRowHeader())
            fout << getRowName(*rItr) + comma;

        // Iterate over each column
        for (CellSpecItr cItr = columnItrs.begin;
             cItr != columnItrs.end;
             cItr = next(cItr)) {

            // Add comma if not the first column
            if (cItr != columnItrs.begin)
                fout << comma;

            // Retrieve the contents of this (row, column) pair and add
            //   it to the output buffer.
            fout << getCell(*rItr, *cItr);
        }

        // Terminate line
        fout << newline;
    }

    // Flush and close output buffer
    fout.flush();
    fout.close();

    return true;
}

/////////////////////////////////////////
// TimeSeries CSV Exporter
/////////////////////////////////////////

template <typename T>
bool
TimeSeriesExport<T>::Add(TimeSeries<T> *ts, int id) {
    // vector<T> *tsPointer;
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
        // printf("::Add(): tsPeriodLength=%d\n", tsPeriodLength);
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
    tsName    = ts->GetName();

    rows      = nullptr;
    columns   = nullptr;

    tsPtrs.push_back(ts);
    tsTime0s.push_back(tsTime0);
    tsTimeMaxs.push_back(tsTimeMax);
    tsNames.push_back(tsName);
    trajectoryID.push_back(id);

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
TimeSeriesExport<T>::getColumnIters(void) {
    // 3 columns: time, value, run
    columns = new vector<CellSpec>(3);
    CellSpecItrs cellSpecItrs;

    iota(columns->begin(), columns->end(), (CellSpec)0);

    // printf("nTimeSeries=%d\n", nTimeSeries);
    // printf("printing columnIters vector:\n");
    // printVector(columns);

    cellSpecItrs.begin = columns->begin();
    cellSpecItrs.end   = columns->end();

    return cellSpecItrs;
}

// See above
template <typename T>
CellSpecItrs
TimeSeriesExport<T>::getRowIters(void) {
    int nPeriods;
    CellSpecItrs cellSpecItrs;

    nPeriods = (int)ceil(tMax / (double)tsPeriodLength) + 1;

    rows = new vector<CellSpec>(nPeriods * nTimeSeries);

    // printf("tMax=%f, tsPeriodLength=%d, nPeriods=%d\n", tMax, tsPeriodLength, nPeriods);
    iota(rows->begin(), rows->end(), (CellSpec)0);

    cellSpecItrs.begin = rows->begin();
    cellSpecItrs.end   = rows->end();

    return cellSpecItrs;
}

template <typename T>
bool
TimeSeriesExport<T>::isColumnHeader(void) {
    // Really?
    return true;
}

template <typename T>
bool
TimeSeriesExport<T>::isRowHeader(void) {
    // We don't represent time/period using a row header, so in this case
    //   we return false.
    return false;
}

template <typename T>
string
TimeSeriesExport<T>::getRowName(CellSpec rowSpec) {
    // While ::getRowName(CellSpec) should never be called by CSVExport,
    //   we return a dummy string just to be safe.w
    return string("");
}

template <typename T>
string
TimeSeriesExport<T>::getColumnName(CellSpec columnSpec) {
    // Right now, we call the first column "Period". In the future, this
    //   may be changed to "Time," in which case columnSpecs > 0 will
    //   have to return a different value than they currently due, assuming
    //   periodLength != 1.
    switch (columnSpec)
    {
        case 0:  return string("period");
        case 1:  return string("trajectory");
        case 2:  return string("value");
        default: return string("unsupported columnSpec");
    }
}

template <typename T>
string
TimeSeriesExport<T>::getCell(CellSpec rowSpec, CellSpec columnSpec) {
    // 'rowSpec' corresponds to period because the first row is period=0.
    // 'columnSpec'-1 corresponds to the index of the TimeSeries being
    //   referenced.
    int period, tsIdx, tsTime0, tsTimeMax;
    int nPeriods;
    T cellVal;
    string empty;

    nPeriods = (int)ceil(tMax / (double)tsPeriodLength) + 1;

    // Return period # if first column
    if (columnSpec == 0)
        return to_string(rowSpec % nPeriods);

    if (columnSpec == 1)
        return to_string(trajectoryID[rowSpec / nPeriods]);

    empty     = string("");

    period    = rowSpec % nPeriods;  // Which period is being requested
    tsIdx     = rowSpec / nPeriods;  // Which timeSeries is being requested
    tsTime0   = tsTime0s[tsIdx];     // time0 for this timeSeries
    tsTimeMax = tsTimeMaxs[tsIdx];   // timeMax for this timeSeries

    // If time is below time0 or above timeMax, output empty string
    if ( (period * tsPeriodLength) < tsTime0   || \
         (period * tsPeriodLength) > tsTimeMax     )
        return empty;

    // Otherwise, retrieve value from the timeSeries, convert to a string,
    //   and return.
    cellVal  = tsPtrs[tsIdx]->GetTotalAtTime(period * tsPeriodLength);

    // printf("Printing tsVectors[%d]\n", tsIdx);
    // for (int i = 0; i < tsVectors[tsIdx]->size(); ++i)
    //     printf("\tPrinting element: %4f\n", (double)tsVectors[tsIdx]->at(i));

    return to_string(cellVal);
}

/////////////////////////////////////////
// PyramidTimeSeries CSV Exporter
/////////////////////////////////////////



PyramidTimeSeriesExport/*<T>*/::~PyramidTimeSeriesExport()
{
// Deallocate any memory allocated on instantiation
    delete columns;
    delete rows;
}

bool
PyramidTimeSeriesExport/*<T>*/::Add(PyramidTimeSeries/*<T>*/ *ptse, int id) {

    int ptseTime0, ptseTimeMax;
    PyramidTimeSeries *ptsePointer;

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

    // Make sure that the period length of the time series being added
    //   is identical to the period length of other time series which
    //   have already been queued for export.
    if (nPyramidTimeSeries != 0 &&
        ptse->GetPeriodLength() != ptsePeriodLength) {
        printf("Error: All PyramidTimeSeries must have the same periodLength\n");
        return false;
    }

    ptsePeriodLength = ptse->GetPeriodLength();

    // Make sure that new PyramidTimeSeries has same number of age groups
    if (nPyramidTimeSeries != 0 &&
        ptse->GetAgeBreaks() != ageBreaks)
    {
        printf("Error: All PyramidTimeSeries must have the same age breaks\n");
        return false;
    } else if (nPyramidTimeSeries == 0) {
        ageBreaks = ptse->GetAgeBreaks();
    }

    // Make sure that new PyramidTimeSeries has same number of categories
    if (nPyramidTimeSeries != 0 &&
        ptse->GetNumberCategories() != nCategories)
    {
        printf("Error: All PyramidTimeSeries must have the same number of categories\n");
        return false;
    } else if (nPyramidTimeSeries == 0) {
        nCategories = ptse->GetNumberCategories();
    }

    // nTotalCategories += ptse->GetNumberCategories();

    ptseTime0   = ptse->GetTime0();
    ptseTimeMax = ptse->GetTimeMax();
    ptsePointer = ptse;

    rows        = nullptr;
    columns     = nullptr;

    ptseTime0s.push_back(ptseTime0);
    ptseTimeMaxs.push_back(ptseTimeMax);
    ptsePointers.push_back(ptsePointer);
    trajectoryID.push_back(id);

    tMax =   ptseTimeMax > tMax \
           ? ptseTimeMax : tMax;

    nPyramidTimeSeries += 1;

    return true;
}


CellSpecItrs
PyramidTimeSeriesExport/*<T>*/::getColumnIters(void) {
    int nTotalCategories = nCategories * nPyramidTimeSeries;

    // The first "+1" is to account for the extra column we need to represent time.
    // The second "+1" is for trajectory
    // The third "+1" is to account for the extra column needed to represent the age group.
    // The fourth "+1" is for category
    columns = new vector<CellSpec>(1 + 1 + 1 + 1 + 1);
    CellSpecItrs cellSpecItrs;

    iota(columns->begin(), columns->end(), (CellSpec)0);

    cellSpecItrs.begin = columns->begin();
    cellSpecItrs.end   = columns->end();

    return cellSpecItrs;

}

CellSpecItrs
PyramidTimeSeriesExport/*<T>*/::getRowIters(void) {
    int nAgeGroups;
    int nPeriods;
    CellSpecItrs cellSpecItrs;

    nPeriods = (int)ceil((double)tMax/(double)ptsePeriodLength) + 1;
    nAgeGroups = ageBreaks.size() + 1;

    rows = new vector<CellSpec>(nPeriods * nPyramidTimeSeries * nAgeGroups * nCategories);

    iota(rows->begin(), rows->end(), (CellSpec)0);

    cellSpecItrs.begin = rows->begin();
    cellSpecItrs.end   = rows->end();

    return cellSpecItrs;
}

bool
PyramidTimeSeriesExport/*<T>*/::isColumnHeader(void) {
    // Really?
    return true;
}

bool
PyramidTimeSeriesExport/*<T>*/::isRowHeader(void) {
    // Really?
    return false;
}

string
PyramidTimeSeriesExport/*<T>*/::getRowName(CellSpec rowSpec) {
    return string("");
}

string
PyramidTimeSeriesExport/*<T>*/::getColumnName(CellSpec columnSpec) {
    switch (columnSpec) {
        case 0: return string("period"); break;
        case 1: return string("trajectory"); break;
        case 2: return string("age group"); break;
        case 3: return string("category"); break;
        case 4: return string("value"); break;

        default: return string("UNKNOWN COLUMN"); break;
    }
}

string
PyramidTimeSeriesExport/*<T>*/::getCell(CellSpec rowSpec, CellSpec columnSpec) {

    int ptseIdx, period, categoryIdx, nAgeGroups, ageGroupIdx, time0, timeMax, cellVal;

    string empty, ageRange;

    int nPeriods = (int)ceil((double)tMax/(double)ptsePeriodLength) + 1;

    // there is one more age group than there are age breaks
    nAgeGroups = ageBreaks.size() + 1;

    // calculate period by finding the quotient of rowSpec and nAgeGroups
    period = (rowSpec / (nCategories*nAgeGroups)) % nPeriods;

    // Find age group index (0,1,...)
    ageGroupIdx = (rowSpec/nCategories) % nAgeGroups;

    // Find category (i.e. gender) index
    categoryIdx = rowSpec % nCategories;

    // Find trajectory
    ptseIdx = rowSpec / (nPeriods*nAgeGroups*nCategories);

    // Return period # if first column
    if (columnSpec == 0)
        return to_string(period);

    if (columnSpec == 1)
        return to_string(trajectoryID[ptseIdx]);

    // Return ageRange if third column
    if (columnSpec == 2)
    {

        // Return different strings depending on age group index
        if (ageGroupIdx == 0 && nAgeGroups == 1)
            ageRange = string("0-inf");
        else if (ageGroupIdx == 0)
            ageRange = string("0-" + to_string((int)ageBreaks[0]));
        else if (ageGroupIdx == nAgeGroups - 1)
            ageRange = string(to_string((int)ageBreaks[ageGroupIdx-1]) + "-inf");
        else
            ageRange = string(to_string((int)ageBreaks[ageGroupIdx-1]) + \
                              "-" + \
                              to_string((int)ageBreaks[ageGroupIdx]));

        return ageRange;
    }

    if (columnSpec == 3)
        return to_string(categoryIdx);

    empty = string("");

    time0 = ptseTime0s[ptseIdx];
    timeMax = tMax;

    if ( (period * ptsePeriodLength) < time0   || \
         (period * ptsePeriodLength) > timeMax )
        return empty;

    cellVal = ptsePointers[ptseIdx]->GetTotalInAgeGroupAndCategoryAtPeriod(period, ageGroupIdx, categoryIdx);

    return to_string(cellVal);
}

template <typename T>
bool
PyramidDataExport<T>::Add(PyramidData<T> *_pd) {

    // Make sure PyramidData exists
    if (_pd == nullptr) {
        printf("Error: PyramidData pointer was NULL\n");
        return false;
    }

    if (n_pds == 0) {
        nCategories = _pd->GetNumCategories();
        ageBreaks   = _pd->GetAgeBreaks();
    }

    // Make sure same number of categories
    if (nCategories != _pd->GetNumCategories()) {
        printf("Error: PyramidData had different number of categories. Existing: %d cats, New: %d cats\n", \
                (int)nCategories, (int)_pd->GetNumCategories());
        return false;
    }

    // Make sure same age breaks
    if (ageBreaks != _pd->GetAgeBreaks()) {
        printf("Error: PyramidData had different age groups\n");
        return false;
    }

    pds.push_back(_pd);
    n_pds += 1;

    return true;
}


template <typename T>
CellSpecItrs
PyramidDataExport<T>::getColumnIters(void) {

    // The "+1" is to account for the extra column needed to represent the age group.
    // Therefore, column 0 is for the age group.
    int nColumns = 1 + (nCategories * n_pds);

    columns = new vector<CellSpec>(nColumns);
    CellSpecItrs cellSpecItrs;

    iota(columns->begin(), columns->end(), (CellSpec)0);

    cellSpecItrs.begin = columns->begin();
    cellSpecItrs.end   = columns->end();

    return cellSpecItrs;

}

template <typename T>
CellSpecItrs
PyramidDataExport<T>::getRowIters(void) {
    CellSpecItrs cellSpecItrs;

    int nRows = 1 + ageBreaks.size();

    // 1 row for the moment
    rows = new vector<CellSpec>(nRows);

    iota(rows->begin(), rows->end(), (CellSpec)0);

    cellSpecItrs.begin = rows->begin();
    cellSpecItrs.end   = rows->end();

    return cellSpecItrs;
}

template <typename T>
bool
PyramidDataExport<T>::isColumnHeader(void) {
    // Really?
    return true;
}

template <typename T>
bool
PyramidDataExport<T>::isRowHeader(void) {
    // Really?
    return false;
}

template <typename T>
string
PyramidDataExport<T>::getRowName(CellSpec rowSpec) {
    return string("");
}

template <typename T>
string
PyramidDataExport<T>::getColumnName(CellSpec columnSpec) {
    string categoryStr;
    int categoryIdx;

    categoryStr = string("");

    // the index of the category can be found using modulo
    categoryIdx = (columnSpec - 1) % nCategories;

    if (columnSpec == 0)
        return string("Age Group");
    else {
        categoryStr += string("Category ");
        categoryStr += to_string(categoryIdx);
        return categoryStr;
    }
}

template <typename T>
string
PyramidDataExport<T>::getCell(CellSpec rowSpec, CellSpec columnSpec) {

    int categoryIdx, nAgeGroups, ageGroupIdx;
    int pdIdx;
    T cellVal;

    string empty, ageRange;

    // there is one more age group than there are age breaks
    nAgeGroups = ageBreaks.size() + 1;

    // Find age group index (0,1,...)
    ageGroupIdx = rowSpec % nAgeGroups;

    // Find which category is to be printed
    categoryIdx = (columnSpec - 1) % nCategories;

    pdIdx = (columnSpec - 1) / nCategories;

    // Return ageRange if second column
    if (columnSpec == 0) {
        // Return different strings depending on age group index
        if (ageGroupIdx == 0 && nAgeGroups == 1) {
            return string("0-inf");
        }
        else if (ageGroupIdx == 0) {
            return string("0-" + to_string((int)ageBreaks[0]));
        }
        else if (ageGroupIdx == nAgeGroups - 1) {
            return string(to_string((int)ageBreaks[ageGroupIdx-1]) + "-inf");
        }
        else {
            return string(to_string((int)ageBreaks[ageGroupIdx-1]) + "-" + to_string((int)ageBreaks[ageGroupIdx]));
        }
    }
    empty = string("");


    cellVal = pds[pdIdx]->GetTotalInAgeGroupAndCategory(ageGroupIdx, categoryIdx);

    return to_string(cellVal);
}


/////////////////////////////////////////
// TimeStatistic CSV Exporter
/////////////////////////////////////////

TimeStatisticsExport::~TimeStatisticsExport()
{
    delete _columns;
    delete _rows;
}


bool
TimeStatisticsExport::Add(TimeStatistic *tst) {

    if (tst == nullptr) {
        printf("Error: TimeStatistics pointer was NULL\n");
        return false;
    }

    stats.push_back(tst);
    nStats += 1;

    return true;
}


CellSpecItrs
TimeStatisticsExport::getColumnIters(void) {
    _columns = new vector<CellSpec>(columns.size());
    CellSpecItrs cellSpecItrs;

    iota(_columns->begin(), _columns->end(), (CellSpec) 0);

    cellSpecItrs.begin = _columns->begin();
    cellSpecItrs.end   = _columns->end();

    return cellSpecItrs;
}


CellSpecItrs
TimeStatisticsExport::getRowIters(void) {
    _rows = new vector<CellSpec>(nStats);
    CellSpecItrs cellSpecItrs;

    iota(_rows->begin(), _rows->end(), (CellSpec) 0);

    cellSpecItrs.begin = _rows->begin();
    cellSpecItrs.end   = _rows->end();

    return cellSpecItrs;
}

bool
TimeStatisticsExport::isColumnHeader(void) {
    // Really?
    return true;
}

bool
TimeStatisticsExport::isRowHeader(void) {
    // Really?
    return true;
}


string
TimeStatisticsExport::getRowName(CellSpec rowSpec) {
    return stats.at(rowSpec)->name;
}


string
TimeStatisticsExport::getColumnName(CellSpec columnSpec) {
    int j;
    string rowName;

    j = 0;

    // If we are looking at the first column of the statistics column names
    //   we should add a "," before the name so that the column names row of
    //   the CSV lines up correctly with the rows beneath it
    if (columnSpec == 0)
    {
        for (auto i = columns.cbegin(); \
         i != columns.cend();                 \
         ++i, ++j)
        if (j == (int)columnSpec)
            return "," + i->second;
    }

    // Look through the columns map in the default sorted order. When map entry
    //   #'columnSpec' is found, return the string associated with this map entry.
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
TimeStatisticsExport::getCell(CellSpec rowSpec, CellSpec columnSpec) {
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
        case TimeStatType::Sum      : result = tst->GetSum();
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
        default                     : throw out_of_range("Unsupported TimeStatType");
                                      break;
    }

    return to_string(result);
}


}
