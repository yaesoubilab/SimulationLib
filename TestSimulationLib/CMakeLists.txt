cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
project(TestSimulationLib)

find_package(SimulationLib REQUIRED)
find_package(StatisticalDistributionsLib REQUIRED)
find_package(Boost REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(SerialBirthDeathSim run-SerialBirthDeathSim.cpp SerialBirthDeathSim.cpp)
add_executable(ParallelBirthDeathSim run-ParallelBirthDeathSim.cpp ParallelBirthDeathSim.cpp)
add_executable(PyramidBirthDeathSim run-PyramidBirthDeathSim.cpp PyramidBirthDeathSim.cpp)

target_link_libraries(SerialBirthDeathSim   PUBLIC SimulationLib StatisticalDistributionsLib)
target_link_libraries(ParallelBirthDeathSim PUBLIC SimulationLib StatisticalDistributionsLib)
target_link_libraries(PyramidBirthDeathSim  PUBLIC SimulationLib StatisticalDistributionsLib)

