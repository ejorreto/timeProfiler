# timeProfiler

C library to profile execution times in embedded and POSIX environments.

## What this library does (and does not)

The library **measures** time intervals and **calculates** statistics (min, max, average, standard deviation).  
It **does not** know how to display or transmit those results.  

You must provide a function that consumes the statistics and assign it to the global function pointer `tProfGetStatistics`.  This is a deliberate design choice: the library stays I/O-free so it can be used on systems without `printf`, UART, or a filesystem.

```c
tProfGetStatistics = &myCustomStatisticsHandler;
TPROF_INIT(prof, 100, STATISTICS_AUTOFULL);
```

## Data flow

```
┌─────────────────┐     tProfStart()      ┌──────────────┐
│   Your code     │ ─────────────────────>│  timeProfiler │
│  (the code you  │                       │   library     │
│   want to time) │ <─────────────────────│               │
└─────────────────┘     tProfStop()       └──────────────┘
                                                │
                                                │ calculations
                                                ▼
                                       ┌──────────────────┐
                                       │  tProfCalculate  │
                                       │   Statistics()   │
                                       └──────────────────┘
                                                │
                                                │ calls
                                                ▼
                                       ┌──────────────────┐
                                       │  YOUR FUNCTION   │
                                       │ tProfGetStatistics│
                                       │ (printf, UART,   │
                                       │  log, etc.)      │
                                       └──────────────────┘
```

A **reference** `printf`-based implementation is provided in `examples/example_statistics_printf.c`, but it is **not** part of the library.  Copy it into your project or write your own.

## Key principles

* **KISS**: smallest footprint suitable for embedded systems.
* Written in C, built with CMake.
* Tested with Unity and CMock.
* I/O-agnostic: the library never calls `printf`, `malloc`, or any blocking operation.
* This is experimental. Probably forever.
* One solution does not fit all. There are multiple ways to measure time, software based and hardware based, timeProfiler does not aim to solve all use cases.

## How to build

The build is controlled via the CMake `TPROF_TARGET` flag.

```bash
mkdir -p build && cd build
```

### POSIX (default)

```bash
cmake ..
make
```

This builds the static library and the `example_posix` executable.

### RTEMS

```bash
cmake .. -DTPROF_TARGET=RTEMS
make
```

### FreeRTOS

```bash
cmake .. -DTPROF_TARGET=FREERTOS
make
```

### Unit tests & coverage

```bash
cmake .. -DTPROF_TARGET=TEST
make test_coverage
```

* The TEST configuration requires **CMake 3.18 or newer**.
* Unity and CMock are **not vendored**: without further options they are fetched at configure time (network required) — see *Providing Unity and CMock* below.
* `make test_coverage` is a **custom target** that cleans `.gcda`, runs `ctest -V`, and generates coverage reports via `gcovr`.
* Do **not** run `make test` or `ctest` directly if you want coverage.
* Reports land in `reports/`.

### Build types and compiler flags

The library itself does **not** force any optimization level, so you control it via standard CMake mechanisms.

CMake automatically applies optimization flags when you set `CMAKE_BUILD_TYPE`:

| Build type | Typical flags |
|------------|---------------|
| `Release` | `-O3 -DNDEBUG` |
| `Debug` | `-O0 -g` |
| `RelWithDebInfo` | `-O2 -g -DNDEBUG` |
| `MinSizeRel` | `-Os -DNDEBUG` |

You can also override or augment flags manually:

```bash
cmake .. -DCMAKE_C_FLAGS="-O2 -g"
```

Note that the library target always enforces **`-Wall -Werror`** regardless of build type.

When building with **`-DTPROF_TARGET=TEST`**, the configuration forces `-O0 -g` plus coverage flags (`-fprofile-arcs -ftest-coverage`) to ensure accurate `gcov` results, overriding any externally supplied optimization level.

### Install

`make install` (or `cmake --install .` from the build directory) installs the library for consumption by other projects. Install rules exist for the `POSIX`, `RTEMS` and `FREERTOS` targets. The `TEST` configuration is excluded: it produces a coverage-instrumented, non-optimized library that is not fit for distribution.

