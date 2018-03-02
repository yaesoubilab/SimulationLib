# Set headers
set(JSON_INCLUDE "-I/usr/local/homebrew/Cellar/nlohmann_json/1.1.0/include")
set(header_path "${SimulationLib_SOURCE_DIR}/include/SimulationLib")
set(header ${header_path}/ContinuousTimeStatistic.h
           ${header_path}/Calibrate.h
           ${header_path}/utils/Bound.h
           ${header_path}/CSVExport.h
           ${header_path}/CSVImport.h
           ${header_path}/DiscreteTimeStatistic.h
           ${header_path}/IncidencePyramidTimeSeries.h
           ${header_path}/IncidenceTimeSeries.h
           ${header_path}/PrevalencePyramidTimeSeries.h
           ${header_path}/PrevalenceTimeSeries.h
           ${header_path}/PyramidData.h
           ${header_path}/PyramidTimeSeries.h
           ${header_path}/TimeSeries.h
           ${header_path}/TimeStatistic.h
           ${header_path}/EventQueue.h
           ${header_path}/JSONImport.h
           ${header_path}/JSONParameterize.h
	   ${header_path}/utils/csv.h
	   ${header_path}/DataFrame.h
	   # ${header_path}/Config.h
	   # ${header_path}/ReadParam.h
	         ${header_path}/Param.h
           ${header_path}/JSONImport.h
           ${header_path}/Likelihood.h
           ${header_path}/Likelihood-adaptors.h
           ${header_path}/Likelihood-sums.h)

# Require C++14 compilation
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED)
find_package(StatisticalDistributionsLib REQUIRED)

# Set source files
set(src ContinuousTimeStatistic.cpp
        CSVExport.cpp
        DiscreteTimeStatistic.cpp
        IncidencePyramidTimeSeries.cpp
        IncidenceTimeSeries.cpp
        JSONImport.cpp
        JSONParameterize.cpp
        PrevalencePyramidTimeSeries.cpp
        PrevalenceTimeSeries.cpp
        PyramidData.cpp
        PyramidTimeSeries.cpp

	# ParamDataFrame.cpp
	# Parameter.cpp
	# Config.cpp
        Param.cpp
        Likelihood.cpp
	     DataFrame.cpp
  )

# Configure config.hpp.in
# configure_file("config.hpp.in" "${CMAKE_CURRENT_BINARY_DIR}/config_impl.hpp")

# Define library target
add_library(SimulationLib ${header} ${src})

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${JSON_INCLUDE})
include_directories(${header_path})

target_link_libraries(SimulationLib PUBLIC StatisticalDistributionsLib)


target_include_directories(SimulationLib PUBLIC
  $<BUILD_INTERFACE:${SimulationLib_SOURCE_DIR}/include> # for headers when building
  # $<BUILD_INTERFACE:${StatisticalDistributionsLib_INCLUDE_DIRS}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}> # for config_impl.hpp when building
  $<INSTALL_INTERFACE:${include_dest}> # for client in install mode
  $<INSTALL_INTERFACE:${lib_dest}>) # for config_impl.hpp in install mode

install(TARGETS SimulationLib EXPORT SimulationLib DESTINATION "${lib_dest}")
install(FILES ${header} DESTINATION "${include_dest}")
# install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config_impl.hpp DESTINATION "${include_dest}")
