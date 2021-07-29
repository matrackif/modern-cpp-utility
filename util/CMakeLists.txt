# Project name and a few useful settings. Other commands can pick up the results
project(
  util
  VERSION 0.1
  DESCRIPTION "Modern C++ utility library. Currently header only."
  LANGUAGES CXX)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)
# Allow IDE to see header files
file(GLOB_RECURSE UTIL_HEADERS include/*.hpp)
message(STATUS "UTIL_HEADERS: ${UTIL_HEADERS}")
target_sources(${PROJECT_NAME} INTERFACE ${UTIL_HEADERS})
