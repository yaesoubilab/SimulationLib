#include "CSVExport-inl-new.h"

namespace SimulationLib
{
    template class TimeSeriesCSVExport<int>;
    template class TimeSeriesCSVExport<double>;
    template class TimeStatisticsCSVExport<int>;
    template class TimeStatisticsCSVExport<double>;
    // template class PyramidTimeSeriesCSVExport<int>;
    // Add more supported types here...
}
