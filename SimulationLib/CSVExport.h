#pragma once

#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <iterator>
#include <numeric>
#include <utility>

#include "PyramidTimeSeries.h"
#include "TimeSeries.h"
#include "TimeStatistic.h"

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
    using CellSpecItr = vector<CellSpec>::iterator;
    using CellSpecItrs = struct {
        CellSpecItr begin;
        CellSpecItr end;
    };

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
    template <typename T>
    class CSVExport
    {
    public:
        CSVExport(string fname);
        ~CSVExport();

        // bool Add(ObjType obj);
        bool Write(void);

    private:
        string fname;

        CellSpecItrs rowItrs;
        CellSpecItrs columnItrs;

        virtual CellSpecItrs getColumnIters(void) = 0;
        virtual CellSpecItrs getRowIters(void) = 0;

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
    class TimeSeriesCSVExport : public CSVExport<T> {
    public:
        TimeSeriesCSVExport(string fname) : CSVExport<T>(fname) {
            tsVectors      = vector<vector<T> *>{};
            tsTime0s       = vector<double>{};
            tsTimeMaxs     = vector<double>{};
            tsNames        = vector<string>{};
            tsSizes        = vector<long>{};

            tsPeriodLength = 0;
            tMax           = 0;

            nTimeSeries    = 0;
        };

        ~TimeSeriesCSVExport() {
            delete rows;
            delete columns;
        };

        bool Add(TimeSeries<T> *tse);

    private:
        CellSpecItrs getColumnIters(void);
        CellSpecItrs getRowIters(void);

        vector<CellSpec> *rows;
        vector<CellSpec> *columns;

        bool isColumnHeader(void);
        bool isRowHeader(void);

        string getRowName(CellSpec rowSpec);
        string getColumnName(CellSpec columnSpec);
        string getCell(CellSpec rowSpec, CellSpec columnSpec);

        vector<vector<T> *> tsVectors;
        vector<double>      tsTime0s;
        vector<double>      tsTimeMaxs;
        vector<string>      tsNames;
        vector<long>        tsSizes;

        int tsPeriodLength;
        double tMax;

        int nTimeSeries;
    };


    /////////////////////////////////////////
    // PyramidTimeSeries CSV Exporter
    /////////////////////////////////////////
    // For now, there are no class templates on PyramidTimeSeries, but that
    //   could change...
    // template <typename T>
    class PyramidTimeSeriesCSVExport/*<T>*/ : public CSVExport<int> {
    public:
        PyramidTimeSeriesCSVExport(string fname) : CSVExport(fname) {

        };

        ~PyramidTimeSeriesCSVExport();

        bool Add(PyramidTimeSeries/*<T>*/ *ptse);

    private:
        virtual CellSpecItrs getColumnIters(void);
        virtual CellSpecItrs getRowIters(void);

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
    class TimeStatisticsCSVExport : public CSVExport<T> {
    public:
        TimeStatisticsCSVExport(string fname, uint8_t _fieldFlags) \
          : CSVExport<T>(fname) {
            fieldFlags = _fieldFlags;
        };

        ~TimeStatisticsCSVExport();

        bool Add(TimeStatistic *tst);

        function<uint8_t (TimeStatType)> Flag = \
          [](auto a) { return static_cast<uint8_t>(a); };

    private:
        virtual CellSpecItrs getColumnIters(void);
        virtual CellSpecItrs getRowIters(void);

        virtual bool isColumnHeader(void);
        virtual bool isRowHeader(void);

        virtual string getRowName(CellSpec rowSpec);
        virtual string getColumnName(CellSpec columnSpec);
        virtual string getCell(CellSpec rowSpec, CellSpec columnSpec);

        uint8_t fieldFlags;
    };
};
