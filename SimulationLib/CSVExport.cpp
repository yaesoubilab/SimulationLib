#include "CSVExport-inl.h"

namespace SimulationLib
{
    template class CSVExport<double>;
    template class CSVExport<int>;
    template class TimeSeriesCSVExport<int>;
    // Add more supported types here...
}
