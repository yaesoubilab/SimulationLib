CC=gcc
CXX=c++
RM=rm -f
CPPFLAGS=-g -std=c++11

SRCS_BDSIM=test-BirthDeathSim.cpp BirthDeathSim.cpp ../SimulationLib/ContinuousTimeStatistic.cpp ../SimulationLib/DiscreteTimeStatistic.cpp ../SimulationLib/IncidenceTimeSeries.cpp ../SimulationLib/PrevalenceTimeSeries.cpp ../../StatisticalDistributionsLib/StatisticalDistributionsLib/Binomial.cpp ../SimulationLib/CSVExport.cpp
OBJS_BDSIM=$(subst .cpp,.o,$(SRCS_BDSIM))

BirthDeathSim: $(OBJS_BDSIM)
	$(CXX) $(LDFLAGS) -o BirthDeathSim $(OBJS_BDSIM)

clean:
	$(RM) $(OBJS_BDSIM) BirthDeathSim BirthDeathSim-output.csv
