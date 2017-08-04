#include "Config.h"
#include "ReadParam.h"
#include <cstdio>
#include <iostream>
#include <filesystem>

// Marcus: Explain what these macros are used for
// Eyal: Stringification.
// This allows me to say "fdaso" ST(BAR) "adf", and have it turn into
// "fdaso" "12" "adf" in the preprocessor (if BAR expands to 12.)
// This is a hack, but it's the easiest way to write a format string
// that depends on a macro.
#define ST(x) XST(x)
#define XST(x) #x

namespace SimulationLib {
  // Marcus: explain what this constant is and why it is set to 255
  // Eyal: Buffer size - 1. 255 because it's a nice number.
#ifndef BFSZ
#define BFSZ 255
#endif
  using namespace StatisticalDistributions;
  using namespace std;
  Config::Config(const char *file) {
    FILE *ifile = fopen(file, "r");
    std::filesystem::path fpath(file);
    int c;
    char buf[BFSZ + 1], t;
    // Skip the first two lines.
    while((c = getc(ifile)) != EOF && c != '\n');
    while((c = getc(ifile)) != EOF && c != '\n');
    while(!feof(ifile)) {
      // Marcus: explain what the following line does
      // Eyal: Scans a line in a neat way.
      // So if BFSZ expands to 255,
      // this is fscanf(ifile, "%*[^,],%255[^,]%*[^,],%c", buf, &t),
      // So:
      // %*[^,], -- Skip every character until you hit a comma, skip that too.
      // %255[^,] -- Take at most 255 non-commas, and store them in buf.
      // %*[^,], -- Same as before.
      // %c, -- Take a character, store it in t. Then delete the next comma.
      // That's it.
      fscanf(ifile, "%*[^,],%" ST(BFSZ) "[^,]%*[^,],%c,", buf, &t);
      std::string item = buf;
      bool loop = false;
      double v;
      // Marcus: An example of a switch statement for which I would like the indentation
      //   more in keeping with other the style of other nested C++ syntax
      // Eyal: I line up case, labels, and public/private one tab to the left
      // of everything else (Emacs default settings.)
      switch(t) {
      case 'v': // Variable
	fgets(buf, sizeof(buf), ifile);
	var[item] = paramFromString(buf);
	break;
      case 'F': // A data frame that loops.
	loop = true;
      case 'f': // A data frame that doesn't.	
	fscanf(ifile, "%" ST(BFSZ) "[^,]", buf);
	var[item]
	  = make_shared<DataFrame<Parameter> >((fpath / buf).c_str(), loop);
	break;
      default:
	std::cerr << "Unknown type character " << t << std::endl;
      }
      // Explain why the following while loop exists
      // Explanation:
      // So that if there's any extraneous stuff after what we want,
      // it's ignored. It eats characters until the next newline.
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
      cerr << "Variable not found: " << '"' << name << '"' << std::endl;
      // If that fails, print a diagnostic, and rethrow.
      throw(e);
    }
  }
}
