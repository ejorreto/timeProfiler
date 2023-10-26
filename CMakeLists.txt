cmake_minimum_required(VERSION 3.5)
project(tProf VERSION 0.1.0 LANGUAGES C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ar)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)  

# Produce the Compilation database (compile_commands.json) file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(COVERAGE_FLAGS "-fprofile-arcs -ftest-coverage")

if(ENV STREQUAL TEST)

  set(CMAKE_C_FLAGS "-g -O0 -Wall -Werror ${COVERAGE_FLAGS}")
  # Enable the use of doubles in Unity
  ADD_COMPILE_DEFINITIONS(UNITY_INCLUDE_DOUBLE)
  include(CTest)  
  add_subdirectory(external)  
  add_subdirectory(test)  

else()

  set(CMAKE_C_FLAGS "-g -O0 -Wall -Werror")
  set(SOURCES code/src/timeProfiler.c code/src/timeProfiler_clock_posix.c)
  include_directories(${CMAKE_CURRENT_LIST_DIR}/code/include)
  add_library(timeProfiler STATIC ${SOURCES})
  target_include_directories(timeProfiler PUBLIC ${CMAKE_CURRENT_LIST_DIR}/code/include)

endif()
