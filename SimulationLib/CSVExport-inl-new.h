#include "CSVExport-inl-new.h"

namespace SimulationLib {

/////////////////////////////////////////
// General CSV Exporter
/////////////////////////////////////////
CSVExport::CSVExport(string _fname) {
    fname = _fname;
}

CSVExport::~CSVExport() {

}

bool
CSVExport::Write(void) {
    int stringSize;
    string buf, comma, newline;
    FILE *fs;
    CellSpecItr columnItr, rowItr;

    buf       = string("");
    comma     = string(",");
    newline   = string("\n");

    columnItr = getColumnIter();
    rowItr    = getRowIter();

    if (columnItr.cbegin() == columnItr.cend()) {
        printf("Error: No columns to write\n");
        return false;
    }

    if (rowItr.cbegin() == rowItr.cend()) {
        printf("Error: No rows to write\n");
        return false;
    }

    if (!(fs = fopen(fname.c_str(), "a"))) {
        printf("Error: fopen() returned NULL pointer\n");
        fclose(fs);
        return;
    }

    // Print column header, if exists
    if (isColumnHeader()) {
        for (CellSpecItr itr = columnItr.cbegin(); \
             itr != columnItr.cend();              \
             itr = next(itr)) {

            // Add comma, if not the first column
            if (itr != columnItr.cbegin())
                buf += comma;

            // Add column name
            buf += getColumnName(*itr);
        }

        buf += newline;
    }

    // For each row
    for (CellSpecItr rItr = rowItr.cbegin(); \
         rItr = rowItr.cend();               \
         rItr = next(rItr)) {

        // Print row header if exists
        if (isRowHeader())
            buf += getRowName(*rItr) + comma;

        // For each column
        for (CellSpecItr cItr = columnItr.cbegin();
             cItr != columnItr.cend();
             cItr = next(cItr)) {

            // Add comma if not the first column
            if (cItr != columnItr.cbegin())
                buf += comma;

            buf += getCell(*rItr, *cItr);
        }

        buf += newline;
    }

    // Perform write
    stringSize = buf.size();
    if (fwrite(buf.c_str(), sizeof(char), stringSize, fs) < stringSize) {
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
TimeSeriesCSVExport<T>::~TimeSeriesCSVExport()
{}

template <typename T>
bool
TimeSeriesCSVExport<T>::Add(TimeSeries<T> *tse) {
    vector<T> *tsPointer;
    int        tsTime0;
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

    if (nTimeSeries != 0 && ts->GetPeriodLength() != periodLength) {
        printf("Error: All TimeSeries must have the same periodLength\n");
        return false;
    } else if (nTimeSeries == 0)
        periodLength = ts->GetPeriodLength();

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
    tsPointer = ts->GetVector();
    tsName    = ts->GetName();
    tsSize    = tsPointer->size();

    tsTime0s.push_back(tsTime0);
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
CellSpecItr
TimeSeriesCSVExport<T>::getColumnIter(void) {
    // "-1+1" is to account for the inclusive nature of the upper bound
    //   on range, and the extra column we need to represent time.
    // Therefore, column 0 is for time, not value.
    return Range<0, nTimeSeries - 1 + 1>{};
}

template <typename T>
CellSpecItr
TimeSeriesCSVExport<T>::getRowIter(void) {
    return Range<0, timeMax>{};
}

template <typename T>
virtual bool
TimeSeriesCSVExport<T>::isColumnHeader(void) {
    // Really?
    return true;
}

template <typename T>
virtual bool
TimeSeriesCSVExport<T>::isRowHeader(void) {
    // Really?
    return true;
}

template <typename T>
string
TimeSeriesCSVExport<T>::getRowName(CellSpec rowSpec) {
    return to_string(rowSpec);
}

template <typename T>
string
TimeSeriesCSVExport<T>::getColumnName(CellSpec columnSpec) {
    string timeHeader = string("t");

    if (columnSpec = 0)
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
    int _time, _tsIdx;

    if (columnSpec == 0)
        return to_string(columnSpec);

    _time  = rowSpec;
    _tsIdx = columnSpec - 1;
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


CellSpecItr
PyramidTimeSeriesCSVExport/*<T>*/::getColumnIter(void) {

}

CellSpecItr
PyramidTimeSeriesCSVExport/*<T>*/::getRowIter(void) {

}

virtual bool
PyramidTimeSeriesCSVExport/*<T>*/::isColumnHeader(void) {
    // Really?
    return true;
}

virtual bool
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
TimeStatisticCSVExport<T>::~TimeStatisticCSVExport()
{

}

template<typename T>
bool
TimeStatisticCSVExport<T>::Add(TimeStatistics<T> *tst) {
    // Boilerplate
    return true;
}


template<typename T>
CellSpecItr
TimeStatisticCSVExport<T>::getColumnIter(void) {

}

template<typename T>
CellSpecItr
TimeStatisticCSVExport<T>::getRowIter(void) {

}

template <typename T>
virtual bool
TimeStatisticCSVExport<T>::isColumnHeader(void) {
    // Really?
    return true;
}

template <typename T>
virtual bool
TimeStatisticCSVExport<T>::isRowHeader(void) {
    // Really?
    return true;
}

template<typename T>
string
TimeStatisticCSVExport<T>::getRowName(CellSpec rowSpec) {

}

template<typename T>
string
TimeStatisticCSVExport<T>::getColumnName(CellSpec columnSpec) {

}

template<typename T>
string
TimeStatisticCSVExport<T>::getCell(CellSpec rowSpec, CellSpec columnSpec) {

}


}
