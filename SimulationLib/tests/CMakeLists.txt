# Require C++14 compilation
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(StatisticalDistributionsLib REQUIRED)
include_directories(${StatisticalDistributionsLib_INCLUDE_DIRS})

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

include_directories (${TEST_SOURCE_DIR}/src
                     )

add_executable (Test
				tests-main.cpp
				# tests-config.cpp
                tests-Calibrate.cpp
                tests-Bound.cpp
				tests-CSVExport.cpp
                tests-CSVImport.cpp
				tests-DiscreteTimeStatistic.cpp
				tests-PrevalenceTimeSeries.cpp
				tests-PyramidData.cpp
				tests-PyramidTimeSeries.cpp
                tests-EventQueue.cpp
                tests-EventQueueWithState.cpp
		# tests-ReadParam.cpp
		# tests-frame.cpp
                tests-Likelihood.cpp
                tests-JSON_param.cpp)

target_link_libraries (Test
					   SimulationLib
					   Catch
					   StatisticalDistributionsLib)

add_test (NAME MyTest COMMAND Test WORKING_DIRECTORY <${CMAKE_CURRENT_SOURCE_DIR}>)
