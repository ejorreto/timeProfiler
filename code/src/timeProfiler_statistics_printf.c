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

#include <stdio.h>
#include <timeProfiler.h>

void tProfGetStatistics(const tProf_t * profiler, bool detailed_statistics)
{
  if (detailed_statistics == true)
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