The install location is the standard `CMAKE_INSTALL_PREFIX` (default `/usr/local`):

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=/your/install/prefix
make
cmake --install .        # or: make install
```

Installed layout (standard `GNUInstallDirs`):

| Artifact | Destination |
|----------|-------------|
| `libtimeProfiler.a` (static) | `<prefix>/lib/` (`lib64` where applicable) |
| `timeProfiler.h` (public API header) | `<prefix>/include/` |
| CMake package files | `<prefix>/lib/cmake/timeProfiler/` |

Only `timeProfiler.h` is installed — it is the entire public API. A custom clock implementation means rebuilding the library from source (see *Clock implementations* below). For RTEMS and FreeRTOS cross builds, pass your BSP/toolchain sysroot prefix as `CMAKE_INSTALL_PREFIX`.

Consuming the library from another CMake project:

```cmake
find_package(timeProfiler CONFIG REQUIRED)
target_link_libraries(your_app PRIVATE timeProfiler::timeProfiler)
```

The imported target carries the include path and the link library, so no manual `include_directories()` is needed. Linking the installed `libtimeProfiler.a` directly also works if you prefer not to use CMake packages.

## Providing Unity and CMock

The TEST configuration needs the Unity test framework and the CMock runtime. They are **not vendored** in this repository. Each dependency is resolved through the ladder below, and Unity is always resolved before CMock (CMock's headers include `unity.h`).

1. **Pre-defined CMake target** — if your build already defines `unity` (or the `unity::framework` alias, imported globally) or `cmock`, those targets are used as they are.
2. **Pre-built library** — point at an archive or shared library you built:

   ```bash
   cmake .. -DTPROF_TARGET=TEST \
     -DTPROF_UNITY_LIBRARY=/path/to/libunity.a \
     -DTPROF_UNITY_INCLUDE_DIR=/path/to/unity/headers \
     -DTPROF_CMOCK_LIBRARY=/path/to/libcmock.a \
     -DTPROF_CMOCK_INCLUDE_DIR=/path/to/cmock/headers
   ```

3. **Source tree** — point at a Unity/CMock checkout or zip extract; its `src/unity.c` / `src/cmock.c` is compiled by this build:

   ```bash
   cmake .. -DTPROF_TARGET=TEST \
     -DTPROF_UNITY_SOURCE_DIR=/path/to/Unity \
     -DTPROF_CMOCK_SOURCE_DIR=/path/to/CMock
   ```

4. **FetchContent default** — with none of the above, the sources are downloaded at configure time from the upstream repositories, pinned to release tags. No git submodules are involved.

| Variable | Meaning |
|----------|---------|
| `TPROF_UNITY_INCLUDE_DIR` | Directory containing `unity.h` |
| `TPROF_UNITY_LIBRARY` | Pre-built Unity archive or shared library |
| `TPROF_UNITY_SOURCE_DIR` | Unity source tree (contains `src/unity.c`) |
| `TPROF_CMOCK_INCLUDE_DIR` | Directory containing `cmock.h` |
| `TPROF_CMOCK_LIBRARY` | Pre-built CMock archive or shared library |
| `TPROF_CMOCK_SOURCE_DIR` | CMock source tree (contains `src/cmock.c`) |
| `TPROF_UNITY_GIT_TAG` / `TPROF_UNITY_GIT_REPOSITORY` | FetchContent pin overrides (Unity) |
| `TPROF_CMOCK_GIT_TAG` / `TPROF_CMOCK_GIT_REPOSITORY` | FetchContent pin overrides (CMock) |

Notes:

* **Tested versions**: Unity `v2.7.0` and CMock `v2.7.0`. When a provided version can be read from its headers and differs from these pins, the configure step emits a warning — the checked-in mocks in `test/mocks/` were generated against the tested versions and may need regeneration.
* **A pre-built Unity must be built with `UNITY_INCLUDE_DOUBLE`** (the definition the TEST configuration uses); a Unity library built without it breaks the double-precision assertions.
* CMock's runtime needs no CException, but its headers need Unity's, so provide both together.
* In super-projects that `add_subdirectory()` timeProfiler, run `ctest` from the timeProfiler build subdirectory (testing is enabled in its scope).
* Regenerating mocks is a development step, not part of the build: from the `test/` directory, run CMock's Ruby generator against the public header with the local config:

  ```bash
  cd test
  ruby <cmock-checkout>/lib/cmock.rb -o cmock_config.yml ../library/include/timeProfiler_clock.h
  ```

## Quick start

```c
#include <timeProfiler.h>

/* You must implement this function or use the reference example */
void myStatisticsHandler(const tProf_t * profiler, bool detailedStatistics)
{
    /* e.g. send over UART, write to flash, or print */
}

int main(void)
{
    tProfGetStatistics = &myStatisticsHandler;

    TPROF_INIT(prof, 10, STATISTICS_AUTOFULL);

    for (int i = 0; i < 10; i++)
    {
        tProfStart(&prof);
        /* ... code to measure ... */
        tProfStop(&prof);
    }
    /* When the profiler is full, statistics are calculated
       and myStatisticsHandler is called automatically. */
    return 0;
}
```

## Reference example

`examples/example_statistics_printf.c` shows one possible output handler using `printf`.  It is **not** compiled into the library.  Include it in your own project if you want console output.

## License

timeProfiler is licensed under the **GNU Lesser General Public License v3 (or later)** with an added **linking exception**.

What this means in practice:

- You may use this library in **both open-source and proprietary projects**.
- You may link it **statically or dynamically** without any requirement to open-source your own code.
- If you **modify** the library itself, those modifications remain under LGPL and must be shared if you distribute the modified library.
- The full exception text is included in the `LICENSE.md` file at the repository root.

## Architecture

| Directory | Contents |
|-----------|----------|
| `library/` | Core library (`timeProfiler.c`, `timeProfiler_clock_*.c`, headers, CMake package template) |
| `examples/` | `example_posix.c` and `example_statistics_printf.c` (reference, not library) |
| `test/` | Unity + CMock tests (`test_timeProfiler.c`) |
| `cmake/` | Dependency resolution for the TEST configuration (FetchContent pins or user-provided Unity/CMock) |

## Clock implementations

Time reading is platform-specific.  The library selects the appropriate source file at build time:

| `TPROF_TARGET` | Clock source |
|-------|--------------|
| `POSIX` (default) | `library/src/timeProfiler_clock_posix.c` |
| `TEST` | `library/src/timeProfiler_clock_posix.c` |
| `RTEMS` | `library/src/timeProfiler_clock_rtems.c` |
| `FREERTOS` | `library/src/timeProfiler_clock_freertos.c` |

If you need a different clock, implement `uint32_t tProfReadClock(void)` and add it to the build.
