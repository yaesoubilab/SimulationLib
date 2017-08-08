#pragma once

#include "CSVImport.h"

#include "IncidenceTimeSeries.h"
#include "PrevalenceTimeSeries.h"

namespace SimulationLib {

// DataToColumns
//     string filename -> Columns
//     [takes a filename, converts it to an istream, passes istream to
//      TimeDataToColumns, returns the result]

//     ^ write the code that does this, comment it out, and instead return
//         some BS Columns type that has mock data inside of it!
template<typename TimeT, typename ValsT, typename... Args>
CSVImport::Columns<TimeT, ValsT>
DataToColumns(string fname, Args &&... columnNames)

// ColumnsToIncidenceTimeSeries
//     Columns -> vector<IncidenceTimeSeries>
//     [takes a Columns and returns an array of IncidenceTimeSeries. Name
//      of each TimeSeries should be the name of the column header.
//      Try to set all parameters of IncidenceTimeSeries correctly etc.
//      Note that a Columns includes a TimeColumn which should be used to call
//      each IncidenceTimeSeries' .Record() function. This function should
//      be templated on the type parameters of Columns]
template<typename TimeT, typename ValsT>
vector<IncidenceTimeSeries<ValsT>>
ColumnsToIncidenceTimeSeries(CSVImport::Columns<TimeT, ValsT> columns);

// ColumnsToPrevalenceTimeSeries
//     Columns -> PrevalenceTimeSeries
//     [same deal as above but there are probably some minor differences going
//      on here, be careful!]
template<typename TimeT, typename ValsT>
vector<PrevalenceTimeSeries<ValsT>>
ColumnsToPrevalenceTimeSeries(CSVImport::Columns<TimeT, ValsT> columns);

// CalculateLikelihood:
//     TimeSeries model, TimeSeries data -> product of likelihoods (using
//                                            Likelihood-sums.h)
template<typename ValsT, typename PrT>
PrT CalculateLikelihood(TimeSeries<ValsT> &model, /*distribution generator, */TimeSeries<ValsT> &data);

// Bonus function

}
