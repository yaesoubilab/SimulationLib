CC=gcc
CXX=c++
RM=rm -f -r
CPPFLAGS=-g -std=c++1z -Wall -Wextra -pedantic

SRCS_TESTS=tests-DiscreteTimeStatistic.cpp \
           tests-PrevalenceTimeSeries.cpp \
           tests-PyramidData.cpp \
           tests-PyramidTimeSeries.cpp \
           tests-CSVExport.cpp
OBJS_TESTS=$(subst .cpp,.o,$(SRCS_TESTS))

SRCS_TEST_DEPENDENCIES= ../DiscreteTimeStatistic.cpp \
                        ../PrevalenceTimeSeries.cpp \
                        ../PyramidData.cpp \
                        ../PyramidTimeSeries.cpp \
                        ../PrevalencePyramidTimeSeries.cpp \
                        ../IncidencePyramidTimeSeries.cpp \
                        ../CSVExport.cpp
OBJS_TEST_DEPENDENCIES=$(subst .cpp,.o,$(SRCS_TEST_DEPENDENCIES))

test: testRunner
	$(CXX) $(CPPFLAGS) tests-main.o $(SRCS_TESTS) $(SRCS_TEST_DEPENDENCIES) -o tests

testRunner:
	$(CXX) tests-main.cpp -c

clean: cleanCSV
	$(RM) $(OBJS_TESTS) $(OBJS_TEST_DEPENDENCIES) tests-main.o tests *.dSYM

cleanCSV:
	$(RM) test-*-out.csv
