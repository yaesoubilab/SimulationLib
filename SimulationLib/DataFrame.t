#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <map>

namespace SimulationLib {
  using namespace std;
  
  template<class C>
  DataFrame<C>::DataFrame(const char *file, bool loopTime,
			  shared_ptr<const C>(*g)(const char *))
    : loopTime(loopTime) {
    char buf[200];
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

    map<int, map<int, shared_ptr<const C> > > male, female;
    double timeRead = 0, ageRead = 0;
    shared_ptr<const C> pC;
    bool readFemale = false;
    auto readLine = ([&]() {
      if(ignoreTime)
	fscanf(ifile, "%*[^,],");
      else
	fscanf(ifile, "%lf,", &timeRead);
      if(ignoreGender)
	fscanf(ifile, "%*[^,],");
      else
	fscanf(ifile, "%hhi,", (char *)&readFemale);
      if(ignoreAge)
	fscanf(ifile, "%*[^,],");
      else
	fscanf(ifile, "%lf,", &ageRead);
      fgets(buf, 200, ifile);
      pC = g(buf);
      });
    readLine();
    timeStart = timeRead;
    do {
      (readFemale? female : male)[(int)((timeRead - timeStart) / timeBracket)]
	[(int)(ageRead / ageBracket)] = pC;
      readLine();
    } while(!feof(ifile));
    m.resize(timeCats = male.rbegin()->first + 1);
    ageCats = male[0].rbegin()->first + 1;
    if(!ignoreGender)
      f.resize(timeCats);
    for(int i = 0; i < timeCats; i++) {
      m[i].resize(ageCats);
      if(!ignoreGender)
	f[i].resize(ageCats);
      for(int j = 0; j < ageCats; j++) {
	m[i][j] = male[i][j];
	if(!ignoreGender)
	  f[i][j] = female[i][j];
      }
    }
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
    int ageBrk = ignoreAge? 0 : (int)(age / ageBracket);
    if(ageBrk < 0)
      ageBrk = 0;
    if(ageBrk >= ageCats)
      ageBrk = ageCats - 1;
    return(*from[timeBrk][ageBrk]);
  }

  template<class C>
  double DataFrame<C>::nextBracketStart(double age) const {
    if(ignoreAge || age > ageBracket * ageCats)
      return(INFINITY);
    if(age < 0)
      return(0);
    return(((int)(age / ageBracket) + 1) * ageBracket);
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
