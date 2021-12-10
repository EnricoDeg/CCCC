# Find MPI library
find_package(MPI REQUIRED COMPONENTS Fortran)
find_package(MPI REQUIRED COMPONENTS CXX)
find_package(PkgConfig REQUIRED)

# Find yaxt library
#find_package (YAXT REQUIRED)
#find_package (YAXT_C REQUIRED)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/CCCC)

subdirlist(directories ${CMAKE_CURRENT_SOURCE_DIR})
foreach(d IN LISTS directories)
  execute_process(
    COMMAND
    ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/${d} ${CMAKE_BINARY_DIR}/CCCC/${d})
endforeach(d)
unset(directories)

include_directories(${MPI_INCLUDE_PATH})
include_directories(${YAXT_INCLUDES})
include_directories(${YAXT_C_INCLUDES})
include_directories(${CMAKE_BINARY_DIR})
include_directories(${MPI_Fortran_INCLUDE_PATH})
add_compile_options(${MPI_Fortran_COMPILE_FLAGS})

set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/modules)

set(SOURCE_EXE  yaxt/Yaxt.cpp
                core/CCCC.cpp
                wrappers/CCCC_capi.cpp
                wrappers/CCCC_mod.f90
                )

add_library(cccc SHARED ${SOURCE_EXE})
target_link_libraries(cccc ${MPI_LIBRARIES} ${YAXT_LIB} ${YAXT_C_LIB})

add_executable(testf test.f90)
set_property(TARGET testf PROPERTY LINKER_LANGUAGE Fortran)
target_link_libraries (testf cccc ${MPI_Fortran_LIBRARIES})

add_executable(testcxx test.cpp)
target_link_libraries (testcxx cccc ${MPI_LIBRARIES})

# Install the library
install (
  TARGETS cccc
  LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/lib
  ARCHIVE DESTINATION ${PROJECT_SOURCE_DIR}/lib)

# Install the library Fortran module
install(DIRECTORY ${CMAKE_Fortran_MODULE_DIRECTORY} 
        DESTINATION ${PROJECT_SOURCE_DIR})

# Install executables.
install (TARGETS
  testf testcxx # executables
  RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)

# ------------ Header Files	
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/
  DESTINATION ${PROJECT_SOURCE_DIR}/include/CCCC
  FILES_MATCHING
  PATTERN "*.hpp"
  )




