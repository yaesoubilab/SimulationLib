CC=gcc
CXX=c++
RM=rm -f -r
CPPFLAGS=-g -std=c++1z

SRCS_BDSIM=run-BirthDeathSim.cpp BirthDeathSim.cpp ../SimulationLib/ContinuousTimeStatistic.cpp ../SimulationLib/DiscreteTimeStatistic.cpp ../SimulationLib/IncidenceTimeSeries.cpp ../SimulationLib/PrevalenceTimeSeries.cpp ../../StatisticalDistributionsLib/StatisticalDistributionsLib/Binomial.cpp ../SimulationLib/CSVExport.cpp
OBJS_BDSIM=$(subst .cpp,.o,$(SRCS_BDSIM))

SRCS_PBDSIM=run-ParallelBirthDeathSim.cpp ParallelBirthDeathSim.cpp ../SimulationLib/ContinuousTimeStatistic.cpp ../SimulationLib/DiscreteTimeStatistic.cpp ../SimulationLib/IncidenceTimeSeries.cpp ../SimulationLib/PrevalenceTimeSeries.cpp ../../StatisticalDistributionsLib/StatisticalDistributionsLib/Binomial.cpp ../SimulationLib/CSVExport.cpp
OBJS_PBDSIM=$(subst .cpp,.o,$(SRCS_PBDSIM))

all: BirthDeathSim ParallelBirthDeathSim

BirthDeathSim: $(OBJS_BDSIM)
	$(CXX) $(LDFLAGS) -o BirthDeathSim $(OBJS_BDSIM)

ParallelBirthDeathSim: $(OBJS_PBDSIM)
	$(CXX) $(LDFLAGS) -o ParallelBirthDeathSim $(OBJS_PBDSIM)

clean: cleanCSV
	$(RM) $(OBJS_BDSIM) $(OBJS_PBDSIM) ParallelBirthDeathSim BirthDeathSim *.dSYM

cleanCSV:
	$(RM) *.csv
