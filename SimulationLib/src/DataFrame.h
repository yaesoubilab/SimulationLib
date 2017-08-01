#pragma once
#include <vector>
#include <memory>
#define ACCESSOR(t, x, y) inline t x() const { return(y); }
namespace SimulationLib {
  template<class C>
    class DataFrame {
  public:
    DataFrame(const char *file, bool loopTime,
	      std::shared_ptr<const C> (*g)(const char *));
    // Only implemented for DataFrame<double>, and for distributions on
    // long, long double, and for parameters.
    DataFrame(const char *file, bool loopTime);
    std::shared_ptr<const C>
      getValue(double time, bool isFemale, double age) const; // Get a value
    // When's the next bracket start?
    double nextBracketStart(double age) const;
    // In how long?
    double timeToNextBracket(double age) const;
    ACCESSOR(bool, ignoresTime, ignoreTime);
    ACCESSOR(bool, ignoresPerson, ignoreAge && ignoreGender);
    ACCESSOR(bool, ignoresAge, ignoreAge);
    ACCESSOR(bool, ignoresGender, ignoreGender);
    ACCESSOR(double, timeBracketSize, timeBracket);
    ACCESSOR(double, ageBracketSize, ageBracket);
    ACCESSOR(double, timeBracketStart, timeStart);
    ACCESSOR(double, ageBracketStart, ageStart);
    ACCESSOR(int, timeBracketCount, timeCats);
    ACCESSOR(int, ageBracketCount, ageCats);
    const bool loopTime;
  private:
    double timeBracket, ageBracket, timeStart, ageStart;
    // The data.
    std::vector<std::vector<std::shared_ptr<const C> > > m, f;
    int timeCats, ageCats;
    bool ignoreAge, ignoreTime, ignoreGender;
  };
}
#undef ACCESSOR

#include "DataFrame.t"
