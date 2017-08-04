#pragma once

#include <utility>
#include <string>
#include <vector>

namespace SimulationLib::CSVImport {

// =================
//   Column types
// =================

template <typename T>
using TimeColumn = std::pair< std::string,
                              std::vector<T> >;

template <typename T>
using Column = std::pair< std::string,
                          vector<T>    >;

template <typename TimeT, ValsT>
using Columns = std::pair< TimeColumn<TimeT>
                           vector< Column<ValsT> >
                         >;

// =================
//    Row types
// =================

using HeaderRow = vector<std::string>;

template <typename TimeT, ValsT>
using Row = std::pair<TimeT, vector<ValsT>>;

template <typename TimeT, ValsT>
using Rows = std::pair< HeaderRow,
                        vector< Row<TimeT, ValsT> >
                      >;

// ======================
//  Function definitions
// ======================

// Given a source istream and a variadic number of string-typed arguments
//   corresponding to the name of each column, returns a Columns<TimeT, ValsT>
template <typename TimeT, typename ValsT, typename... Columns>
Columns<TimeT, ValsT>
TimeDataToColumns(std::istream& source, Columns &&... columnNames);

template <typename TimeT, typename ValsT>
Rows<TimeT, ValsT>
TimeDataToRows(std::istream& source);

}
