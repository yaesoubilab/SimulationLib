#include "../include/SimulationLib/CSVImport.h"
#include "../include/SimulationLib/utils/csv.h"


template <typename TimeT, typename ValsT, typename... Columns>
Columns<TimeT, ValsT>
TimeDataToColumns(std::istream& source, Columns&&... columnNames)
{
    // The number of column names passed
    // The number of columns representing time
    // The number of columns containing values
    const int nColumns      = sizeof...(T);
    const int nTimeColumns  = 1;
    const int nValueColumns = nColumns - nTimeColumns;

    // The time column
    TimeColumn     timeColumn{};

    // The normal columns
    vector<Column> columns(nValueColumns);

    // Will hold the time column of the row being red
    // Will hold the normal columns being read
    TimeT   read_timeColumn;
    ValsT   read_valueColumns[nValueColumns];

    // Create a sequence of integers from [0, nValueColumns) to use
    //   as array offsets when passing array element references to
    //   io::read_header
    std::make_integer_sequence<nValueColumns> valueColumnsOffset;

    // Open the input stream for reading
    io::CSVReader<nColumns> in("data source yay!", source);

    // Read header row
    in.read_header(io::ignore_extra_column, (columnNames)...)

    // Read each row into 'timeColumn' and 'valueColumns[0 - nValueColumns-1]''
    while (in.row(timeColumn, (valueColumns + valueColumnsOffset)...)) {

    }

}

template <typename TimeT, typename ValsT, typename ...Columns>
Rows<TimeT, ValsT>
TimeDataToRows(std::istream& source, Columns&&... columnNames)
{
    const int nColumns = sizeof...(T);

    io::CSVReader<nColumns> in ("data source yay!", source);
}
