#include <cmath>
#include <cstdio>
#include <cstdlib>
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
    
    double timeRead = 0, ageRead = 0;
    shared_ptr<const C> pC;
    bool readFemale = false;
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
      pC = g(buf);
      });
    readLine();
    timeStart = timeRead;
    ageStart = ageRead;
    do {
      (readFemale? f : m)[(int)((timeRead - timeStart) / timeBracket)]
	[(int)((ageRead - ageStart) / ageBracket)] = pC;
      readLine();
    } while(!feof(ifile));
  }
  
  template<class C>
  const C &DataFrame<C>::getValue(double time, bool isFemale, double age)
    const {
    const vector<vector<shared_ptr<const C> > > &from =
      isFemale && !ignoreGender ? f : m;
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
    return(*from[timeBrk][ageBrk]);
  }

  template<class C>
  double DataFrame<C>::nextBracketStart(double age) const {
    if(ignoreAge || age > ageBracket * (ageCats - 1) + ageStart)
      return(INFINITY);
    if(age < ageStart)
      return(ageStart + ageBracket);
    return(((int)((age - ageStart) / ageBracket) + 1) * ageBracket + ageStart);
  }
  
  template<class C>
  double DataFrame<C>::timeToNextBracket(double age) const {
    return(nextBracketStart(age) - age);
  }

  template<>
  DataFrame<double>::DataFrame(const char *file, bool loopTime)
    : DataFrame(file, loopTime, [](const char *str) {
	double *d = (double *)malloc(sizeof(double));
	*d = atof(str);
	return(shared_ptr<const double>(d));
      }) {}
}
#undef DFRAMEBSIZE