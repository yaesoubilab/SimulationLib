#pragma once
#include <vector>
#include <memory>
namespace SimulationLib {
  // Marcus: What is 'C' meant to be?
  template<class C>
    class DataFrame {
  public:
    // Marcus: Explain what each of these parameters represents, and what
    //   any constraints upon their values are.
    DataFrame(const char *file, bool loopTime,
	      std::shared_ptr<const C> (*g)(const char *));
    // Only implemented for DataFrame<double>, and for distributions on
    // long, long double, and for parameters.
    DataFrame(const char *file, bool loopTime);

    // Marcus: Explain what this function does and how it handles errors
    std::shared_ptr<const C>
      getValue(double time, bool isFemale, double age) const; // Get a value
    // When's the next bracket start?
    double nextBracketStart(double age) const;
    // In how long?
    double timeToNextBracket(double age) const;
    inline bool ignoresTime() const {
      return(ignoreTime);
    }
    inline bool ignoresPerson() const {
      return(ignoreAge && ignoreGender);
    }
  private:
    double timeBracket, ageBracket, timeStart, ageStart;
    // The data.
    std::vector<std::vector<std::shared_ptr<const C> > > m, f;
    // Marcus: Explain what each of the following variables does
    int timeCats, ageCats;
    bool ignoreAge, ignoreTime, ignoreGender;
    const bool loopTime;
  };
}

#include "DataFrame.t"
