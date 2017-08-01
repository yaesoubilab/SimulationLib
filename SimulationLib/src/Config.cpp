#include "Config.h"
#include "ReadParam.h"
#include <cstdio>
#include <iostream>

// Marcus: Explain what these macros are used for
#define ST(x) XST(x)
#define XST(x) #x

namespace SimulationLib {
  // Marcus: explain what this constant is and why it is set to 255
#ifndef BFSZ
#define BFSZ 255
#endif
  using namespace StatisticalDistributions;
  using namespace std;
  Config::Config(const char *file) {
    FILE *ifile = fopen(file, "r");
    int c;
    char buf[BFSZ + 1], t;
    // Skip the first two lines.
    while((c = getc(ifile)) != EOF && c != '\n');
    while((c = getc(ifile)) != EOF && c != '\n');
    while(!feof(ifile)) {
      // Marcus: explain what the following line does
      fscanf(ifile, "%*[^,],%" ST(BFSZ) "[^,],%c,", buf, &t);
      std::string item = buf;
      bool loop = false;
      double v;
      // Marcus: An example of a switch statement for which I would like the indentation
      //   more in keeping with other the style of other nested C++ syntax
      switch(t) {
      case 'v': // Variable
	fgets(buf, sizeof(buf), ifile);
	var[item] = paramFromString(buf);
	break;
      case 'F': // A data frame that loops.
	loop = true;
      case 'f': // A data frame that doesn't.
	fgets(buf, BFSZ + 1, ifile);
	var[item] = make_shared<DataFrame<Parameter> >(buf, loop);
	break;
      default:
	std::cerr << "Unknown type character " << t << std::endl;
      }
      // Explain why the following while loop exists
      while((c = getc(ifile)) != EOF && c != '\n');
      // And kill the rest of the line.
    } // Eesh. Reads a file.
    fclose(ifile);
  }
  std::variant<std::shared_ptr<const Parameter>,
	       std::shared_ptr<const DataFrame<Parameter> > >
  Config::getVar(const std::string &name) const {
    try {
      return(var.at(name)); // Try to return the variable.
    } catch(std::out_of_range e) {
      cerr <<"Variable not found: " << '"' << name << '"' << std::endl;
      // If that fails, print a diagnostic, and rethrow.
      throw(e);
    }
  }
}
