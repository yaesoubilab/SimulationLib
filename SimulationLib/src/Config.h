#pragma once
#include "DataFrame.h"
#include "StatisticalDistribution.h"
#include <string>
#include <map>
#include <memory>
#include "Parameter.h"

// To be used as follows:
// Input format (contents of file) is two lines of whatever,
// then lines of the form LONGNAME,SHORTNAME,v,PARAMETER
// or LONGNAME,SHORTNAME,[fF],FILE.
// LONGNAME is ignored; SHORTNAME is the short name of the parameter.
// FILE is a filename to get a data frame from.
// Data-frame file formats are one line of whatever,
// followed by a line of the format TIMEBSIZE,USEGENDER,AGEBSIZE.*
// then a line of the format TIMEBCOUNT,,AGEBCOUNT.*
// TIMEBSIZE is either a number (the size of the time brackets),
// or a hyphen (the time doesn't matter, ignore the first column).
// Similarly for AGEBSIZE, except the third column.
// USEGENDER is an optional hyphen;
// if it is there, the second column is ignored.
// TIMEBCOUNT and AGEBCOUNT are the bracket counts.
// Then there are lines of the form TIME,GENDER,AGE,PARAMETER.
// TIME is the beginning of the time bracket here; similarly for AGE.
// GENDER is either M or F.
// PARAMETER is a complicated format.
// It is either a number, or of the format
// DISTNAME,REMINDER,PAR1,PAR2,PAR3,PAR4,CALINC,SCOPE,MIN,MAX.
// REMINDER is ignored.
// PAR[1-4] are parameters to the distribution.
// CALINC is a boolean marker describing whether
// this parameter should be included in calibration.
// SCOPE I do not understand.
// MIN and MAX are used for a different sort of calibration.
// That's it.

// Call SimulationLib::Config::Config() with the name of the file to find
// configuration data in.
// Call getVar() to pull out by short name.

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
