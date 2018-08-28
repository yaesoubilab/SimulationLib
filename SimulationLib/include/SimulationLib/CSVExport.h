#pragma once

#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <iterator>
#include <numeric>
#include <utility>
#include <map>
#include <fstream>

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

    // This enum represents the various statistics that are collected by the
    //   TimeStatistics classes. Later on they will be used to articulate
    //   which statistics are to be exported (for the TimeSeriesExporter)
    enum class TimeStatType { Sum, Count, Mean, Variance, Min, Max };

    /////////////////////////////////////////
    // General CSV Exporter
    /////////////////////////////////////////
    class CSVExportEngine
    {
    public:
        // 'fname' is the file to be written to. Specify extention (probably
        //   .csv).
        CSVExportEngine(string fname);
        ~CSVExportEngine();

        // Writes the .csv representation of any classes (TimeSeries,
        //   PyramidTimeSeries, TimeStatistics) which have been added to the
        //   exporter out to disk. Returns false if the write fails.
        bool Write(void);

    private:
        string fname;

        CellSpecItrs rowItrs;
        CellSpecItrs columnItrs;

        // The following functions must be implemented by classes inheriting
        //   from CSVExporter (TimeSeriesExport, PyramidTimeSeriesExport,
        //   TimeStatisticsExport). They are called by the ::Write() method
        //   of CSVExport in order to iterate over rows and columns and obtain
        //   values of headers and "cells" to be written.

        // Returns iterators for columns and rows so that ::Write() can iterate
        //   over each cell to be written
        virtual CellSpecItrs getColumnIters(void) = 0;
        virtual CellSpecItrs getRowIters(void) = 0;

        // Allows ::Write() to know if a header for rows and/or columns
        //   should be written to the file
        virtual bool isColumnHeader(void) = 0;
        virtual bool isRowHeader(void) = 0;

        // If ::isRowHeader()==true, then allows retrieval of a header
        //   for a particular row, given 'rowSpec'. Analogous case for
        //   ::getColumnName().
        virtual string getRowName(CellSpec rowSpec) = 0;
        virtual string getColumnName(CellSpec columnSpec) = 0;

        // Returns a string representing the value to a 'cell' identified by
        //   the position of the iterators 'rowSpec' and 'columnSpec'.
        //   Cell (0,0) (or equivalent) represents the cell at the first
        //   row and column. Important: iterators do NOT reflect any presence/
        //   absence of row or column headers - they iterate over data cells,
        //   regardless of and headers that may or may not exist.
        virtual string getCell(CellSpec rowSpec, CellSpec columnSpec) = 0;
    };


    /////////////////////////////////////////
    // TimeSeries CSV Exporter
    /////////////////////////////////////////
    template<typename T>
    class TimeSeriesExport : public CSVExportEngine {
    public:
        TimeSeriesExport(string fname) : CSVExportEngine(fname) {
            // The following five vectors store information about TimeSeries
            //   to be exported. Index 'i' of any of these vectors stores
            //   information about the 'i'th TimeSeries added to TimeSeriesExport
            //   via ::Add(TimeSeries<T>).
            tsPtrs         = vector<TimeSeries<T> *>{};
            tsVectors      = vector<vector<T> *>{};
            tsTime0s       = vector<double>{};
            tsTimeMaxs     = vector<double>{};
            tsNames        = vector<string>{};
            tsSizes        = vector<long>{};

            tsPeriodLength = 0;
            tMax           = 0;

            nTimeSeries    = 0;
        };

        ~TimeSeriesExport() {
            delete rows;
            delete columns;
        };

        // Adds a TimeSeries to the list of TimeSeries which will be exported.
        //   The periodLength of this TimeSeries must be equal to that of
        //   any other TimeSeries already ::Add()'ed. Returns false if this
        //   condition is not met, or if 'tse' is =nullptr, or if ::Close()
        //   has not been called on 'tse'.
        bool Add(TimeSeries<T> *tse);

    private:
        // Implementations of virtual methods inherited from CSVExport
        CellSpecItrs getColumnIters(void);
        CellSpecItrs getRowIters(void);

        bool isColumnHeader(void);
        bool isRowHeader(void);

        string getRowName(CellSpec rowSpec);
        string getColumnName(CellSpec columnSpec);
        string getCell(CellSpec rowSpec, CellSpec columnSpec);

        // Class specific variables
        vector<CellSpec> *rows;
        vector<CellSpec> *columns;

        vector<TimeSeries<T> *> tsPtrs;
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
    class PyramidTimeSeriesExport/*<T>*/ : public CSVExportEngine {
    public:
        PyramidTimeSeriesExport(string fname) : CSVExportEngine(fname) {
            ptsePointers     = vector<PyramidTimeSeries *>{};
            ptseTime0s       = vector<int>{};
            ptseTimeMaxs     = vector<int>{};

            ageBreaks = vector<double>{};

            ptsePeriodLength   = 0;
            tMax               = 0;

            nCategories        = 0;
            // nTotalCategories   = 0;
            nPyramidTimeSeries = 0;

        };

        ~PyramidTimeSeriesExport();

        bool Add(PyramidTimeSeries/*<T>*/ *ptse);

    private:
        vector<CellSpec> *rows;
        vector<CellSpec> *columns;

        vector<int>      ptseTime0s;
        vector<int>      ptseTimeMaxs;
        vector<PyramidTimeSeries *> ptsePointers;

        vector<string> categoryNames;
        vector<double> ageBreaks;

        int ptsePeriodLength;
        int tMax;

        int nPyramidTimeSeries;

        // Total categories could be used instead in the future
        // if assumption that all PyramidTimeSeries having
        // same number of categories is no longer desired
        // int nTotalCategories;
        int nCategories;


        // virtual CellSpecItrs getColumnIters(void);
        // virtual CellSpecItrs getRowIters(void);
        CellSpecItrs getColumnIters(void);
        CellSpecItrs getRowIters(void);

        virtual bool isColumnHeader(void);
        virtual bool isRowHeader(void);

        virtual string getRowName(CellSpec rowSpec);
        virtual string getColumnName(CellSpec columnSpec);
        virtual string getCell(CellSpec rowSpec, CellSpec columnSpec);
    };

    /////////////////////////////////////////
    // PyramidData CSV Exporter
    /////////////////////////////////////////
    template <typename T>
    class PyramidDataExport : public CSVExportEngine {
    public:
        PyramidDataExport(string fname) : CSVExportEngine(fname) {
            pds = vector<PyramidData<T> *>{};
            rows = nullptr;
            columns = nullptr;

            n_pds = 0;
            nCategories = 0;
        };

        ~PyramidDataExport() {
            if (rows != nullptr) delete rows;
            if (columns != nullptr) delete columns;
        };

        bool Add(PyramidData<T> *pd);

    private:
        size_t n_pds;
        size_t nCategories;
        vector<double> ageBreaks;
        vector<PyramidData<T> *> pds;

        vector<CellSpec> *rows;
        vector<CellSpec> *columns;

        // Virtual function implementations
        CellSpecItrs getColumnIters(void);
        CellSpecItrs getRowIters(void);

        bool isColumnHeader(void);
        bool isRowHeader(void);

        string getRowName(CellSpec rowSpec);
        string getColumnName(CellSpec columnSpec);
        string getCell(CellSpec rowSpec, CellSpec columnSpec);
    };


    /////////////////////////////////////////
    // TimeStatistic CSV Exporter
    /////////////////////////////////////////
    class TimeStatisticsExport : public CSVExportEngine {
    public:
        TimeStatisticsExport(string fname, map<TimeStatType, string> _columns) \
          : CSVExportEngine(fname) {
            columns = _columns;
            nStats  = 0;
        };

        ~TimeStatisticsExport();

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

        // Class specific variables
        int              nStats;
        vector<CellSpec> *_rows;
        vector<CellSpec> *_columns;

        map<TimeStatType, string> columns;
        vector<TimeStatistic *> stats;
    };
};
