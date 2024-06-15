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

#include <rtems.h>

/* TODO Review data type selection for this clock to identify overflows */

/**
 * @brief Gets the number of miliseconds since some time point during the system initialization using CLOCK_MONOTONIC
 * 
 * @return uint32_t Miliseconds
 */
uint32_t tProfReadClock(void)
{
  struct timespec uptime;
  rtems_clock_get_uptime(&uptime);
  return (uint32_t)((1e3 * uptime.tv_sec) + (1e-6 * uptime.tv_nsec));
}