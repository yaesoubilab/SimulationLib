#include "CSVExport-new.h"

namespace SimulationLib {

/////////////////////////////////////////
// General CSV Exporter
/////////////////////////////////////////
template <typename T>
CSVExport<T>::CSVExport(string _fname) {
    fname = _fname;
}

template <typename T>
CSVExport<T>::~CSVExport() {}

template <typename T>
bool
CSVExport<T>::Write(void) {
    int stringSize;
    string buf, comma, newline;
    FILE *fs;
    CellSpecItrs columnItrs, rowItrs;

    buf        = string("");
    comma      = string(",");
    newline    = string("\n");

    columnItrs = getColumnIters();
    rowItrs    = getRowIters();

    if (columnItrs.begin == columnItrs.end) {
        printf("Error: No columns to write\n");
        return false;
    }

    if (rowItrs.begin == rowItrs.end) {
        printf("Error: No rows to write\n");
        return false;
    }

    if (!(fs = fopen(fname.c_str(), "a"))) {
        printf("Error: fopen() returned NULL pointer\n");
        fclose(fs);
        return false;
    }

    // Print column header, if exists
    if (isColumnHeader()) {
        for (CellSpecItr itr = columnItrs.begin; \
             itr != columnItrs.end;              \
             itr = next(itr)) {

            // Add comma, if not the first column
            if (itr != columnItrs.begin)
                buf += comma;

            // Add column name
            buf += getColumnName(*itr);
        }

        buf += newline;
    }

    // For each row
    for (CellSpecItr rItr = rowItrs.begin; \
         rItr != rowItrs.end;              \
         rItr = next(rItr)) {

        // Print row header if exists
        if (isRowHeader())
            buf += getRowName(*rItr) + comma;

        // For each column
        for (CellSpecItr cItr = columnItrs.begin;
             cItr != columnItrs.end;
             cItr = next(cItr)) {

            // Add comma if not the first column
            if (cItr != columnItrs.begin)
                buf += comma;

            buf += getCell(*rItr, *cItr);
        }

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

    if (ts == nullptr) {
        printf("Error: TimeSeries pointer was NULL\n");
        return false;
    }

    if (ts->IsWritable()) {
        printf("Error: TimeSeries is still writable\n");
        return false;
    }

    if (nTimeSeries != 0 && ts->GetPeriodLength() != tsPeriodLength) {
        printf("Error: All TimeSeries must have the same periodLength\n");
        return false;
    } else if (nTimeSeries == 0)
        tsPeriodLength = ts->GetPeriodLength();

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

    tsTime0s.push_back(tsTime0);
    tsTimeMaxs.push_back(tsTimeMax);
    tsVectors.push_back(tsPointer);
    tsNames.push_back(tsName);
    tsSizes.push_back(tsSize);

    // Update the largest value of 't' that will be written to CSV
    // file, and the number of TimeSeries subject to export.
    tMax =   (tsTime0 + tsSize - 1) > tMax \
           ? (tsTime0 + tsSize - 1) : tMax;

    nTimeSeries += 1;

    return true;
}

template <typename T>
CellSpecItrs
TimeSeriesCSVExport<T>::getColumnIters(void) {
    // "-1+1" is to account for the inclusive nature of the upper bound
    //   on range, and the extra column we need to represent time.
    // Therefore, column 0 is for time, not value.
    vector<CellSpec> columns(nTimeSeries + 1);
    CellSpecItrs cellSpecItrs;

    iota(columns.begin(), columns.end(), (CellSpec)0);

    cellSpecItrs.begin = columns.begin();
    cellSpecItrs.end   = columns.end();

    return cellSpecItrs;
}

template <typename T>
CellSpecItrs
TimeSeriesCSVExport<T>::getRowIters(void) {
    int nPeriods;
    CellSpecItrs cellSpecItrs;

    nPeriods = ceil(tMax / tsPeriodLength) + 1;
    vector<CellSpec> periods{nPeriods};
    iota(periods.begin(), periods.end(), (CellSpec)0);

    cellSpecItrs.begin = periods.begin();
    cellSpecItrs.end   = periods.end();

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
    // Really?
    return true;
}

template <typename T>
string
TimeSeriesCSVExport<T>::getRowName(CellSpec rowSpec) {
    return to_string(rowSpec * tsPeriodLength);
}

template <typename T>
string
TimeSeriesCSVExport<T>::getColumnName(CellSpec columnSpec) {
    string timeHeader = string("t");

    if (columnSpec == 0)
        return timeHeader;
    else
        return tsNames[columnSpec - 1];
}

template <typename T>
string
TimeSeriesCSVExport<T>::getCell(CellSpec rowSpec, CellSpec columnSpec) {
    // 'rowSpec' corresponds to time because the first row is time=0.
    // 'columnSpec'-1 corresponds to the index of the TimeSeries being
    //   referenced.
    int period, tsIdx, tsTime0, tsTimeMax;
    T cellVal;
    string empty;

    if (columnSpec == 0)
        return to_string(columnSpec);

    empty     = string("");

    period    = rowSpec;
    tsIdx     = columnSpec - 1;
    tsTime0   = tsTime0s[tsIdx];
    tsTimeMax = tsTimeMaxs[tsIdx];

    if ( (period * tsPeriodLength) < tsTime0   || \
         (period * tsPeriodLength) > tsTimeMax    )
        return empty;

    cellVal  = tsVectors[tsIdx]->at(period);

    return to_string(cellVal);
}

/////////////////////////////////////////
// PyramidTimeSeries CSV Exporter
/////////////////////////////////////////
PyramidTimeSeriesCSVExport/*<T>*/::~PyramidTimeSeriesCSVExport()
{

}

bool
PyramidTimeSeriesCSVExport/*<T>*/::Add(PyramidTimeSeries/*<T>*/ *ptse) {
    // Boilerplate
    return true;
}


CellSpecItrs
PyramidTimeSeriesCSVExport/*<T>*/::getColumnIters(void) {

}

CellSpecItrs
PyramidTimeSeriesCSVExport/*<T>*/::getRowIters(void) {

}

bool
PyramidTimeSeriesCSVExport/*<T>*/::isColumnHeader(void) {
    // Really?
    return true;
}

bool
PyramidTimeSeriesCSVExport/*<T>*/::isRowHeader(void) {
    // Really?
    return true;
}

string
PyramidTimeSeriesCSVExport/*<T>*/::getRowName(CellSpec rowSpec) {

}

string
PyramidTimeSeriesCSVExport/*<T>*/::getColumnName(CellSpec columnSpec) {

}

string
PyramidTimeSeriesCSVExport/*<T>*/::getCell(CellSpec rowSpec, CellSpec columnSpec) {

}


/////////////////////////////////////////
// TimeStatistic CSV Exporter
/////////////////////////////////////////
template<typename T>
TimeStatisticsCSVExport<T>::~TimeStatisticsCSVExport()
{

}

template<typename T>
bool
TimeStatisticsCSVExport<T>::Add(TimeStatistic *tst) {
    // Boilerplate
    return true;
}


template<typename T>
CellSpecItrs
TimeStatisticsCSVExport<T>::getColumnIters(void) {

}

template<typename T>
CellSpecItrs
TimeStatisticsCSVExport<T>::getRowIters(void) {

}

template <typename T>
bool
TimeStatisticsCSVExport<T>::isColumnHeader(void) {
    // Really?
    return true;
}

template <typename T>
bool
TimeStatisticsCSVExport<T>::isRowHeader(void) {
    // Really?
    return true;
}

template<typename T>
string
TimeStatisticsCSVExport<T>::getRowName(CellSpec rowSpec) {

}

template<typename T>
string
TimeStatisticsCSVExport<T>::getColumnName(CellSpec columnSpec) {

}

template<typename T>
string
TimeStatisticsCSVExport<T>::getCell(CellSpec rowSpec, CellSpec columnSpec) {

}


}
