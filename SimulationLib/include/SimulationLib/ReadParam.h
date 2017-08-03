#pragma once
#include "Parameter.h"
#include <memory>

// For reading parameters.

//  Marcus: This is a crucial component
//  of the entire Config system, and thus even if it's a relatively simple header,
//  you need to be very clear and detailed in exactly how it works.
// This is defined in ParamDataFrame.cpp.

namespace SimulationLib {
  extern std::shared_ptr<const Parameter> paramFromString(const char *str);
}
