# Select the platform-specific clock source
if(TPROF_TARGET STREQUAL RTEMS)
    set(TPROF_CLOCK_SRC src/timeProfiler_clock_rtems.c)
elseif(TPROF_TARGET STREQUAL FREERTOS)
    set(TPROF_CLOCK_SRC src/timeProfiler_clock_freertos.c)
else()
    set(TPROF_CLOCK_SRC src/timeProfiler_clock_posix.c)
endif()

set(SOURCES src/timeProfiler.c ${TPROF_CLOCK_SRC})

add_library(timeProfiler STATIC ${SOURCES})
target_compile_options(timeProfiler PRIVATE -Wall -Werror)
target_link_libraries(timeProfiler m)
target_include_directories(timeProfiler PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
