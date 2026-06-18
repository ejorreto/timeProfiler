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
 * @file timeProfiler_clock_rtems.c
 * @brief RTEMS clock implementation for timeProfiler.
 *
 * Returns the current time in microseconds since system initialization using
 * CLOCK_MONOTONIC via rtems_clock_get_uptime.
 */

#include <timeProfiler_clock.h>

#include <rtems.h>

/**
 * @brief Gets the number of microseconds since some time point during the system initialization using CLOCK_MONOTONIC
 *
 * @return uint32_t Microseconds
 */
uint32_t tProfReadClock(void)
{
  struct timespec   uptime      = {0};
  uint32_t          currentTime = 0;
  rtems_status_code ec          = rtems_clock_get_uptime(&uptime);
  if (ec == RTEMS_SUCCESSFUL)
  {
    currentTime = (uint32_t)((1000000 * uptime.tv_sec) + (uptime.tv_nsec / 1000));
  }
  else
  {
    currentTime = 0;
    /* uptime not reachable */
  }
  return currentTime;
}