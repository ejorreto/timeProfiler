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

The build is controlled via the CMake `ENV` flag.

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
cmake .. -DENV=RTEMS
make
```

### FreeRTOS

```bash
cmake .. -DENV=FREERTOS
make
```

### Unit tests & coverage

```bash
cmake .. -DENV=TEST
make unittest
```

* `make unittest` is a **custom target** that cleans `.gcda`, runs `ctest -V`, and generates coverage reports via `gcovr`.
* Do **not** run `make test` or `ctest` directly if you want coverage.
* Reports land in `reports/`.

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
- The full exception text is included in [`LICENSE.md`](LICENSE.md).

## Architecture

| Directory | Contents |
|-----------|----------|
| `library/` | Core library (`timeProfiler.c`, `timeProfiler_clock_*.c`, headers) |
| `examples/` | `example_posix.c` and `example_statistics_printf.c` (reference, not library) |
| `test/` | Unity + CMock tests (`test_timeProfiler.c`) |
| `external/` | Vendored Unity and CMock |

## Clock implementations

Time reading is platform-specific.  The library selects the appropriate source file at build time:

| `ENV` | Clock source |
|-------|--------------|
| `POSIX` (default) | `library/src/timeProfiler_clock_posix.c` |
| `TEST` | `library/src/timeProfiler_clock_posix.c` |
| `RTEMS` | `library/src/timeProfiler_clock_rtems.c` |
| `FREERTOS` | `library/src/timeProfiler_clock_freertos.c` |

If you need a different clock, implement `uint32_t tProfReadClock(void)` and add it to the build.
