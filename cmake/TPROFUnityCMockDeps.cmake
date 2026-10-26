# ===========================================================================
# Unity and CMock dependency resolution for the TEST configuration.
#
# The dependencies are resolved, per dependency and with Unity resolved
# first (CMock's headers include unity.h), through a four-rung ladder:
#
#   1. An already-defined CMake target (unity, unity::framework, cmock).
#   2. A pre-built library   : TPROF_<DEP>_LIBRARY with TPROF_<DEP>_INCLUDE_DIR.
#   3. A user source tree    : TPROF_<DEP>_SOURCE_DIR; its src/<dep>.c is
#                              compiled here into the <dep> target.
#   4. FetchContent          : pinned upstream release tags below.
#
# Whatever the rung, the build always exposes the targets `unity` and `cmock`.
# A pre-built Unity library must have been built with UNITY_INCLUDE_DOUBLE,
# the definition the TEST configuration uses. When the provided version can
# be read from the headers and differs from the pins, a WARNING is emitted.
# The Ruby-based CMock generator is a development tool for regenerating
# test/mocks and is not part of this build contract.
# ===========================================================================

include(FetchContent)

# --- FetchContent pins (overridable) ---------------------------------------
set(TPROF_UNITY_GIT_REPOSITORY "https://github.com/ThrowTheSwitch/Unity.git"
    CACHE STRING "Unity git repository used when the dependency is fetched")
set(TPROF_UNITY_GIT_TAG "v2.7.0"
    CACHE STRING "Unity release tag used when the dependency is fetched")
set(TPROF_CMOCK_GIT_REPOSITORY "https://github.com/ThrowTheSwitch/CMock.git"
    CACHE STRING "CMock git repository used when the dependency is fetched")
set(TPROF_CMOCK_GIT_TAG "v2.7.0"
    CACHE STRING "CMock release tag used when the dependency is fetched")

# --- User-provision variables (public contract, see README) ----------------
set(TPROF_UNITY_INCLUDE_DIR ""
    CACHE PATH "Directory containing unity.h for a user-provided Unity")
set(TPROF_UNITY_LIBRARY ""
    CACHE FILEPATH "Pre-built Unity archive or shared library to link instead of compiling unity.c")
set(TPROF_UNITY_SOURCE_DIR ""
    CACHE PATH "Unity source tree whose src/unity.c is compiled by this build")
set(TPROF_CMOCK_INCLUDE_DIR ""
    CACHE PATH "Directory containing cmock.h for a user-provided CMock")
set(TPROF_CMOCK_LIBRARY ""
    CACHE FILEPATH "Pre-built CMock archive or shared library to link instead of compiling cmock.c")
set(TPROF_CMOCK_SOURCE_DIR ""
    CACHE PATH "CMock source tree whose src/cmock.c is compiled by this build")

# Versions expected from the pins above ("v2.7.0" -> "2.7.0").
string(REGEX REPLACE "^v" "" TPROF_UNITY_TESTED_VERSION "${TPROF_UNITY_GIT_TAG}")
string(REGEX REPLACE "^v" "" TPROF_CMOCK_TESTED_VERSION "${TPROF_CMOCK_GIT_TAG}")

# ---------------------------------------------------------------------------
# tprof_report_dep_version(<UNITY|CMOCK> <header path> <tested version>)
#
# Reads UNITY_VERSION_* / CMOCK_VERSION_* out of the dependency header when
# reachable and warns when it differs from the tested pin. Silent when the
# header cannot be located.
# ---------------------------------------------------------------------------
function(tprof_report_dep_version dep header_path tested_version)
  set(_major 0)
  set(_minor 0)
  set(_build 0)
  if(EXISTS "${header_path}")
    file(STRINGS "${header_path}" _version_lines
         REGEX "^#define ${dep}_VERSION_(MAJOR|MINOR|BUILD)[ \t]+[0-9]+[ \t]*$")
    foreach(_line IN LISTS _version_lines)
      if(_line MATCHES "VERSION_MAJOR[ \t]+([0-9]+)")
        set(_major "${CMAKE_MATCH_1}")
      elseif(_line MATCHES "VERSION_MINOR[ \t]+([0-9]+)")
        set(_minor "${CMAKE_MATCH_1}")
      elseif(_line MATCHES "VERSION_BUILD[ \t]+([0-9]+)")
        set(_build "${CMAKE_MATCH_1}")
      endif()
    endforeach()
    set(_detected "${_major}.${_minor}.${_build}")
    if(_detected STREQUAL tested_version)
      message(STATUS "timeProfiler: ${dep} version ${_detected} matches the tested pin")
    else()
      message(WARNING
        "timeProfiler: ${dep} version ${_detected} (from ${header_path}) differs "
        "from the tested ${tested_version} pin; the mocks under test/mocks were "
        "generated against the tested version and may need regeneration")
    endif()
  endif()
