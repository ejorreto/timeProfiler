cmake_minimum_required(VERSION 3.5)
project(tProf VERSION 0.1.0 LANGUAGES C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ar)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Produce the Compilation database (compile_commands.json) file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Select the platform-specific clock source
if(ENV STREQUAL RTEMS)
    set(TPROF_CLOCK_SRC src/timeProfiler_clock_rtems.c)
elseif(ENV STREQUAL FREERTOS)
    set(TPROF_CLOCK_SRC src/timeProfiler_clock_freertos.c)
else()
    set(TPROF_CLOCK_SRC src/timeProfiler_clock_posix.c)
endif()

# TEST-specific coverage flags
if(ENV STREQUAL TEST)
    set(COVERAGE_FLAGS "-fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COVERAGE_FLAGS}")
endif()

set(SOURCES src/timeProfiler.c ${TPROF_CLOCK_SRC})

add_library(timeProfiler STATIC ${SOURCES})
target_compile_options(timeProfiler PRIVATE -Wall -Werror)
target_link_libraries(timeProfiler m)
target_include_directories(timeProfiler PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
