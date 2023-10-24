#include <timeProfiler_clock.h>

#include <time.h>

uint32_t tProfReadClock(void)
{
  struct timespec tempReadClock;
  clock_gettime(CLOCK_REALTIME, &tempReadClock);
  // Alternative clocks that can be used: CLOCK_REALTIME , CLOCK_PROCESS_CPUTIME_ID
  return (uint32_t)((1e6 * tempReadClock.tv_sec) + (1e-3 * tempReadClock.tv_nsec));
}