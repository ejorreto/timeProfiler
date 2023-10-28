#include <timeProfiler.h>
#include <timeProfiler_statistics.h>

#include <unistd.h>
#include <stdio.h>

int main(void)
{
  printf("Sample program: Measuring the duration of sleep(1)\n");
  TPROF_INIT(sample, 10);
  for (int i = 0; i < sample.nIncrements; i++)
  {
    tProfStart(&sample);
    sleep(1);
    tProfStop(&sample);
  }
  tProfCalculateStatistics(&sample);
  tProfGetStatistics(&sample, true);
  return 0;
}