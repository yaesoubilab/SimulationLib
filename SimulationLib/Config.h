#pragma once
#include "DataFrame.h"
#include "StatisticalDistribution.h"
#include <string>
#include <map>
#include <memory>
#include "Parameter.h"

namespace SimulationLib {
  using namespace StatisticalDistributions;
  using namespace std;
  class Config {
  public:
    Config(const char *file);
    std::variant<std::shared_ptr<const Parameter>,
		 std::shared_ptr<const DataFrame<Parameter> > >
    getVar(const std::string &name) const;
    // Either gives a parameter, or a data frame thereof.
  private:
    std::map<std::string,
	     std::variant<std::shared_ptr<const Parameter>,
			  std::shared_ptr<const DataFrame<Parameter> > > > var;
    // Holds a map from strings to whatevers.
  };
}
