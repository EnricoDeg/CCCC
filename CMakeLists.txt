cmake_minimum_required(VERSION 3.0)

project(cccc Fortran CXX)
include ("CMake/CCCC_CMake_macros.cmake")
include ("CMake/FindYAXT.cmake")
include ("CMake/FindYAXT_C.cmake")
add_subdirectory(src)
