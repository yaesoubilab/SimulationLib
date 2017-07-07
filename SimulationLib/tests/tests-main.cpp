// This file simply injects the Catch testing framework, which then detects
//   all Catch tests available via linkage to other tests-*.cpp files
//   and produces an executable to run all tests

#define CATCH_CONFIG_MAIN
#include "catch.hpp"
