#pragma once
#include "Parameter.h"
#include <memory>

// For reading parameters.

namespace SimulationLib {
  extern std::shared_ptr<const Parameter> paramFromString(const char *str);
}
