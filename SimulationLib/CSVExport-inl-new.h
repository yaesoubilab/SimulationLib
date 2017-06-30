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
{

}

template <typename T>
bool
TimeSeriesCSVExport<T>::Add(TimeSeries<T> tse) {
    // Boilerplate
    return true;
}

template <typename T>
CellSpecItr
TimeSeriesCSVExport<T>::getColumnIter(void) {

}

template <typename T>
CellSpecItr
TimeSeriesCSVExport<T>::getRowIter(void) {

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

}

template <typename T>
string
TimeSeriesCSVExport<T>::getColumnName(CellSpec columnSpec) {

}

template <typename T>
string
TimeSeriesCSVExport<T>::getCell(CellSpec rowSpec, CellSpec columnSpec) {

}

/////////////////////////////////////////
// PyramidTimeSeries CSV Exporter
/////////////////////////////////////////
PyramidTimeSeriesCSVExport::~PyramidTimeSeriesCSVExport()
{

}

bool
PyramidTimeSeriesCSVExport::Add(PyramidTimeSeries/*<T>*/ ptse) {
    // Boilerplate
    return true;
}


CellSpecItr
PyramidTimeSeriesCSVExport::getColumnIter(void) {

}

CellSpecItr
PyramidTimeSeriesCSVExport::getRowIter(void) {

}

virtual bool
PyramidTimeSeriesCSVExport::isColumnHeader(void) {
    // Really?
    return true;
}

virtual bool
PyramidTimeSeriesCSVExport::isRowHeader(void) {
    // Really?
    return true;
}

string
PyramidTimeSeriesCSVExport::getRowName(CellSpec rowSpec) {

}

string
PyramidTimeSeriesCSVExport::getColumnName(CellSpec columnSpec) {

}

string
PyramidTimeSeriesCSVExport::getCell(CellSpec rowSpec, CellSpec columnSpec) {

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
TimeStatisticCSVExport<T>::Add(TimeStatistics<T> tst) {
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
