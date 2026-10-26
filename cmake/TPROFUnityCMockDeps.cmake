# ===========================================================================
# Unity and CMock dependency resolution for the TEST configuration.
#
# The dependencies are resolved, per dependency and with Unity resolved
# first (CMock's headers include unity.h), in this order:
#
#   1. An already-defined CMake target (unity, unity::framework, cmock).
#   2. A pre-built library   : TPROF_<DEP>_LIBRARY with TPROF_<DEP>_INCLUDE_DIR.
#   3. A user source tree    : TPROF_<DEP>_SOURCE_DIR; its src/<dep>.c is
#                              compiled here into the <dep> target.
#   4. FetchContent          : pinned upstream release tags below.
#
# Whatever the rung, the build always exposes the targets `unity` and `cmock`.
# The Ruby-based CMock generator is a development tool for regenerating
# test/mocks and is not part of this build contract.
# ===========================================================================

include(FetchContent)

set(TPROF_UNITY_GIT_REPOSITORY "https://github.com/ThrowTheSwitch/Unity.git"
    CACHE STRING "Unity git repository used when the dependency is fetched")
set(TPROF_UNITY_GIT_TAG "v2.7.0"
    CACHE STRING "Unity release tag used when the dependency is fetched")
set(TPROF_CMOCK_GIT_REPOSITORY "https://github.com/ThrowTheSwitch/CMock.git"
    CACHE STRING "CMock git repository used when the dependency is fetched")
set(TPROF_CMOCK_GIT_TAG "v2.7.0"
    CACHE STRING "CMock release tag used when the dependency is fetched")

if(NOT TARGET unity)
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

if(NOT TARGET cmock)
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
  # cmock_internals.h includes unity.h; every consumer of cmock needs Unity's
  # headers as well.
  target_link_libraries(cmock PUBLIC unity)
endif()
