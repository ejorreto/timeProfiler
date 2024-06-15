cmake_minimum_required(VERSION 3.5)
project(tProf VERSION 0.1.0 LANGUAGES C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ar)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)  

# Produce the Compilation database (compile_commands.json) file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(COVERAGE_FLAGS "-fprofile-arcs -ftest-coverage")


# TODO Group configurations between POSIX, RTEMS and FreeRTOS, as only the selected clock file is different at this point
if(ENV STREQUAL TEST)
  # Unit test environment, linux based

  set(CMAKE_C_FLAGS "-g -O0 -Wall -Werror ${COVERAGE_FLAGS}")
  # Enable the use of doubles in Unity
  ADD_COMPILE_DEFINITIONS(UNITY_INCLUDE_DOUBLE)
  include(CTest)  
  add_subdirectory(library)
  add_subdirectory(external)  
  add_subdirectory(test)  

elseif(ENV STREQUAL RTEMS)
  # RTEMS environment

  add_subdirectory(library)

elseif(ENV STREQUAL FREERTOS)
  # FreeRTOS environment

  add_subdirectory(library)

else()
  # Posix environment

  add_subdirectory(library)

  add_executable(example_posix examples/example_posix.c examples/timeProfiler_statistics_printf.c)
  target_link_libraries(example_posix timeProfiler)

endif()
