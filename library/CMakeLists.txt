# Select the platform-specific clock source
if(TPROF_TARGET STREQUAL RTEMS)
    set(TPROF_CLOCK_SRC src/timeProfiler_clock_rtems.c)
elseif(TPROF_TARGET STREQUAL FREERTOS)
    set(TPROF_CLOCK_SRC src/timeProfiler_clock_freertos.c)
else()
    set(TPROF_CLOCK_SRC src/timeProfiler_clock_posix.c)
endif()

set(SOURCES src/timeProfiler.c ${TPROF_CLOCK_SRC})

include(GNUInstallDirs)

add_library(timeProfiler STATIC ${SOURCES})
target_compile_options(timeProfiler PRIVATE -Wall -Werror)
target_link_libraries(timeProfiler m)

# BUILD_INTERFACE: source-tree headers while building the library or its
# tests in this project. INSTALL_INTERFACE: the installed layout, so that
# exported targets point to <prefix>/include.
target_include_directories(timeProfiler PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Installation is disabled for the TEST configuration: that configuration
# builds a coverage-instrumented, non-optimized library which must never
# be distributed as "the library".
if(NOT TPROF_TARGET STREQUAL TEST)
    install(TARGETS timeProfiler EXPORT timeProfilerTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
    install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/timeProfiler.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(EXPORT timeProfilerTargets
        FILE timeProfilerTargets.cmake
        NAMESPACE timeProfiler::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/timeProfiler
    )

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/timeProfilerConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/timeProfilerConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/timeProfilerConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/timeProfiler
    )
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/timeProfilerConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/timeProfilerConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/timeProfiler
    )
endif()
