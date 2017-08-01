#include "catch.hpp"

// Change when needed.
#include "../src/DataFrame.h"
#include <random>
#include <ctime>
#include <cstdio>
#include <vector>

using namespace std;
using namespace SimulationLib;

TEST_CASE("DataFrameA", "[dataframe]") {
  mt19937_64 rng(time());
  uniform_real_distribution<> ud;
  normal_distribution<> nd;
  char *fnam = tmpnam(NULL);
  double m[4][3], f[4][3], tsz, asz, tst, ast;
  FILE *out = fopen(fnam, "w");
  fprintf(out, "t,s,a,v\n");
  tsz = ud(rng) + 1;
  asz = ud(rng) + 1;
  tst = nd(rng);
  ast = nd(rng);
  fprintf(out, "%lf,,%lf,\n", tsz, asz);
  fprintf(out, "4,,3,\n");
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 3; j++) {
      m[i][j] = nd(rng);
      f[i][j] = nd(rng);
      fprintf(out, "%lf,M,%lf,%lf\n", tst + i * tsz, ast + i * asz, m[i][j]);
      fprintf(out, "%lf,F,%lf,%lf\n", tst + i * tsz, ast + i * asz, f[i][j]);
    }
  fclose(out);
  SECTION("noloop") {
    DataFrame<double> df(fnam, false);
    CHECK_FALSE(df.ignoresTime());
    CHECK_FALSE(df.ignoresAge());
    CHECK_FALSE(df.ignoresGender());
    CHECK(df.timeBracketCount() == 4);
    CHECK(df.ageBracketCount() == 3);
    CHECK_FALSE(df.loopTime);
    CHECK(df.timeBracketSize() == Approx(tsz));
    CHECK(df.ageBracketSize() == Approx(asz));
    CHECK(df.timeBracketStart() == Approx(tst));
    CHECK(df.ageBracketStart() == Approx(ast));
    lognormal_distribution<> lnd;
    for(int i = 0; i < 4; i++) {
      double t = tst + tsz * (i + ud(rng));
      bool tsp = i == 0 || i == 3;
      double t1;
      if(i == 0)
	t1 = tst - lnd(rng);
      else if(i == 3)
	t1 = tst + tsz * 4 + lnd(rng);
      for(int j = 0; j < 3; j++) {
	double a = ast + asz * (j + ud(rng));
	bool asp = j != 1;
	double a1;
	if(j == 0)
	  a1 = ast - lnd(rng);
	else if(j == 2)
	  a1 = ast + asz * 3 + lnd(rng);
	CHECK(*df.getValue(t, false, a) == Approx(m[i][j]));
	CHECK(*df.getValue(t, true, a) == Approx(f[i][j]));
	if(tsp) {
	  CHECK(*df.getValue(t1, false, a) == Approx(m[i][j]));
	  CHECK(*df.getValue(t1, true, a) == Approx(f[i][j]));
	}
	if(asp) {
	  CHECK(*df.getValue(t, false, a1) == Approx(m[i][j]));
	  CHECK(*df.getValue(t, true, a1) == Approx(f[i][j]));
	}
	if(asp && tsp) {
	  CHECK(*df.getValue(t1, false, a1) == Approx(m[i][j]));
	  CHECK(*df.getValue(t1, true, a1) == Approx(f[i][j]));
	}
      }
    }
  }
  SECTION("loop") {
    DataFrame<double> df(fnam, true);
    CHECK_FALSE(df.ignoresTime());
    CHECK_FALSE(df.ignoresAge());
    CHECK_FALSE(df.ignoresGender());
    CHECK(df.timeBracketCount() == 4);
    CHECK(df.ageBracketCount() == 3);
    CHECK(df.loopTime);
    CHECK(df.timeBracketSize() == Approx(tsz));
    CHECK(df.ageBracketSize() == Approx(asz));
    CHECK(df.timeBracketStart() == Approx(tst));
    CHECK(df.ageBracketStart() == Approx(ast));
    lognormal_distribution<> lnd;
    for(int i = 0; i < 4; i++) {
      double t = tst + tsz * (i + ud(rng) + (rng() % 65536) * 4);
      for(int j = 0; j < 3; j++) {
	double a = ast + asz * (j + ud(rng));
	bool asp = j != 1;
	double a1;
	if(j == 0)
	  a1 = ast - lnd(rng);
	else if(j == 2)
	  a1 = ast + asz * 3 + lnd(rng);
	CHECK(*df.getValue(t, false, a) == Approx(m[i][j]));
	CHECK(*df.getValue(t, true, a) == Approx(f[i][j]));
	if(asp) {
	  CHECK(*df.getValue(t, false, a1) == Approx(m[i][j]));
	  CHECK(*df.getValue(t, true, a1) == Approx(f[i][j]));
	}
      }
    }
  }
}

