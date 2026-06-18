/*
* Time Profiler
* Copyright (C) 2023 Enrique Jorreto
* 
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
* 
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file timeProfiler_clock_posix.c
 * @brief POSIX clock implementation for timeProfiler.
 *
 * Returns the current time in microseconds since the Epoch using CLOCK_REALTIME.
 */

#include <time.h>

#include <timeProfiler_clock.h>

/**
 * @brief Gets the time of microseconds since the Epoch
 * 
 * @return uint32_t Microseconds. 0 if time cannot be obtained.
 */
uint32_t tProfReadClock(void)
{
  struct timespec timeSinceEpoch;
  uint32_t currentTime = 0;
  int ec = clock_gettime(CLOCK_REALTIME, &timeSinceEpoch);
  if(ec == 0)
  {
    currentTime = (uint32_t)((1000000 * timeSinceEpoch.tv_sec) + (timeSinceEpoch.tv_nsec/1000));
  }
  else
  {
    currentTime = 0;
    /* Error code in errno. Clock not implemented, timeSinceEpoch not reachable */
  }
  return currentTime;
}