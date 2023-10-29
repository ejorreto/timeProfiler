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

#include <timeProfiler_clock.h>

#include <time.h>

/* TODO Review data type selection for this clock to identify overflows */

/**
 * @brief Gets the time of microseconds since the Epoch
 * 
 * @return uint32_t Microseconds
 */
uint32_t tProfReadClock(void)
{
  struct timespec timeSinceEpoch;
  clock_gettime(CLOCK_REALTIME, &timeSinceEpoch);
  // Alternative clocks that can be used: CLOCK_REALTIME , CLOCK_PROCESS_CPUTIME_ID, but they depend on the implementation
  return (uint32_t)((1e6 * timeSinceEpoch.tv_sec) + (1e-3 * timeSinceEpoch.tv_nsec));
}