CC=gcc
CXX=c++
RM=rm -f -r
CPPFLAGS=-g -std=c++1z

SRCS_SBDSIM=run-SerialBirthDeathSim.cpp SerialBirthDeathSim.cpp \
            ../SimulationLib/ContinuousTimeStatistic.cpp \
            ../SimulationLib/DiscreteTimeStatistic.cpp \
            ../SimulationLib/IncidenceTimeSeries.cpp \
            ../SimulationLib/PrevalenceTimeSeries.cpp \
            ../../StatisticalDistributionsLib/StatisticalDistributionsLib/Binomial.cpp \
            ../SimulationLib/CSVExport.cpp
OBJS_SBDSIM=$(subst .cpp,.o,$(SRCS_SBDSIM))

SRCS_PBDSIM=run-ParallelBirthDeathSim.cpp \
            ParallelBirthDeathSim.cpp \
            ../SimulationLib/ContinuousTimeStatistic.cpp \
            ../SimulationLib/DiscreteTimeStatistic.cpp \
            ../SimulationLib/IncidenceTimeSeries.cpp \
            ../SimulationLib/PrevalenceTimeSeries.cpp \
            ../../StatisticalDistributionsLib/StatisticalDistributionsLib/Binomial.cpp \
            ../SimulationLib/CSVExport.cpp
OBJS_PBDSIM=$(subst .cpp,.o,$(SRCS_PBDSIM))

SRCS_PYBDSIM=run-PyramidBirthDeathSim.cpp PyramidBirthDeathSim.cpp \
             ../SimulationLib/CSVExport.cpp \
             ../SimulationLib/ContinuousTimeStatistic.cpp \
             ../SimulationLib/DiscreteTimeStatistic.cpp \
             ../SimulationLib/IncidenceTimeSeries.cpp \
             ../SimulationLib/PrevalenceTimeSeries.cpp \
             ../SimulationLib/IncidencePyramidTimeSeries.cpp \
             ../SimulationLib/PrevalencePyramidTimeSeries.cpp \
             ../SimulationLib/PyramidData.cpp \
             ../../StatisticalDistributionsLib/StatisticalDistributionsLib/Binomial.cpp \
             ../../StatisticalDistributionsLib/StatisticalDistributionsLib/Bernoulli.cpp \
             ../../StatisticalDistributionsLib/StatisticalDistributionsLib/Uniform.cpp \
OBJS_PYBDSIM=$(subst .cpp,.o,$(SRCS_PYBDSIM))

all: SerialBirthDeathSim ParallelBirthDeathSim PyramidBirthDeathSim

SerialBirthDeathSim: $(OBJS_SBDSIM)
	$(CXX) $(LDFLAGS) -o SerialBirthDeathSim $(OBJS_SBDSIM)

ParallelBirthDeathSim: $(OBJS_PBDSIM)
	$(CXX) $(LDFLAGS) -o ParallelBirthDeathSim $(OBJS_PBDSIM)

PyramidBirthDeathSim: $(OBJS_PYBDSIM)
	$(CXX) $(LDFLAGS) -o PyramidBirthDeathSim $(OBJS_PYBDSIM)

clean: cleanCSV
	$(RM) $(OBJS_SBDSIM) $(OBJS_PBDSIM) $(OBJS_PYBDSIM) \
          ParallelBirthDeathSim SerialBirthDeathSim PyramidBirthDeathSim \
          *.dSYM

cleanCSV:
	$(RM) *.csv
