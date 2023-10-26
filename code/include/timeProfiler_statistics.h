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

#ifndef TIMEPROFILER_STATISTICS_H
#define TIMEPROFILER_STATISTICS_H

#include <stdbool.h>
#include <timeProfiler.h>

/**
 * @brief Output the statistics of a profiler. To be implemented differently in different hosts / environments
 * 
 * @param profiler Profiler to get the statistics from
 * @param detailed_statistics True if detailed statistics are required, i.e.: Printing all measurements besides
 * the min/max/average etc...
 */
void tProfGetStatistics(const tProf_t * profiler, bool detailed_statistics);

#endif
