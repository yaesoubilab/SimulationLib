#include "Config.h"
#include <cstdio>
#include <iostream>

#define ST(x) XST(x)
#define XST(x) #x

namespace SimulationLib {
#ifndef BFSZ
#define BFSZ 255
#endif
  using namespace StatisticalDistributions;
  DemographicData::DemographicData(const char *file) {
    FILE *ifile = fopen(file, "r");
    int c;
    char buf[BFSZ + 1], t;
    while((c = getc(ifile)) != EOF && c != '\n');
    while((c = getc(ifile)) != EOF && c != '\n');
    while(!feof(ifile)) {
      fscanf(ifile, "%*[^,],%" ST(BFSZ) "[^,],%c,", buf, &t);
      std::string item = buf;
      bool loop = false;
      double v;
      switch(t) {
      case 'v':
	fscanf(ifile, "%lf", &v);
	var[item] = v;
	break;
      case 'N':
	loop = true;
      case 'n':
	fgets(buf, BFSZ + 1, ifile);
	framevar[item] = make_shared<DataFrame<double> >(buf, loop);
	break;
      case 'D':
	loop = true;
      case 'd':
	fgets(buf, BFSZ + 1, ifile);
	discrete[item]
	  = make_shared<DataFrame<StatisticalDistribution<long> > >(buf, loop);
	break;
      case 'C':
	loop = true;
      case 'c':
	fgets(buf, BFSZ + 1, ifile);
	continuous[item]
	  = make_shared<DataFrame<
	    StatisticalDistribution<long double> > >(buf, loop);
	break;
      default:
	std::cerr << "Unknown type character " << t << std::endl;
      }
      while((c = getc(ifile)) != EOF && c != '\n');
    }
    fclose(ifile);
  }
  double DemographicData::getVar(const std::string &name) const {
    auto ptr = var.find(name);
    if(ptr == var.end()) {
      std::cerr << "Variable not found: " << '"' << name << '"' << std::endl;
      return(0);
    }
    return(ptr->second);
  }
  const DataFrame<double>
  &DemographicData::getFrameVar(const std::string &name) const {
    try {
      return(*framevar.at(name));
    } catch(std::out_of_range e) {
      cerr << "Number frame not found: " << '"' << name << '"' << std::endl;
      throw(e);
    }
  }
  const DataFrame<StatisticalDistribution<long> >
  &DemographicData::getFrameDiscrete(const std::string &name) const {
    try {
      return(*discrete.at(name));
    } catch(std::out_of_range e) {
      cerr << "Discrete distribution frame not found: "
	   << '"' << name << '"' << endl;
      throw(e);
    }
  }
  const DataFrame<StatisticalDistribution<long double> >
  &DemographicData::getFrameContinuous(const std::string &name) const {
    try {
      return(*continuous.at(name));
    } catch(std::out_of_range e) {
      cerr << "Discrete distribution frame not found: "
	   << '"' << name << '"' << endl;
      throw(e);
    }
  }
}
