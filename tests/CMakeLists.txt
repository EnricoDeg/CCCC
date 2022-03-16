# Find MPI library
find_package(MPI REQUIRED COMPONENTS Fortran)
find_package(MPI REQUIRED COMPONENTS CXX)
find_package(PkgConfig REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG release-1.11.0
  SOURCE_DIR        "${CMAKE_BINARY_DIR}/external/googletest-src"
  BINARY_DIR        "${CMAKE_BINARY_DIR}/external/googletest-build"
)
FetchContent_GetProperties(googletest)
#googletest_POPULATED
#googletest_SOURCE_DIR
#googletest_BUILD_DIR
if (NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

#enable_testing()

include_directories(${MPI_INCLUDE_PATH})
include_directories(${YAXT_INCLUDES})
include_directories(${YAXT_C_INCLUDES})
include_directories(${CMAKE_BINARY_DIR})
include_directories("${gtest_SOURCE_DIR}/include")
include_directories("${gmock_SOURCE_DIR}/include")

add_executable(testrunner test.cpp)
add_test(NAME testrunner COMMAND testrunner)
target_link_libraries (testrunner cccc ${MPI_LIBRARIES} gtest_main gmock_main)

#install (TARGETS
#  testrunner # executables
#  RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/tests)

#include(GoogleTest)
#gtest_discover_tests(testrunner)
