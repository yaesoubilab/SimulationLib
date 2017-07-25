# Set headers
set(header_path "${SimulationLib_SOURCE_DIR}/include/SimulationLib")
set(header ${header_path}/ContinuousTimeStatistic.h
           ${header_path}/CSVExport.h
           ${header_path}/DiscreteTimeStatistic.h
           ${header_path}/IncidencePyramidTimeSeries.h
           ${header_path}/IncidenceTimeSeries.h
           ${header_path}/PrevalencePyramidTimeSeries.h
           ${header_path}/PrevalenceTimeSeries.h
           ${header_path}/PyramidData.h
           ${header_path}/PyramidTimeSeries.h
           ${header_path}/TimeSeries.h
           ${header_path}/TimeStatistic.h)

# Require C++14 compilation
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set source files
set(src ContinuousTimeStatistic.cpp
        CSVExport.cpp
        DiscreteTimeStatistic.cpp
        IncidencePyramidTimeSeries.cpp
        IncidenceTimeSeries.cpp
        PrevalencePyramidTimeSeries.cpp
        PrevalenceTimeSeries.cpp
        PyramidData.cpp
        PyramidTimeSeries.cpp)

# Configure config.hpp.in
# configure_file("config.hpp.in" "${CMAKE_CURRENT_BINARY_DIR}/config_impl.hpp")

# Define library target
add_library(SimulationLib ${header} ${src})

target_include_directories(SimulationLib PUBLIC
  $<BUILD_INTERFACE:${SimulationLib_SOURCE_DIR}/include> # for headers when building
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # for config_impl.hpp when building
  $<INSTALL_INTERFACE:${include_dest}> # for client in install mode
  $<INSTALL_INTERFACE:${lib_dest}>) # for config_impl.hpp in install mode

install(TARGETS SimulationLib EXPORT SimulationLib DESTINATION "${lib_dest}")
install(FILES ${header} DESTINATION "${include_dest}")
# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config_impl.hpp DESTINATION "${include_dest}")