#pragma once
#include <vector>
#include <memory>
#ifndef ACCESSORC
#define ACCESSORC(x, y) inline auto x() const { return(y); }
#endif
namespace SimulationLib {
  // Marcus: What is 'C' meant to be?
  // Whatever the data frame holds.
  template<class C>
    class DataFrame {
  public:
    // Marcus: Explain what each of these parameters represents, and what
    //   any constraints upon their values are.
    // file should be a file that exists.
    // loopTime is true if we want the value of time (in getValue)
    // to matter only modulo timeBracket * timeCats.
    // g turns a const char* into a C.
    DataFrame(const char *file, bool loopTime,
	      std::shared_ptr<const C> (*g)(const char *));
    // Only implemented for DataFrame<double>, and for parameters.
    DataFrame(const char *file, bool loopTime);

    // Marcus: Explain what this function does and how it handles errors
    // Eyal: It extracts the value it read out of the input file.
    // So it computes (time-timeStart)/timeBracket,
    // rounds down, and either caps to fit within [0, timeCats)
    // or takes it modulo timeCats (depending on loopTime).
    // Similarly for age.
    // It then indices into m/f.
    // Errors are impossible.
    std::shared_ptr<const C>
      getValue(double time, bool isFemale, double age) const; // Get a value
    // When's the next bracket start?
    double nextBracketStart(double age) const;
    // In how long?
    double timeToNextBracket(double age) const;
    ACCESSORC(ignoresTime, ignoreTime);
    ACCESSORC(ignoresPerson, ignoreAge && ignoreGender);
    ACCESSORC(ignoresAge, ignoreAge);
    ACCESSORC(ignoresGender, ignoreGender);
    ACCESSORC(timeBracketSize, timeBracket);
    ACCESSORC(ageBracketSize, ageBracket);
    ACCESSORC(timeBracketStart, timeStart);
    ACCESSORC(ageBracketStart, ageStart);
    ACCESSORC(timeBracketCount, timeCats);
    ACCESSORC(ageBracketCount, ageCats);
    const bool loopTime;
  private:
    double timeBracket, ageBracket, timeStart, ageStart;
    // The data.
    std::vector<std::vector<std::shared_ptr<const C> > > m, f;
    // Marcus: Explain what each of the following variables does
    // Eyal: timeCats: the number of brackets for time
    // ageCats: ditto age.
    // ignoreAge: does the age of the person matter?
    // ignoreTime: does the year matter?
    // ignoreGender: does the gender of the person matter?
    // loopTime: Is this something that goes on a yearly cycle or something?
    int timeCats, ageCats;
    bool ignoreAge, ignoreTime, ignoreGender;
  };
}
#undef ACCESSOR

#include "DataFrameEyal.t"
