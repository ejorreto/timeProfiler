#include <unistd.h>
#include <stdio.h>

#include <timeProfiler.h>
#include "example_statistics_printf.h"

/**
 * @brief Sample program
 * 
 * @return int 0
 */
int main(void)
{
  /* Assign the function to printf the statistics */
  tProfGetStatistics = &tProfPrintfStatistics;

  printf("Sample program: Measuring the duration of sleep(1)\n");
  TPROF_INIT(sample, 10, STATISTICS_AUTOFULL);
  for (int i = 0; i < sample.nIncrements; i++)
  {
    tProfStart(&sample);
    sleep(1);
    tProfStop(&sample);
  }
  return 0;
}