TEST_CASE("DataFrameB", "[dataframe]") {
  mt19937_64 rng(time());
  uniform_real_distribution<> ud;
  normal_distribution<> nd;
  char *fnam = tmpnam(NULL);
  double tsz, asz, tst, ast;
  FILE *out = fopen(fnam, "w");
  fprintf(out, "t,s,a,v\n");
  tsz = ud(rng) + 1;
  asz = ud(rng) + 1;
  tst = nd(rng);
  ast = nd(rng);
  fprintf(out, "%lf,-,%lf,\n", tsz, asz);
  fprintf(out, "4,,3,\n");
  for(int i = 0; i < 4; i++)
    for(int j = 0; j < 3; j++)
      fprintf(out, "%lf,M,%lf,%lf\n", tst + i * tsz, ast + i * asz, nd(rng));
  fclose(out);
  DataFrame<double> df(fnam, false);
  CHECK_FALSE(df.ignoresTime());
  CHECK_FALSE(df.ignoresAge());
  CHECK(df.ignoresGender());
  CHECK(df.timeBracketCount() == 4);
  CHECK(df.ageBracketCount() == 3);
  CHECK_FALSE(df.loopTime);
  CHECK(df.timeBracketSize() == Approx(tsz));
  CHECK(df.ageBracketSize() == Approx(asz));
  CHECK(df.timeBracketStart() == Approx(tst));
  CHECK(df.ageBracketStart() == Approx(ast));
  for(int i = 0; i < 4; i++) {
    double t = tst + tsz * (i + ud(rng));
    for(int j = 0; j < 3; j++) {
      double a = ast + asz * (j + ud(rng));
      CHECK(*df.getValue(t, true, a) == *df.getValue(t, false, a));
    }
  }
}

TEST_CASE("DataFrameC", "[dataframe]") {
  mt19937_64 rng(time());
  uniform_real_distribution<> ud;
  normal_distribution<> nd;
  char *fnam = tmpnam(NULL);
  double m[3], f[3], asz, ast;
  FILE *out = fopen(fnam, "w");
  fprintf(out, "t,s,a,v\n");
  asz = ud(rng) + 1;
  ast = nd(rng);
  fprintf(out, "-,,%lf,\n", asz);
  fprintf(out, "-,,3,\n");
  for(int j = 0; j < 3; j++) {
    fprintf(out, "-,M,%lf,%lf\n", ast + i * asz, m[j] = nd(rng));
    fprintf(out, "-,F,%lf,%lf\n", ast + i * asz, f[j] = nd(rng));
  }
  fclose(out);
  DataFrame<double> df(fnam, false);
  CHECK(df.ignoresTime());
  CHECK_FALSE(df.ignoresAge());
  CHECK_FALSE(df.ignoresGender());
  CHECK(df.ageBracketCount() == 3);
  CHECK_FALSE(df.loopTime);
  CHECK(df.ageBracketSize() == Approx(asz));
  CHECK(df.ageBracketStart() == Approx(ast));
  for(int j = 0; j < 3; j++) {
    double a = ast + asz * (j + ud(rng));
    CHECK(*df.getValue(nd(rng), false, a) == Approx(m[j]));
    CHECK(*df.getValue(nd(rng), true, a) == Approx(f[j]));
  }
}

TEST_CASE("DataFrameD", "[dataframe]") {
  mt19937_64 rng(time());
  uniform_real_distribution<> ud;
  normal_distribution<> nd;
  char *fnam = tmpnam(NULL);
  double m[4], f[4], tsz, tst;
  FILE *out = fopen(fnam, "w");
  fprintf(out, "t,s,a,v\n");
  tsz = ud(rng) + 1;
  tst = nd(rng);
  fprintf(out, "%lf,,-,\n", tsz);
  fprintf(out, "4,,-,\n");
  for(int j = 0; j < 3; j++) {
    fprintf(out, "%lf,M,-,%lf\n", tst + i * tsz, m[j] = nd(rng));
    fprintf(out, "%lf,F,-,%lf\n", tst + i * tsz, f[j] = nd(rng));
  }
  fclose(out);
  DataFrame<double> df(fnam, false);
  CHECK_FALSE(df.ignoresTime());
  CHECK(df.ignoresAge());
  CHECK_FALSE(df.ignoresGender());
  CHECK(df.timeBracketCount() == 4);
  CHECK_FALSE(df.loopTime);
  CHECK(df.timeBracketSize() == Approx(tsz));
  CHECK(df.timeBracketStart() == Approx(tst));
  for(int i = 0; i < 4; i++) {
    double t = tst + tsz * (i + ud(rng));
    CHECK(*df.getValue(t, false, nd(rng)) == Approx(m[i]));
    CHECK(*df.getValue(t, true, nd(rng)) == Approx(f[i]));
  }
}
