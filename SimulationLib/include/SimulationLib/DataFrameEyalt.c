#include <cmath>
#include <cstdio>
#include <cstdlib>

// Marcus: Explain what this constant is and why it is set at 256
// Eyal: It's a buffer size. 256 is a nice number for that.
#ifndef DFRAMEBSIZE
#define DFRAMEBSIZE 256
#endif
namespace SimulationLib {
using namespace std;
DataFrame::DataFrame(const char *file, bool loopTime)
  : loopTime(loopTime) {
  char buf[DFRAMEBSIZE];
  FILE *ifile = fopen(file, "r");
  int c;
  while ((c = getc(ifile)) != EOF && c != '\n');  // Ignore first line.
  // Now we read the line that tells us bracket size.
  c = getc(ifile);
  if ((ignoreTime = (c == '-')))
    timeBracket = 0;
  else {
    ungetc(c, ifile);
    fscanf(ifile, "%lf,", &timeBracket);
  }
  if ((ignoreGender = (getc(ifile) == '-')))
    getc(ifile);
  c = getc(ifile);
  if ((ignoreAge = (c == '-')))
    ageBracket = 0;
  else {
    ungetc(c, ifile);
    fscanf(ifile, "%lf%*s ", &ageBracket);
  }
  // And the line that tells us bracket count.
  if (ignoreTime) {
    timeCats = 1;
    fscanf(ifile, "%*[^,],,");
  } else
    fscanf(ifile, "%i,,", &timeCats);
  if (ignoreAge)
    ageCats = 1;
  else
    fscanf(ifile, "%i", &ageCats);
  while ((c = getc(ifile)) != EOF && c != '\n');

  m.resize(timeCats);
  if (!ignoreGender)
    f.resize(timeCats);
  for (int i = 0; i < timeCats; i++) {
    m[i].resize(ageCats);
    if (!ignoreGender)
      f[i].resize(ageCats);
  }
  // So now we read in the data.
  double timeRead = 0, ageRead = 0;
  shared_ptr<const C> pC;
  bool readFemale = false;
  // A function to read in a line.
  // Marcus: Include more information here about the format of the line
  // 'readLine' expects to be reading and what it does if
  // unhappy with the line it receives.
  // Eyal: It expects to see something of the form
  // TIME,GENDER,AGE,STUFF.
  // TIME is optional if ignoreTime is true,
  // similarly with GENDER and AGE.
  // TIME should be a float, as should AGE.
  // Only the first character of GENDER matters,
  // and only whether it's an F.
  // STUFF should be something that g understands.
  // If something goes wrong, currently behavior is undefined.

  // Also, please change the indentation so that if- and else-enclosed
  // statements are indented more deeply than the if/else statements
  // themselves.
  // Eyal: What's going on here is that Emacs is using tabs,
  // and tab stops are every 8 characters.
  // If they aren't for you, that's your headache.

  // Please also change your indentation conventions
  // in all other areas of code in SimulationLib,
  // including in switch/case statements.
  // Eyal: See above.
  auto readLine = ([&]() {
    if (ignoreTime)
      fscanf(ifile, "%*[^,],");
    else
      fscanf(ifile, "%lf,", &timeRead);
    if (!ignoreGender)
      readFemale = getc(ifile) == 'F';
    fscanf(ifile, "%*[^,],");
    if (ignoreAge)
      fscanf(ifile, "%*[^,],");
    else
      fscanf(ifile, "%lf,", &ageRead);
    fgets(buf, DFRAMEBSIZE, ifile);
    if (*buf)
      pC = g(buf); // Don't call on a null string.
    while ((c = getc(ifile)) != EOF && c != '\n');
  });
  readLine();
  timeStart = timeRead;
  ageStart = ageRead;
  do {
    // Marcus: This is a really complicated line of code which needs at leas
    // a couples lines' worth of explanation.
    // In fact, it would be best if you split this up into a few statements
    // because code like this takes a long
    // time for the average developer to understand and therefore maintain

    // Should we put this in the male or female box?
    auto &to = readFemale ? f : m;
    // Which time bracket?
    auto &toTime = to[(int)((timeRead - timeStart) / timeBracket)];
    // And which age bracket?
    toTime[(int)((ageRead - ageStart) / ageBracket)] = pC;
    // Put it in the right place, then read the next line.
    readLine();
  } while (!feof(ifile)); // If we've hit EOF, then quit.
  fclose(ifile);
}

template<class C>
shared_ptr<const C> DataFrame<C>::getValue(double time, bool isFemale, double age)
const {
  auto &from = isFemale && !ignoreGender ? f : m;
  // Marcus: Here I see that you have created a varaible 'timeBrk' so that
  //   later operations can use the stored value without having to contain
  //   the verbose syntax used to define the value of 'timeBrk'. While I think
  //   this is a very good design pattern, it is hamstringed by the fact that
  //   you include no explanation of what 'timeBrk' contains. Include a thorough
  //   explanation here.
  // timeBrk is the bracket that the time we are given is in.
  // timeBracket is the size of these brackets,
  // timeCats is the number thereof,
  // and timeStart is when the first one starts.
  int timeBrk = ignoreTime ? 0 : (int)((time - timeStart) / timeBracket);
  if (loopTime) {
    timeBrk %= timeCats;
    if (timeBrk < 0)
      timeBrk += timeCats;
  }
  else if (timeBrk >= timeCats)
    timeBrk = timeCats - 1;
  else if (timeBrk < 0)
    timeBrk = 0;
  int ageBrk = ignoreAge ? 0 : (int)((age - ageStart) / ageBracket);
  if (ageBrk < 0)
    ageBrk = 0;
  if (ageBrk >= ageCats)
    ageBrk = ageCats - 1;
  // Basically a huge collection of hacks to get the right values.
  return (from[timeBrk][ageBrk]);
}

double DataFrame::nextBracketStart(double age) const {
  if (ignoreAge || age > ageBracket * (ageCats - 1) + ageStart)
    return (INFINITY);
  // There'll never be a new bracket.

  // Marcus: Eyal, returning a constant called INIFINITY whose definition
  //   is not even included in the current file, or in the entire SimulationLib
  //   codebase, is not 'Reasonably obvious'. In fact, it is disrespectful
  //   to other developers reading this code (such as myself) to write such
  //   a statement into the codebase in lieu of a much-needed explanation of
  //   what is going on here. Revise this line, make it clear what the INFINITY
  //   constant is and where it is defined, and explain what return(INFINITY) means.
  // Oh for crying out loud.
  // From the description for <cmath> on cppreference.com:
  // If the implementation supports floating-point infinities,
  // the macro INFINITY expands to constant expression of type float
  // which evaluates to positive or unsigned infinity.
  // Translation: return(INFINITY) means "return positive infinity".
  // What else would it mean, besides "return negative infinity"?

  if (age < ageStart)d
    return (ageStart + ageBracket);
  return (((int)((age - ageStart) / ageBracket) + 1) * ageBracket + ageStart);
}

double DataFrame::timeToNextBracket(double age) const {
  return (nextBracketStart(age) - age);
}
}
// Marcus: Another example of indentation and spacing which needs work
// Eyal: It looks fine to me. Check your tab stops.

// Marcus: Explain why you have chosen to undefine this constant
// Eyal:
#undef DFRAMEBSIZE
