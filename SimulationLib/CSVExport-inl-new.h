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
    // Implementation for
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

}


CellSpecItr
PyramidTimeSeriesCSVExport::getColumnIter(void) {

}

CellSpecItr
PyramidTimeSeriesCSVExport::getRowIter(void) {

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

}


template<typename T>
CellSpecItr
TimeStatisticCSVExport<T>::getColumnIter(void) {

}

template<typename T>
CellSpecItr
TimeStatisticCSVExport<T>::getRowIter(void) {

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
