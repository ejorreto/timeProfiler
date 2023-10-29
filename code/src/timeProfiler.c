/*
 * Time Profiler
 * Copyright (C) 2023 Enrique Jorreto
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
#include <timeProfiler.h>
#include <timeProfiler_clock.h>

void tProfStart(tProf_t * profiler)
{
  if (profiler->currentIndex < profiler->nIncrements)
  {
    profiler->tStart = tProfReadClock();
    profiler->status = TPROF_RUNNING;
  }
  else
  {
    /* Profiler full */
    profiler->status = TPROF_FULL;
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
    profiler->status = TPROF_STOPPED;
    if (profiler->currentIndex == profiler->nIncrements)
    {
      /* Profiler full */
      profiler->status = TPROF_FULL;
      if (profiler->statisticsMode == STATISTICS_AUTOLITE)
      {
        tProfCalculateStatistics(profiler);
        tProfGetStatistics(profiler, false);
      }
      else if (profiler->statisticsMode == STATISTICS_AUTOFULL)
      {
        tProfCalculateStatistics(profiler);
        tProfGetStatistics(profiler, true);
      }
      else
      {
        /* Statics to be calculated manually by the user */
      }
    }
  }
  else
  {
    /* Profiler full */
    profiler->status = TPROF_FULL;
  }
}

void tProfCalculateStatistics(tProf_t * profiler)
{
  uint32_t lastIndex;

  if ((profiler->status == TPROF_STOPPED) || (profiler->status == TPROF_FULL))
  {
    lastIndex = profiler->currentIndex < profiler->nIncrements ? profiler->currentIndex : profiler->nIncrements;

    if (lastIndex == 0)
    {
      /* The profiler is empty, nothing to calculate */
    }
    else
    {
      uint32_t sum = 0;
      /* Max and min values */
      for (uint32_t i = 0; i < lastIndex; i++)
      {
        profiler->tMin = profiler->tIncrements[i] < profiler->tMin ? profiler->tIncrements[i] : profiler->tMin;
        profiler->tMax = profiler->tIncrements[i] > profiler->tMax ? profiler->tIncrements[i] : profiler->tMax;
        sum += profiler->tIncrements[i];
      }

      /* Average */
      profiler->tAverage = (double)sum / lastIndex;

      /* Standard deviation of a population */
      double squaredDiffSum = 0;
      for (uint32_t i = 0; i < lastIndex; i++)
      {
        double diff = (double)(profiler->tIncrements[i]) - profiler->tAverage;
        squaredDiffSum += (diff * diff);
      }
      double variance = squaredDiffSum / lastIndex;

      profiler->tStddev = sqrt(variance);
    }
  }
  else
  {
    profiler->status = TPROF_ERROR;
  }
}