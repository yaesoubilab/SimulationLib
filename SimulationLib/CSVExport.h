#pragma once
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include "TimeSeries.h"

using namespace std;

// This class uses Drake's `-inl.h` pattern.  When seeing linker errors from
// this class, please refer to http://drake.mit.edu/cxx_inl.html.
//
// Instantiated templates for the following kinds of T's are provided:
// - double
// - int
//
// They are already available to link against in the containing library.

namespace SimulationLib {
    template <typename T>
    class CSVExport
    {
    public:
        CSVExport(string fname);
        // ~CSVExport();

        void AddTimeSeries(TimeSeries<T> *ts);
        void Write(void);

    private:
        vector<vector<T> *> tsVectors;
        vector<double>      tsTime0s;
        vector<string>      tsNames;
        vector<long>        tsSizes;

        string fname;

        int tMax;
        int nTimeSeries;
    };
}
