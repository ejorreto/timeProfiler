cmake_minimum_required(VERSION 3.18)
project(tProf VERSION 0.1.0 LANGUAGES C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ar)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Produce the Compilation database (compile_commands.json) file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Target environment selection. POSIX is the default.
set(TPROF_TARGET "POSIX" CACHE STRING "Target environment for timeProfiler")
set_property(CACHE TPROF_TARGET PROPERTY STRINGS POSIX RTEMS FREERTOS TEST)
set(TPROF_KNOWN_TARGETS POSIX RTEMS FREERTOS TEST)
if(NOT TPROF_TARGET IN_LIST TPROF_KNOWN_TARGETS)
  message(FATAL_ERROR "Invalid TPROF_TARGET='${TPROF_TARGET}'. Allowed: POSIX RTEMS FREERTOS TEST")
endif()

if(TPROF_TARGET STREQUAL TEST)
  # Unit test environment, linux based
  set(COVERAGE_FLAGS "-fprofile-arcs -ftest-coverage")
  set(CMAKE_C_FLAGS "-g -O0 -Wall -Werror ${COVERAGE_FLAGS}")
  # Enable the use of doubles in Unity
  add_compile_definitions(UNITY_INCLUDE_DOUBLE)
  include(CTest)
  add_subdirectory(library)
  include(${CMAKE_SOURCE_DIR}/cmake/TPROFUnityCMockDeps.cmake)
  add_subdirectory(test)

elseif(TPROF_TARGET STREQUAL POSIX)
  add_subdirectory(library)

  add_executable(example_posix examples/example_posix.c examples/example_statistics_printf.c)
  target_link_libraries(example_posix timeProfiler)

else()  # RTEMS or FREERTOS — only the clock file differs (chosen in library/)
  add_subdirectory(library)

endif()

# Doxygen documentation target (optional)
find_package(Doxygen)
if(DOXYGEN_FOUND)
  add_custom_target(docs
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/doc/Doxyfile
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/doc
    COMMENT "Generating Doxygen HTML documentation"
  )
else()
  message(STATUS "Doxygen not found; 'docs' target unavailable")
endif()