endfunction()

# ===========================================================================
# Unity (resolved before CMock: cmock_internals.h includes unity.h)
# ===========================================================================
if(TARGET unity::framework AND NOT TARGET unity)
  # find_package(unity) exposes the namespaced alias; the build links `unity`.
  # This fails when unity::framework is a directory-scoped imported target
  # from another directory; global imports work.
  add_library(unity ALIAS unity::framework)
endif()

if(NOT TARGET unity)
  if(TPROF_UNITY_LIBRARY)
    if(NOT EXISTS "${TPROF_UNITY_LIBRARY}")
      message(FATAL_ERROR
        "timeProfiler: TPROF_UNITY_LIBRARY '${TPROF_UNITY_LIBRARY}' does not exist")
    endif()
    if(TPROF_UNITY_INCLUDE_DIR)
      set(_unity_include "${TPROF_UNITY_INCLUDE_DIR}")
    elseif(TPROF_UNITY_SOURCE_DIR)
      set(_unity_include "${TPROF_UNITY_SOURCE_DIR}/src")
    else()
      message(FATAL_ERROR
        "timeProfiler: TPROF_UNITY_LIBRARY needs Unity's headers: set "
        "TPROF_UNITY_INCLUDE_DIR (or TPROF_UNITY_SOURCE_DIR alongside)")
    endif()
    if(NOT EXISTS "${_unity_include}/unity.h")
      message(FATAL_ERROR
        "timeProfiler: unity.h not found under '${_unity_include}' "
        "(point TPROF_UNITY_INCLUDE_DIR at the directory holding unity.h)")
    endif()
    message(STATUS "timeProfiler: Unity via pre-built library ${TPROF_UNITY_LIBRARY}")
    add_library(unity UNKNOWN IMPORTED)
    set_target_properties(unity PROPERTIES
      IMPORTED_LOCATION "${TPROF_UNITY_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${_unity_include}")
    tprof_report_dep_version(UNITY "${_unity_include}/unity.h"
                             "${TPROF_UNITY_TESTED_VERSION}")
  elseif(TPROF_UNITY_SOURCE_DIR)
    if(NOT EXISTS "${TPROF_UNITY_SOURCE_DIR}/src/unity.c")
      message(FATAL_ERROR
        "timeProfiler: TPROF_UNITY_SOURCE_DIR '${TPROF_UNITY_SOURCE_DIR}' has no "
        "src/unity.c; point it at a Unity source tree")
    endif()
    if(TPROF_UNITY_INCLUDE_DIR)
      set(_unity_include "${TPROF_UNITY_INCLUDE_DIR}")
    else()
      set(_unity_include "${TPROF_UNITY_SOURCE_DIR}/src")
    endif()
    message(STATUS "timeProfiler: Unity via source tree ${TPROF_UNITY_SOURCE_DIR}")
    add_library(unity STATIC "${TPROF_UNITY_SOURCE_DIR}/src/unity.c")
    target_include_directories(unity PUBLIC "${_unity_include}")
    tprof_report_dep_version(UNITY "${_unity_include}/unity.h"
                             "${TPROF_UNITY_TESTED_VERSION}")
  else()
    message(STATUS "timeProfiler: Unity via FetchContent ${TPROF_UNITY_GIT_TAG}")
    FetchContent_Declare(unity
      GIT_REPOSITORY "${TPROF_UNITY_GIT_REPOSITORY}"
      GIT_TAG        "${TPROF_UNITY_GIT_TAG}"
      GIT_SHALLOW    TRUE
      # Unity ships its own CMakeLists.txt, which this build must not enter:
      # unity.c is compiled here with the definitions chosen by the TEST
      # configuration. Pointing SOURCE_SUBDIR at a directory without a
      # CMakeLists.txt makes FetchContent_MakeAvailable() populate the sources
      # without calling add_subdirectory() on them.
      SOURCE_SUBDIR  cmake-not-consumed
    )
    FetchContent_MakeAvailable(unity)

    add_library(unity STATIC "${unity_SOURCE_DIR}/src/unity.c")
    target_include_directories(unity PUBLIC "${unity_SOURCE_DIR}/src")
  endif()
