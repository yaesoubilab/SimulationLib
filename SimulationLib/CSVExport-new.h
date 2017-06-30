#pragma once

#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <iterator>

#include "TimeSeries.h"
#include "TimeStatistic.h"
#include "utils/Range.h"

using namespace std;

// This class uses Drake's `-inl.h` pattern.  When seeing linker errors from
// this class, please refer to http://drake.mit.edu/cxx_inl.html.
//
// Instantiated templates for the following kinds of T's are provided:
// - TimeStatistic
// - TimeSeries
//
// They are already available to link against in the containing library.

namespace SimulationLib {

    using CellSpec = int;
    using CellSpecItr = vector<CellSpec>::const_iterator

    enum class TimeStatType : uint8_t {
        Sum       = 0 << 1,
        Count     = 0 << 2,
        Mean      = 0 << 3,
        Variance  = 0 << 4,
        Min       = 0 << 5,
        Max       = 0 << 6
    };

    /////////////////////////////////////////
    // General CSV Exporter
    /////////////////////////////////////////
    class CSVExport
    {
    public:
        CSVExport(string fname);
        ~CSVExport();

        // bool Add(ObjType obj);
        bool Write(void);

    private:
        string fname;

        virtual CellSpecItr getColumnIter(void) = 0;
        virtual CellSpecItr getRowIter(void) = 0;

        virtual bool isColumnHeader(void) = 0;
        virtual bool isRowHeader(void) = 0;

        virtual string getRowName(CellSpec rowSpec) = 0;
        virtual string getColumnName(CellSpec columnSpec) = 0;
        virtual string getCell(CellSpec rowSpec, CellSpec columnSpec) = 0;
    };


    /////////////////////////////////////////
    // TimeSeries CSV Exporter
    /////////////////////////////////////////
    template<typename T>
    class TimeSeriesCSVExport<T> : public CSVExport {
    public:
        TimeSeriesCSVExport(string fname) : CSVExport(fname) {
            tsVectors   = vector<vector<T> *>{};
            tsTime0s    = vector<double>{};
            tsNames     = vector<string>{};
            tsSizes     = vector<long>{};

            tMax         = 0;
            nTimeSeries  = 0;
            periodLength = 0;
        };

        ~TimeSeriesCSVExport();

        bool Add(TimeSeries<T> *tse);

    private:
        virtual CellSpecItr getColumnIter(void);
        virtual CellSpecItr getRowIter(void);

        virtual bool isColumnHeader(void);
        virtual bool isRowHeader(void);

        virtual string getRowName(CellSpec rowSpec);
        virtual string getColumnName(CellSpec columnSpec);
        virtual string getCell(CellSpec rowSpec, CellSpec columnSpec);

        vector<vector<T> *> tsVectors;
        vector<double>      tsTime0s;
        vector<string>      tsNames;
        vector<long>        tsSizes;
    };


    /////////////////////////////////////////
    // PyramidTimeSeries CSV Exporter
    /////////////////////////////////////////
    // For now, there are no class templates on PyramidTimeSeries, but that
    //   could change...
    // template <typename T>
    class PyramidTimeSeriesCSVExport/*<T>*/ : public CSVExport {
    public:
        PyramidTimeSeriesCSVExport(string fname) : CSVExport(fname) {

        };

        ~PyramidTimeSeriesCSVExport();

        bool Add(PyramidTimeSeries/*<T>*/ *ptse);

    private:
        virtual CellSpecItr getColumnIter(void);
        virtual CellSpecItr getRowIter(void);

        virtual bool isColumnHeader(void);
        virtual bool isRowHeader(void);

        virtual string getRowName(CellSpec rowSpec);
        virtual string getColumnName(CellSpec columnSpec);
        virtual string getCell(CellSpec rowSpec, CellSpec columnSpec);
    };


    /////////////////////////////////////////
    // TimeStatistic CSV Exporter
    /////////////////////////////////////////
    template<typename T>
    class TimeStatisticsCSVExport<T> : public CSVExport {
    public:
        TimeStatisticsCSVExport(string fname, uint8_t _fieldFlags) \
          : CSVExport(fname) {
            fieldFlags = _fieldFlags;
        };

        ~TimeStatisticsCSVExport();

        bool Add(TimeStatistics<T> *tst);

        auto Flag = [](TimeStatType a) { return static_cast<uint8_t> a; };

    private:
        virtual CellSpecItr getColumnIter(void);
        virtual CellSpecItr getRowIter(void);

        virtual bool isColumnHeader(void);
        virtual bool isRowHeader(void);

        virtual string getRowName(CellSpec rowSpec);
        virtual string getColumnName(CellSpec columnSpec);
        virtual string getCell(CellSpec rowSpec, CellSpec columnSpec);

        uint8_t fieldFlags;
    };
};
