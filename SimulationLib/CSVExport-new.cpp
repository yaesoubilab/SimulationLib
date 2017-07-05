#include "CSVExport-new-inl.h"

namespace SimulationLib
{
    template class CSVExport<int>;
    template class CSVExport<double>;
    template class TimeSeriesCSVExport<int>;
    template class TimeSeriesCSVExport<double>;
    // template class TimeStatisticsCSVExport<int>;
    // template class TimeStatisticsCSVExport<double>;
    // template class PyramidTimeSeriesCSVExport<int>;
    // Add more supported types here...
}