elseif(TPROF_UNITY_LIBRARY OR TPROF_UNITY_SOURCE_DIR)
  message(STATUS "timeProfiler: using pre-defined target unity; TPROF_UNITY_* inputs ignored")
else()
  message(STATUS "timeProfiler: Unity via pre-defined target")
endif()

# ===========================================================================
# CMock
# ===========================================================================
if(NOT TARGET cmock)
  if(TPROF_CMOCK_LIBRARY)
    if(NOT EXISTS "${TPROF_CMOCK_LIBRARY}")
      message(FATAL_ERROR
        "timeProfiler: TPROF_CMOCK_LIBRARY '${TPROF_CMOCK_LIBRARY}' does not exist")
    endif()
    if(TPROF_CMOCK_INCLUDE_DIR)
      set(_cmock_include "${TPROF_CMOCK_INCLUDE_DIR}")
    elseif(TPROF_CMOCK_SOURCE_DIR)
      set(_cmock_include "${TPROF_CMOCK_SOURCE_DIR}/src")
    else()
      message(FATAL_ERROR
        "timeProfiler: TPROF_CMOCK_LIBRARY needs CMock's headers: set "
        "TPROF_CMOCK_INCLUDE_DIR (or TPROF_CMOCK_SOURCE_DIR alongside)")
    endif()
    if(NOT EXISTS "${_cmock_include}/cmock.h")
      message(FATAL_ERROR
        "timeProfiler: cmock.h not found under '${_cmock_include}' "
        "(point TPROF_CMOCK_INCLUDE_DIR at the directory holding cmock.h)")
    endif()
    message(STATUS "timeProfiler: CMock via pre-built library ${TPROF_CMOCK_LIBRARY}")
    add_library(cmock UNKNOWN IMPORTED)
    set_target_properties(cmock PROPERTIES
      IMPORTED_LOCATION "${TPROF_CMOCK_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${_cmock_include}")
    tprof_report_dep_version(CMOCK "${_cmock_include}/cmock.h"
                             "${TPROF_CMOCK_TESTED_VERSION}")
  elseif(TPROF_CMOCK_SOURCE_DIR)
    if(NOT EXISTS "${TPROF_CMOCK_SOURCE_DIR}/src/cmock.c")
      message(FATAL_ERROR
        "timeProfiler: TPROF_CMOCK_SOURCE_DIR '${TPROF_CMOCK_SOURCE_DIR}' has no "
        "src/cmock.c; point it at a CMock source tree")
    endif()
    if(TPROF_CMOCK_INCLUDE_DIR)
      set(_cmock_include "${TPROF_CMOCK_INCLUDE_DIR}")
    else()
      set(_cmock_include "${TPROF_CMOCK_SOURCE_DIR}/src")
    endif()
    message(STATUS "timeProfiler: CMock via source tree ${TPROF_CMOCK_SOURCE_DIR}")
    add_library(cmock STATIC "${TPROF_CMOCK_SOURCE_DIR}/src/cmock.c")
    target_include_directories(cmock PUBLIC "${_cmock_include}")
    tprof_report_dep_version(CMOCK "${_cmock_include}/cmock.h"
                             "${TPROF_CMOCK_TESTED_VERSION}")
    # cmock_internals.h includes unity.h; every consumer of cmock needs
    # Unity's headers as well.
    target_link_libraries(cmock PUBLIC unity)
  else()
    message(STATUS "timeProfiler: CMock via FetchContent ${TPROF_CMOCK_GIT_TAG}")
    FetchContent_Declare(cmock
      GIT_REPOSITORY "${TPROF_CMOCK_GIT_REPOSITORY}"
      GIT_TAG        "${TPROF_CMOCK_GIT_TAG}"
      GIT_SHALLOW    TRUE
      GIT_SUBMODULES ""  # CMock vendors Unity/CException under vendor/; unused here
      SOURCE_SUBDIR  cmake-not-consumed
    )
    FetchContent_MakeAvailable(cmock)

    add_library(cmock STATIC "${cmock_SOURCE_DIR}/src/cmock.c")
    target_include_directories(cmock PUBLIC "${cmock_SOURCE_DIR}/src")
    target_link_libraries(cmock PUBLIC unity)
  endif()
elseif(TPROF_CMOCK_LIBRARY OR TPROF_CMOCK_SOURCE_DIR)
  message(STATUS "timeProfiler: using pre-defined target cmock; TPROF_CMOCK_* inputs ignored")
else()
  message(STATUS "timeProfiler: CMock via pre-defined target")
endif()
