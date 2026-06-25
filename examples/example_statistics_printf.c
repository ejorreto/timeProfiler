/**
 * @file example_statistics_printf.c
 * @brief Reference implementation of a statistics output handler using printf.
 *
 * This file is NOT part of the library. Copy it into your own project or use
 * it as a starting point for a custom tProfGetStatistics handler.
 */

#include <stdio.h>
#include "example_statistics_printf.h"

void tProfPrintfStatistics(const tProf_t * profiler, bool detailedStatistics)
{
  if (detailedStatistics == true)
  {
    /* Print the full list of measurments separated by ; for further processing offline */
    for (uint32_t i = 0; i < profiler->currentIndex; i++)
    {
      printf("%s;%u;%u\n", profiler->name, i, profiler->tIncrements[i]);
    }
  }
  else
  {
    /* Nothing else to do */
  }

  printf("%s Measurements %u\n", profiler->name, profiler->currentIndex);
  printf("%s Max %u\n", profiler->name, profiler->tMax);
  printf("%s Min %u\n", profiler->name, profiler->tMin);
  printf("%s StdDev %f\n", profiler->name, profiler->tStddev);
  printf("%s Average %f\n", profiler->name, profiler->tAverage);
}