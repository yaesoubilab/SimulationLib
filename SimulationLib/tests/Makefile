CC=gcc
CXX=c++
RM=rm -f -r
CPPFLAGS=-g -std=c++11

SRCS_TESTS=tests-DiscreteTimeStatistic.cpp
OBJS_TESTS=$(subst .cpp,.o,$(SRCS_TESTS))

test: testRunner
	$(CXX) tests-main.o $(SRCS_TESTS) -o tests

testRunner:
	$(CXX) tests-main.cpp -c

clean:
	$(RM) $(OBJS_TESTS) RunTests *.dSym
