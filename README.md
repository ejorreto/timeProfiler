# timeProfiler
C library to profile execution times

## What is this about?

The timeProfiler library is a tool that basically measures time between calls to the library, with the goal of calculating some statistics and help understand how much time is spent in different parts of your program. No more, no less.

## Key principles

The timeProfiler is built under the following principles:

* KISS: Keep It Simple, Stupid. Having the smallest footprint as possible, but not smaller, making it suitable for embedded systems.
* Programmed in C.
* Built with CMake
* Tested with Unity and CMock.
* Be standards friendly. This is not meant to be critical software, but being close to such standards is always nice.
* Don't over optimize.
* One solution does not fit all. There are multiple ways to measure time, software based and hardware based, timeProfiler does not aim to solve all use cases.
* Experimental. Probably forever.
