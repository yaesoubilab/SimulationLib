#include "CSVExport-inl.h"

namespace SimulationLib
{
    template class TimeSeriesExport<int>;
    template class TimeSeriesExport<double>;
    template class PyramidDataExport<int>;
    template class PyramidDataExport<double>;
    class PyramidTimeSeriesExport;
    class TimeStatisticsExport;
    // template class TimeStatisticsCSVExport<int>;
    // template class TimeStatisticsCSVExport<double>;
    // template class PyramidTimeSeriesCSVExport<int>;
    // Add more supported types here...
}
