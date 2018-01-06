#pragma once

#include <utility>
#include <string>
#include <vector>

// Disable threading for CSV library
#define CSV_IO_NO_THREAD
#include "SimulationLib-0.2/csv.h"

namespace SimulationLib {
namespace CSVImport {

// =================
//   Common (to Columns and Rows) types
// =================
using HeaderRow     = std::vector<std::string>;

// =================
//   Column types
// =================
using Column        = std::vector<std::string>;
using Columns       = std::vector<Column>;
using DataByColumns = std::pair<HeaderRow, Columns>;

// =================
//    Row types
// =================
using Row           = std::vector<std::string>;
using Rows          = std::vector<Row>;
using DataByRows    = std::pair<HeaderRow, Rows>;

// ======================
//  Function definitions
// ======================

// This is an overload of CSVToColumns which is called by
//   CSVToColumns(source, columnNames...). It is recommended that the user
//   take advantage of the other overload of CSVToColumns, which is defined
//   after this function overload.
template <size_t... I, typename... ColumnNames>
inline
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
inline
DataByColumns
CSVToColumns(std::istream& source,
             ColumnNames&&... columnNames)
{
    // Generate an index sequence (0, 1, 2, n, etc..) for ColumnNames and pass
    //   it to the 3-"argument" overload of CSVToColumns. This allows easy
    //   association of a particular column in the .csv file with an array index
    return CSVToColumns(std::forward<std::istream &>(source),
                        std::index_sequence_for<ColumnNames...>{},
                        std::forward<ColumnNames>(columnNames)...);
}

// This is an overload of CSVToRows which is called by
//   CSVToRows(source, columnNames...). It is recommended that the user
//   take advantage of the other overload of CSVToRows, which is defined
//   after this function overload.
template <size_t... I, typename... ColumnNames>
inline
DataByRows
CSVToRows(std::istream& source,
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
inline
DataByRows
CSVToRows(std::istream& source,
          ColumnNames&&... columnNames)
{
    // Generate an index sequence (0, 1, 2, n, etc..) for ColumnNames and pass
    //   it to the 3-"argument" overload of CSVToRows. This allows easy
    //   association of a particular column in the .csv file with an array index
    return CSVToRows(std::forward<std::istream &>(source),
                     std::index_sequence_for<ColumnNames...>{},
                     std::forward<ColumnNames>(columnNames)...);
}

// template <typename Distribution>
// inline
// Distribution
// DataToDistribution(DataByRows d, int row)
// {
//     std::string param = d.second[row][3];
//     return CSVToRows(std::forward<std::istream &>(source),
//                      std::index_sequence_for<ColumnNames...>{},
//                      std::forward<ColumnNames>(columnNames)...);
// }
 
} // </CSVImport>
} // </SimulationLib>

// Remove definition in case external code wants threaded csv import
#undef CSV_IO_NO_THREAD
