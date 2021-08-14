project(
  util_tests
  VERSION 0.1
  DESCRIPTION "Unit tests powered by C++20 macro-free test framework by Kris Jusiak"
  LANGUAGES CXX)

# Include modern C++20 macro free unit testing suite by Kris Jusiak in boost-ext library
FetchContent_Declare(
  ut
  GIT_REPOSITORY  https://github.com/boost-ext/ut.git
  GIT_TAG master
)

FetchContent_MakeAvailable(ut)

add_executable(${PROJECT_NAME} src/TimeSplitter_CompileTimeSuite.cpp
							   src/main.cpp
							    "include/TestUtilities.hpp")

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
# Temporary fix, boost.ut as a module may work only if it's installed,
# and not with FetchContent?
target_compile_definitions(${PROJECT_NAME} PRIVATE BOOST_UT_DISABLE_MODULE)

target_link_libraries(${PROJECT_NAME} PRIVATE util ut)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

add_test(${PROJECT_NAME} ${PROJECT_NAME})