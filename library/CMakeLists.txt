cmake_minimum_required(VERSION 3.5)
project(tProf VERSION 0.1.0 LANGUAGES C)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/ar)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)  

# Produce the Compilation database (compile_commands.json) file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# TODO Group configurations between POSIX, RTEMS and FreeRTOS, as only the selected clock file is different at this point
if(ENV STREQUAL TEST)
  # Unit test environment, linux based
  set(COVERAGE_FLAGS "-fprofile-arcs -ftest-coverage")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror ${COVERAGE_FLAGS}")

  set(SOURCES src/timeProfiler.c src/timeProfiler_clock_posix.c)
  
  include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
  
  add_library(timeProfiler STATIC ${SOURCES})
  target_link_libraries(timeProfiler m)
  target_include_directories(timeProfiler PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include) 

elseif(ENV STREQUAL RTEMS)
  # RTEMS environment

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
  set(SOURCES src/timeProfiler.c src/timeProfiler_clock_rtems.c)
  
  include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
  
  add_library(timeProfiler STATIC ${SOURCES})
  target_link_libraries(timeProfiler m)
  target_include_directories(timeProfiler PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

elseif(ENV STREQUAL FREERTOS)
  # FreeRTOS environment

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
  set(SOURCES src/timeProfiler.c src/timeProfiler_clock_freertos.c)
  
  include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
  
  add_library(timeProfiler STATIC ${SOURCES})
  target_link_libraries(timeProfiler m)
  target_include_directories(timeProfiler PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

else()
  # Posix environment

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
  set(SOURCES src/timeProfiler.c src/timeProfiler_clock_posix.c)
  
  include_directories(${CMAKE_CURRENT_LIST_DIR}/include)
  
  add_library(timeProfiler STATIC ${SOURCES})
  target_link_libraries(timeProfiler m)
  target_include_directories(timeProfiler PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

endif()
