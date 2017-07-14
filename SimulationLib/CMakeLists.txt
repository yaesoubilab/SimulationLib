cmake_minimum_required(VERSION 3.3)
project (SimulationLib)

# Require C++14 compilation
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define library version
set(SimulationLib_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(SimulationLib_VERSION_MINOR 2 CACHE STRING "minor version" FORCE)
set(SimulationLib_VERSION ${SimulationLib_VERSION_MAJOR}.${SimulationLib_VERSION_MINOR} CACHE STRING "version" FORCE)

# Define install destinations
set(include_dest "include/SimulationLib-${SimulationLib_VERSION}")
set(main_lib_dest "lib/SimulationLib-${SimulationLib_VERSION}")
set(lib_dest ${main_lib_dest}/${CMAKE_BUILD_TYPE})

# Define options here

# Add subdirectories
add_subdirectory(src)
# add_subdirectory(tests)

install(FILES SimulationLib-config.cmake DESTINATION ${main_lib_dest})
install(EXPORT SimulationLib DESTINATION ${lib_dest})
