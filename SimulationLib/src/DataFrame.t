#include <cmath>
#include <cstdio>
#include <cstdlib>

// Marcus: Explain what this constant is and why it is set at 256
#ifndef DFRAMEBSIZE
#define DFRAMEBSIZE 256
#endif
namespace SimulationLib {
  using namespace std;
  template<class C>
  DataFrame<C>::DataFrame(const char *file, bool loopTime,
			  shared_ptr<const C>(*g)(const char *))
    : loopTime(loopTime) {
    char buf[DFRAMEBSIZE];
    FILE *ifile = fopen(file, "r");
    fscanf(ifile, "%*s"); // Ignore first line.
    // Now we read the line that tells us bracket size.
    int c = getc(ifile);
    if((ignoreTime = (c == '-')))
      timeBracket = 0;
    else {
      ungetc(c, ifile);
      fscanf(ifile, "%lf,", &timeBracket);
    }
    if((ignoreGender = (getc(ifile) == '-')))
      getc(ifile);
    c = getc(ifile);
    if((ignoreAge = (c == '-')))
      ageBracket = 0;
    else {
      ungetc(c, ifile);
      fscanf(ifile, "%lf%*s ", &ageBracket);
    }
    // And the line that tells us bracket count.
    if(ignoreTime) {
      timeCats = 1;
      fscanf(ifile, "%*[^,],,");
    } else
      fscanf(ifile, "%i,,", &timeCats);
    if(ignoreAge)
      ageCats = 1;
    else
      fscanf(ifile, "%i", &ageCats);
    while((c = getc(ifile)) != EOF && c != '\n');

    m.resize(timeCats);
    if(!ignoreGender)
      f.resize(timeCats);
    for(int i = 0; i < timeCats; i++) {
      m[i].resize(ageCats);
      if(!ignoreGender)
	f[i].resize(ageCats);
    }
    // So now we read in the data.
    double timeRead = 0, ageRead = 0;
    shared_ptr<const C> pC;
    bool readFemale = false;
    // A function to read in a line.
    // Marcus: Include more information here about the format of the line 'readLine'
    //   expects to be reading and what it does if unhappy with the line it receives.
    //   Also, please change the indentation so that if- and else-enclosed statements
    //   are indented more deeply than the if/else statements themselves. Please also
    //   change your indentation conventions in all other areas of code in SimulationLib,
    //   including in switch/case statements.
    auto readLine = ([&]() {
      if(ignoreTime)
	fscanf(ifile, "%*[^,],");
      else
	fscanf(ifile, "%lf,", &timeRead);
      if(!ignoreGender)
	readFemale = getc(ifile) == 'F';
      fscanf(ifile, "%*[^,],");
      if(ignoreAge)
	fscanf(ifile, "%*[^,],");
      else
	fscanf(ifile, "%lf,", &ageRead);
      fgets(buf, DFRAMEBSIZE, ifile);
      if(*buf)
	pC = g(buf); // Don't call on a null string.
      while((c = getc(ifile)) != EOF && c != '\n');
      });
    readLine();
    timeStart = timeRead;
    ageStart = ageRead;
    do {
      // Marcus: This is a really complicated line of code which needs at least a
      //   couples lines' worth of explanation. In fact, it would be best if you
      //   split this up into a few statements because code like this takes a long
      //   time for the average developer to understand and therefore maintain
      (readFemale? f : m)[(int)((timeRead - timeStart) / timeBracket)]
	[(int)((ageRead - ageStart) / ageBracket)] = pC;
      // Put it in the right place, then read the next line.
      readLine();
    } while(!feof(ifile)); // If we've hit EOF, then quit.
    fclose(ifile);
  }

  template<class C>
  shared_ptr<const C> DataFrame<C>::getValue(double time, bool isFemale, double age)
    const {
    // Marcus: This is a very good example of code which, though potentially expressive,
    //   is cumbersome to read, error-prone to maintain, and encumbered by overuse
    //   of the C++ templating language. Please read about the 'using' alias syntax,
    //   go through this and other libraries, and create appropriate type aliases so
    //   that types such as "const vector<vector<stared_ptr<const C>>> ref" are replaced
    //   by aliases that more compactly express the values represented by the type specified.
    //   If unfamiliar with aliases, refer p.63 of "Effective Modern C++" by Scott Meyers,
    //   which sits on my desk.
    const vector<vector<shared_ptr<const C> > > &from =
      isFemale && !ignoreGender ? f : m;

    // Marcus: Here I see that you have created a varaible 'timeBrk' so that
    //   later operations can use the stored value without having to contain
    //   the verbose syntax used to define the value of 'timeBrk'. While I think
    //   this is a very good design pattern, it is hamstringed by the fact that
    //   you include no explanation of what 'timeBrk' contains. Include a thorough
    //   explanation here.
    int timeBrk = ignoreTime? 0 : (int)((time - timeStart) / timeBracket);
    if(loopTime) {
      timeBrk %= timeCats;
      if(timeBrk < 0)
	timeBrk += timeCats;
    }
    else if(timeBrk >= timeCats)
      timeBrk = timeCats - 1;
    else if(timeBrk < 0)
      timeBrk = 0;
    int ageBrk = ignoreAge? 0 : (int)((age - ageStart) / ageBracket);
    if(ageBrk < 0)
      ageBrk = 0;
    if(ageBrk >= ageCats)
      ageBrk = ageCats - 1;
    // Basically a huge collection of hacks to get the right values.
    return(from[timeBrk][ageBrk]);
  }

  template<class C>
  double DataFrame<C>::nextBracketStart(double age) const {
    if(ignoreAge || age > ageBracket * (ageCats - 1) + ageStart)
      return(INFINITY); // Reasonably obvious.

    // Marcus: Eyal, returning a constant called INIFINITY whose definition
    //   is not even included in the current file, or in the entire SimulationLib
    //   codebase, is not 'Reasonably obvious'. In fact, it is disrespectful
    //   to other developers reading this code (such as myself) to write such
    //   a statement into the codebase in lieu of a much-needed explanation of
    //   what is going on here. Revise this line, make it clear what the INFINITY
    //   constant is and where it is defined, and explain what return(INFINITY) means.
    //   This line of code would be no more 'obvious' to a Ph.D than it would be to
    //   a B.S. - it's simply unclear what's going on here, no matter how experienced
    //   or talented a software developer one might be. Please make an effort to
    //   consider other places in your code where the attitude communicated
    //   by phrases such as 'Reasonably obvious' is disrespectful or immature,
    //   and replace them with a technical explanation of the code.

    if(age < ageStart)
      return(ageStart + ageBracket);
    return(((int)((age - ageStart) / ageBracket) + 1) * ageBracket + ageStart);
  }

  template<class C>
  double DataFrame<C>::timeToNextBracket(double age) const {
    return(nextBracketStart(age) - age);
  }

// Marcus: Another example of indentation and spacing which needs work
  template<>
  DataFrame<double>::DataFrame(const char *file, bool loopTime)
    : DataFrame(file, loopTime, [](const char *str) {
	double *d = (double *)malloc(sizeof(double));
	*d = atof(str);
	return(shared_ptr<const double>(d));
      }) {}
}

// Marcus: Explain why you have chosen to undefine this constant
#undef DFRAMEBSIZE
