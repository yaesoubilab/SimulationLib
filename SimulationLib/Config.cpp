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
  using namespace std;
  extern shared_ptr<Parameter> paramFromString(const char *);
  Config::Config(const char *file) {
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
	fgets(buf, sizeof(buf), ifile);
	var[item] = paramFromString(buf);
	break;
      case 'F':
	loop = true;
      case 'f':
	fgets(buf, BFSZ + 1, ifile);
	framevar[item] = make_shared<DataFrame<Parameter> >(buf, loop);
	break;
      default:
	std::cerr << "Unknown type character " << t << std::endl;
      }
      while((c = getc(ifile)) != EOF && c != '\n');
    }
    fclose(ifile);
  }
  std::variant<std::shared_ptr<const Parameter>,
	       std::shared_ptr<const DataFrame<Parameter> > >
  Config::getVar(const std::string &name) const {
    try {
      return(var.at(name));
    } catch(std::out_of_range e) {
      cerr <<"Variable not found: " << '"' << name << '"' << std::endl;
      throw(e);
    }
  }
}
