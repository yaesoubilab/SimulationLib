CC=gcc
CXX=c++
RM=rm -f -r
CPPFLAGS=-g -std=c++1z -Wall -Wextra -pedantic

SRCS_SBDSIM=run-SerialBirthDeathSim.cpp SerialBirthDeathSim.cpp ../SimulationLib/ContinuousTimeStatistic.cpp ../SimulationLib/DiscreteTimeStatistic.cpp ../SimulationLib/IncidenceTimeSeries.cpp ../SimulationLib/PrevalenceTimeSeries.cpp ../../StatisticalDistributionsLib/StatisticalDistributionsLib/Binomial.cpp ../SimulationLib/CSVExport.cpp
OBJS_SBDSIM=$(subst .cpp,.o,$(SRCS_SBDSIM))

SRCS_PBDSIM=run-ParallelBirthDeathSim.cpp ParallelBirthDeathSim.cpp ../SimulationLib/ContinuousTimeStatistic.cpp ../SimulationLib/DiscreteTimeStatistic.cpp ../SimulationLib/IncidenceTimeSeries.cpp ../SimulationLib/PrevalenceTimeSeries.cpp ../../StatisticalDistributionsLib/StatisticalDistributionsLib/Binomial.cpp ../SimulationLib/CSVExport.cpp
OBJS_PBDSIM=$(subst .cpp,.o,$(SRCS_PBDSIM))

all: SerialBirthDeathSim ParallelBirthDeathSim

SerialBirthDeathSim: $(OBJS_SBDSIM)
	$(CXX) $(LDFLAGS) -o SerialBirthDeathSim $(OBJS_SBDSIM)

ParallelBirthDeathSim: $(OBJS_PBDSIM)
	$(CXX) $(LDFLAGS) -o ParallelBirthDeathSim $(OBJS_PBDSIM)

clean: cleanCSV
	$(RM) $(OBJS_SBDSIM) $(OBJS_PBDSIM) ParallelBirthDeathSim SerialBirthDeathSim *.dSYM

cleanCSV:
	$(RM) *.csv
