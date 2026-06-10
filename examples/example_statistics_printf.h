/**
 * @file example_statistics_printf.h
 * @brief Reference statistics output handler header.
 *
 * This file is NOT part of the library. It declares a sample
 * tProfGetStatistics implementation that prints results via printf.
 */

#ifndef EXAMPLE_STATISTICS_PRINTF_H
#define EXAMPLE_STATISTICS_PRINTF_H

#include <timeProfiler.h>

/**
 * @brief Reference implementation of a statistics output handler using printf.
 * @param profiler Profiler to print statistics for.
 * @param detailedStatistics If true, print all individual measurements.
 *
 * @note This is a reference implementation. It is NOT part of the library.
 *       Implement your own handler or copy this into your project.
 */
void tProfPrintfStatistics(const tProf_t * profiler, bool detailedStatistics);

#endif 
