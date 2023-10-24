#include <timeProfiler.h>
#include <timeProfiler_clock.h>

bool tProf_foo(void)
{
  return true;
}

void tProfStart(tProf_t * profiler)
{
  if (profiler->currentIndex < profiler->nIncrements)
  {
    profiler->tStart = tProfReadClock();
  }
  else
  {
    /* Profiler full */
  }
}

void tProfStop(tProf_t * profiler)
{
  uint32_t increment;

  if (profiler->currentIndex < profiler->nIncrements)
  {
    profiler->tStop                               = tProfReadClock();
    increment                                     = profiler->tStop - profiler->tStart;
    profiler->tIncrements[profiler->currentIndex] = increment;

    profiler->currentIndex++;
  }
  else
  {
    /* Profiler full */
  }
}

void tProfCalculateStatistics(tProf_t * profiler)
{
  uint32_t lastIndex = profiler->currentIndex < profiler->nIncrements ? profiler->currentIndex : profiler->nIncrements;
  for (uint32_t i = 0; i < lastIndex; i++)
  {
    profiler->tMin = profiler->tIncrements[i] < profiler->tMin ? profiler->tIncrements[i] : profiler->tMin;
    profiler->tMax = profiler->tIncrements[i] > profiler->tMax ? profiler->tIncrements[i] : profiler->tMax;
  }
}