#include "../include/SimulationLib/Calibrate.h"

#include <fstream>

#include "Likelihood.h"
#include "Likelihood-adaptors.h"
#include "Likelihood-sums.h"

template<typename TimeT, typename ValsT, typename... Args>
CSVImport::Columns<TimeT, ValsT>
DataToColumns(string fname, Args &&... columnNames)
{
    std::ifstream f_stream(fname, std::ios::in);

    return TimeDataToColumns<TimeT, ValsT> \
             (f_stream, std::forward<Args>(columnNames)...);
}

template<typename TimeT, typename ValsT>
vector<IncidenceTimeSeries<ValsT>>
ColumnsToIncidenceTimeSeries(CSVImport::Columns<TimeT, ValsT> columns)
{
    return ;
}

template<typename TimeT, typename ValsT>
vector<PrevalenceTimeSeries<ValsT>>
ColumnsToPrevalenceTimeSeries(CSVImport::Columns<TimeT, ValsT> columns)
{
    return ;
}

template<typename VT, typename PT, typename D>
PrT CalculateLikelihood(TimeSeries<VT> &model,
                        TimeSeries<VT> &data,
                        LikelihoodFunction<D, VT(double)>::DistributionGenerator dg)
{
    LikelihoodFunction<D, VT(double)> LfGen =
      LikelihoodOnTimeSeries(model, dg);

    auto L = LfGen.GetLikelihoodFunction();

    // create vector of t
    // create curriedprobabilityong
    // probabilitylgsum on the result of that last call, and on the 'vector of t'
    // return that

    return ;
}
