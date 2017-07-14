cmake_minimum_required(VERSION 3.3)
project (SimulationLib)

# Require C++14 compilation
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories ("${PROJECT_SOURCE_DIR}/SimulationLib/")
add_subdirectory (SimulationLib)

add_library (ParallelBirthDeathSim TestSimulationLib/ParallelBirthDeathSim.cpp)
add_library (SerialBirthDeathSim TestSimulationLib/SerialBirthDeathSim.cpp)

add_executable (pbdsim TestSimulationLib/run-ParallelBirthDeathSim.cpp)
target_link_libraries(pbdsim ParallelBirthDeathSim
                             ContinuousTimeStatistic
                             CSVExport
                             DiscreteTimeStatistic
                             IncidenceTimeSeries
                             PrevalenceTimeSeries
                             PyramidData
                             PyramidTimeSeries) # this is only here because
                                                # CSVExport contains references
                                                # to it (hopefully in future we
                                                # will break this up)

add_executable (sbdsim TestSimulationLib/run-SerialBirthDeathSim.cpp)
target_link_libraries(sbdsim SerialBirthDeathSim
                             ContinuousTimeStatistic
                             CSVExport
                             DiscreteTimeStatistic
                             IncidenceTimeSeries
                             PrevalenceTimeSeries
                             PyramidData
                             PyramidTimeSeries) # this is only here because
                                                # CSVExport contains references
                                                # to it (hopefully in future we
                                                # will break this up)

install (TARGETS pbdsim sbdsim DESTINATION bin)
