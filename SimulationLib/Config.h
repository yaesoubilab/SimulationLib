#pragma once
#include "DataFrame.h"
#include "StatisticalDistribution.h"
#include <string>
#include <map>
#include <memory>

namespace SimulationLib {
  using namespace StatisticalDistributions;
  class Config {
  public:
    Config(const char *file);
    double getVar(const std::string &name) const;
    const DataFrame<double>
    &getFrameVar(const std::string &name) const;
    const DataFrame<StatisticalDistribution<long> >
    &getFrameDiscrete(const std::string &name) const;
    const DataFrame<StatisticalDistribution<long double> >
    &getFrameContinuous(const std::string &name) const;
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
