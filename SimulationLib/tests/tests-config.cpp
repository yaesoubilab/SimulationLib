#include "catch.hpp"

// Change when needed.
#include "../include/SimulationLib/Config.h"
#include <random>
#include <ctime>
#include <cstdio>
#include <vector>

using namespace std;
using namespace SimulationLib;

TEST_CASE("Config", "[config]") {
  Config c("tests-CSVExport-files/test-config.csv");
  CHECK_THROWS_AS(c.getVar("gibberish"), out_of_range);
  SECTION("ptest") {
    auto p = c.getVar("ptest");
    shared_ptr<const Parameter> q;
    REQUIRE(holds_alternative<decltype(q)>(p));
    q = get<decltype(q)>(p);
    CHECK_FALSE(q->calibrate);
    CHECK(q->min == 1234.567L);
    CHECK(q->max == q->min);
    REQUIRE(holds_alternative<long double>(q->dist));
    CHECK(get<long double>(q->dist) == q->min);
  }
  SECTION("ptest") {
    auto p = c.getVar("ptest2");
    shared_ptr<const Parameter> q;
    REQUIRE(holds_alternative<decltype(q)>(p));
    q = get<decltype(q)>(p);
    CHECK_FALSE(q->calibrate);
    CHECK(q->min == 8.901L);
    CHECK(q->max == q->min);
    REQUIRE(holds_alternative<long double>(q->dist));
    CHECK(get<long double>(q->dist) == q->min);
  }
  SECTION("ftest") {
    auto p = c.getVar("ftest");
    shared_ptr<const DataFrame<Parameter> > q;
    REQUIRE(holds_alternative<decltype(q)>(p));
    q = get<decltype(q)>(p);
    CHECK_FALSE(q->loopTime);
    CHECK(q->ignoresTime());
    CHECK_FALSE(q->ignoresAge());
    CHECK_FALSE(q->ignoresGender());
    CHECK_FALSE(q->ignoresPerson());
    CHECK(q->ageBracketSize() == Approx(1.2));
    CHECK(q->ageBracketCount() == 4);
    CHECK(q->ageBracketStart() == Approx(3));
    for(i = 0; i < 4; i++) {
      for(j = 0; j < 2; j++) {
	shared_ptr<const Parameter> r = q->getValue(0, j, i * 1.2 + 3.6);
	CHECK(holds_alternative<long double>(r->dist));
	if(!holds_alternative<long double>(r->dist))
	  break;
	CHECK_FALSE(r->calibrate);
	CHECK(get<long double>(r->dist) == r->min);
	CHECK(r->min == r->max);
	CHECK(r->min == Approx(j + 1 + i / 10.0));
      }
    }
  }
  SECTION("Ftest") {
    auto p = c.getVar("Ftest");
    shared_ptr<const DataFrame<Parameter> > q;
    REQUIRE(holds_alternative<decltype(q)>(p));
    q = get<decltype(q)>(p);
    CHECK(q->loopTime);
    CHECK_FALSE(q->ignoresTime());
    CHECK(q->ignoresPerson());
    CHECK(q->ignoresAge());
    CHECK(q->ignoresGender());
    CHECK(q->timeBracketSize() == Approx(1.2));
    CHECK(q->timeBracketCount() == 3);
    CHECK(q->timeBracketStart() == Approx(1));
    for(i = -10; i < 10; i++) {
      shared_ptr<const Parameter> r = q->getValue(i * 1.2 + 1.6, false, 0);
      CHECK(holds_alternative<long double>(r->dist));
      if(!holds_alternative<long double>(r->dist))
	break;
      CHECK_FALSE(r->calibrate);
      CHECK(get<long double>(r->dist) == r->min);
      CHECK(r->min == r->max);
      CHECK(r->min == Approx((i + 12) % 3));
    }
  }
}
