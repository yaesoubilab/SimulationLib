#pragma once

#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <iterator>
#include <numeric>
#include <utility>
#include <map>

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
    //   which statistics are to be exported (for the TimeSeriesCSVExporter)
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
        // 'fname' is the file to be written to. Specify extention (probably
        //   .csv). Note that currently, files are appended to: if 'fname'
        //   already exists
        CSVExport(string fname);
        ~CSVExport();

        // Writes the .csv representation of any classes (TimeSeries,
        //   PyramidTimeSeries, TimeStatistics) which have been added to the
        //   exporter out to disk. Returns false if the write fails.
        bool Write(void);

    private:
        string fname;

        CellSpecItrs rowItrs;
        CellSpecItrs columnItrs;

        // The following functions must be implemented by classes inheriting
        //   from CSVExporter (TimeSeriesCSVExport, PyramidTimeSeriesCSVExport,
        //   TimeStatisticsCSVExport). They are called by the ::Write() method
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
    class TimeSeriesCSVExport : public CSVExport<T> {
    public:
        TimeSeriesCSVExport(string fname) : CSVExport<T>(fname) {
            // The following five vectors store information about TimeSeries
            //   to be exported. Index 'i' of any of these vectors stores
            //   information about the 'i'th TimeSeries added to TimeSeriesCSVExport
            //   via ::Add(TimeSeries<T>).
            tsVectors      = vector<vector<T> *>{};
            tsTime0s       = vector<double>{};
            tsTimeMaxs     = vector<double>{};
            tsNames        = vector<string>{};
            tsSizes        = vector<long>{};

            //
            tsPeriodLength = 0;
            tMax           = 0;

            nTimeSeries    = 0;
        };

        ~TimeSeriesCSVExport() {
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
            // ptseVectors      = vector<vector<T> *>{};
            // ptseTime0s       = vector<double>{};
            // ptseTimeMaxs     = vector<double>{};
            categoryNames        = vector<string>{};
            // tsSizes        = vector<long>{};

            ageBreaks = vector<double>{};
            
            ptsePeriodLength = 0;
            tMax             = 0;

            nCategories      = 0;

        };

        ~PyramidTimeSeriesCSVExport();

        bool Add(PyramidTimeSeries/*<T>*/ *ptse);

    private:
        vector<CellSpec> *rows;
        vector<CellSpec> *columns;

        PyramidTimeSeries *PTSptr;

        // vector<vector<int>> data;

        vector<string> categoryNames;
        vector<double> ageBreaks;

        int ptsePeriodLength;
        int time0;
        int tMax;

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
    // TimeStatistic CSV Exporter
    /////////////////////////////////////////
    template<typename T>
    class TimeStatisticsCSVExport : public CSVExport<T> {
    public:
        TimeStatisticsCSVExport(string fname, map<TimeStatType, string> _columns) \
          : CSVExport<T>(fname) {
            columns = _columns;
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

        map<TimeStatType, string> columns;
        vector<TimeStatistic *> stats;
    };
};
