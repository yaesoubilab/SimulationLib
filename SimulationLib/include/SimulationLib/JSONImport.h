#pragma once
#include <string>
#include <memory>
#include <nlohmann/json.hpp>

// JSONImporter
// Uses json.hpp class

namespace SimulationLib {
  using json = nlohmann::json;
  using namespace std;
  namespace JSONImport{
    // read a JSON file
    json fileToJSON(string fname);
    bool validateJSON(json);
  }
}