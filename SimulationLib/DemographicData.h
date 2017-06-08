#pragma once
#include "DataFrame.h"
#include "StatisticalDistribution.h"
#include <string>
#include <map>
#include <memory>

namespace SimulationLib {
  using namespace StatisticalDistributions;
  class DemographicData {
  public:
    DemographicData(char *file);
    double getVar(std::string name) const;
    const DataFrame<double>
    &getFrameVar(std::string name) const;
    const DataFrame<StatisticalDistribution<long> >
    &getFrameDiscrete(std::string name) const;
    const DataFrame<StatisticalDistribution<long double> >
    &getFrameContinuous(std::string name) const;
  private:
    std::map<std::string, double> var;
    std::map<std::string,
      std::shared_ptr<DataFrame<StatisticalDistribution<long> > > > discrete;
    std::map<std::string,
      std::shared_ptr<DataFrame<StatisticalDistribution<long double> > > >
    continuous;
    std::map<std::string, std::shared_ptr<DataFrame<double> > > framevar;
  };
}
