#pragma once

#include <utility>
#include <string>
#include <vector>

// Disable threading for CSV library
#define CSV_IO_NO_THREAD
#include "../include/SimulationLib/utils/csv.h"

namespace SimulationLib {
namespace CSVImport {

// =================
//   Column types
// =================

using HeaderRow     = std::vector<std::string>;

using Column        = std::vector<std::string>;
using Columns       = std::vector<Column>;
using DataByColumns = std::pair<HeaderRow, Columns>;

// =================
//    Row types
// =================

using Row        = std::vector<std::string>;
using Rows       = std::vector<Row>;
using DataByRows = std::pair<HeaderRow, Rows>;

// ======================
//  Function definitions
// ======================

template <size_t... I, typename... ColumnNames>
DataByColumns
CSVToColumns(std::istream& source,
             std::index_sequence<I...>,
             ColumnNames&&... columnNames)
{
    const int nColumns = sizeof...(ColumnNames);

    // The header row, and the columns
    HeaderRow headerRow{columnNames...};
    Columns columns(nColumns, Column{});

    // Will hold the columns being read row-wise
    std::string readColumns[nColumns];

    // Open the input stream for reading
    io::CSVReader<nColumns> in("data source yay!", source);

    // Read header row
    in.read_header(io::ignore_extra_column, columnNames...);



    // Read each row into 'readColumns[0, nColumns-1]''
    while ( in.read_row(readColumns[I]...) )
        for (int i = 0; i < nColumns; ++i)
            columns[i].emplace_back(readColumns[i]);

    return std::make_pair(headerRow, columns);
}

// Given a source istream and a variadic number of string-typed arguments
//   corresponding to the name of each column, returns a Columns<TimeT, ValsT>
template <typename... ColumnNames>
DataByColumns
CSVToColumns(std::istream& source,
             ColumnNames&&... columnNames)
{
    return CSVToColumns(std::forward<std::istream &>(source),
                        std::index_sequence_for<ColumnNames...>{},
                        std::forward<ColumnNames>(columnNames)...);
}

template <size_t... I, typename... ColumnNames>
DataByRows
_CSVToRows(std::istream& source,
           std::index_sequence<I...>,
           ColumnNames&&... columnNames)
{
    // The number of columns to expect
    const int nColumns = sizeof...(ColumnNames);

    // The header row and the normal rows
    HeaderRow headerRow{columnNames...};
    Rows rows{};

    // Temporary storage for each row-read
    std::string readColumns[nColumns];

    // Open the input stream for reading
    io::CSVReader<nColumns> in("data source yay!", source);

    // Read header row
    in.read_header(io::ignore_extra_column, columnNames...);

    // Read each row into 'readColumns[0, nColumns-1]'
    while ( in.read_row(readColumns[I]...) )
        rows.push_back(Row{readColumns[I]...});

    return std::make_pair(headerRow, rows);
}


template <typename... ColumnNames>
DataByRows
CSVToRows(std::istream& source,
          ColumnNames&&... columnNames)
{
    return _CSVToRows(std::forward<std::istream &>(source),
                      std::index_sequence_for<ColumnNames...>{},
                      std::forward<ColumnNames>(columnNames)...);
}

} // </CSVImport>
} // </SimulationLib>
