# Works with 3.11 and tested through 3.21
cmake_minimum_required(VERSION 3.14...3.21)

# Project name and a few useful settings. Other commands can pick up the results
project(
  ModernCppUtility
  VERSION 0.1
  DESCRIPTION "Modern C++, (C++20 or higher) library with useful utility functionality, with an emphasis on compile time evaluation"
  LANGUAGES CXX)

include(FetchContent)

# Only do these if this is the main project, and not if it is included through add_subdirectory
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	# Optionally set things like CMAKE_CXX_STANDARD, CMAKE_POSITION_INDEPENDENT_CODE here
	# Let's ensure -std=c++xx instead of -std=g++xx
	set(CMAKE_CXX_EXTENSIONS OFF)
	# Let's nicely support folders in IDEs
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	# Testing only available if this is the main app
	# Note this needs to be done in the main CMakeLists
	# since it calls enable_testing, which must be in the
	# main CMakeLists.
	include(CTest)

	# Docs only available if this is the main app
	find_package(Doxygen)
	if(Doxygen_FOUND)
		add_subdirectory(docs)
	else()
	message(STATUS "Doxygen not found, not building docs")
	endif()
endif()

# The compiled library code is here
add_subdirectory(util)

# Testing only available if this is the main app
# Emergency override MODERN_CPP_UTIL_TESTING provided as well
if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CPP_UTIL_TESTING)
   AND BUILD_TESTING)
	add_subdirectory(util_tests)
endif()
