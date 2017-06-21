#include "CSVExport.h"

namespace SimulationLib {
    template <typename T>
    CSVExport<T>::CSVExport(string _fname) {
        tsVectors = vector<vector<T> *>();
        tsTime0s  = vector<double>();
        tsNames   = vector<string>();
        tsSize    = vector<long>();

        fname = _fname;

        tMax = 0;
        nTimeSeries = 0;
    }

    template <typename T>
    CSVExport<T>::~CSVExport() {
        delete tsVectors, tsTime0s, tsNames;
    }

    template<typename T>
    void CSVExport<T>::AddTimeSeries(TimeSeries<T> *ts) {
        vector<T> * tsPointer;
        int         tsTime0;
        string      tsName;
        long        tsSize;

        if (!ts) {
            printf("Error: TimeSeries pointer was NULL\n");
            return;
        }

        if (ts->IsWritable) {
            printf("Error: TimeSeries is still writable\n");
            return;
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
        tsPointer = ts->GetVector();
        tsName    = ts->name;
        tsSize    = tsPointer->size();

        tsTime0s.push_back(tsTime0);
        tsVectors.push_back(tsPointer);
        tsNames.push_back(tsName);
        tsSize.push_back(tsSize);

        // Update the largest value of 't' that will be written to CSV
        // file, and the number of TimeSeries subject to export.
        tMax =   (tsTime0 + tsSize - 1) > tMax \
               ? (tsTime0 + tsSize - 1) : tMax;
        nTimeSeries += 1;

        return;
    }

    template<typename T>
    void CSVExport<T>::Write(void) {
        int tMax, stringSize;
        string buf, comma, newline;
        FILE *fs;

        // Have any TimeSeries successfully been added?
        if (tsVectors.size() == 0) {
            printf("Error: No TimeSeries have been added for export\n");
        }

        // Open file descriptor
        if (!(fs = fopen(fname, "a"))) {
            printf("Error: fopen() returned NULL pointer\n");
            return;
        }

        comma   = string(",");
        newline = string("\n");

        // Write header row
        buf = string("t");
        for (int i = 0; i < nTimeSeries; ++i)
            buf += (comma + tsNames[i]);
        buf += newline;


        for (int t = 0; t < tMax + 1; ++t)
        {
            buf += to_string(t);

            T n;
            for (int i = 0; i < nTimeSeries; ++i)
            {
                if (tsTime0s[i] <= t && t <= tsTime0s[i] + tsSize[i] - 1) {
                    n = tsVectors[i]->at(t - tsTime0s[i]);
                    buf += (comma + to_string(n));
                } else
                    buf += comma;
            }
            buf += newline;
        }

        // Perform write
        stringSize = buf.size();
        if (fwrite(c_str(buf), sizeof(CharT), stringSize, fs) < stringSize) {
            printf("Error: std::fwrite returned an error\n");
            return;
        }
    }

}
