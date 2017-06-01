#pragma once
#include <vector>
#include <memory>
namespace SimulationLib {
  template<class C>
    class DataFrame {
  public:
    DataFrame(const char *file, bool loopTime,
	      std::shared_ptr<const C> (*g)(const char *));
    // Only implemented for DataFrame<double>.
    DataFrame(const char *file, bool loopTime);
    const C &getValue(double time, bool isFemale, double age) const;
    double nextBracketStart(double age) const;
    double timeToNextBracket(double age) const;
  private:
    double timeBracket, ageBracket, timeStart;
    std::vector<std::vector<std::shared_ptr<const C> > > m, f;
    int timeCats, ageCats;
    bool ignoreAge, ignoreTime, ignoreGender;
    const bool loopTime;
  };
}

#include "DataFrame.t"
