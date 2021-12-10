find_package(MPI REQUIRED COMPONENTS Fortran)
find_package(MPI REQUIRED COMPONENTS CXX)
find_package(PkgConfig REQUIRED)


file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/CCCC)

subdirlist(directories ${CMAKE_CURRENT_SOURCE_DIR})
foreach(d IN LISTS directories)
  execute_process(
    COMMAND
    ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/${d} ${CMAKE_BINARY_DIR}/CCCC/${d})
endforeach(d)
unset(directories)

include_directories(${MPI_INCLUDE_PATH})
include_directories(${CMAKE_BINARY_DIR})

set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/modules)

set(SOURCE_EXE  yaxt/Yaxt.cpp
                core/CCCC.cpp
                wrappers/CCCC_capi.cpp
                wrappers/CCCC_mod.f90
                )

add_library(cccc SHARED ${SOURCE_EXE})
target_link_libraries(cccc ${MPI_LIBRARIES})

add_executable(test test.f90)
set_property(TARGET test PROPERTY LINKER_LANGUAGE Fortran)
target_link_libraries (test cccc ${MPI_LIBRARIES})

# Install the library
install (
  TARGETS cccc
  LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib
  ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/lib)

# Install executables.
install (TARGETS
  test # executables
  RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)

# ------------ Header Files	
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/
  DESTINATION ${PROJECT_SOURCE_DIR}/include/CCCC
  FILES_MATCHING
  PATTERN "*.hh"
  )




