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

#ifndef TIMEPROFILER_H
#define TIMEPROFILER_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Create and initialize a profiler named '_name' with _nIncrements max measurements, and configure auto statistics mode
 *
 */
#define TPROF_INIT(_name, _nIncrements, _statisticsMode)                  \
  uint32_t tIncrements_##_name[_nIncrements] = {0};                       \
  tProf_t  _name                             = {                          \
                                   .tStddev        = 0.0,                 \
                                   .tAverage       = 0.0,                 \
                                   .tMin           = UINT32_MAX,          \
                                   .tMax           = 0,                   \
                                   .tStart         = 0,                   \
                                   .tStop          = 0,                   \
                                   .tIncrements    = tIncrements_##_name, \
                                   .nIncrements    = _nIncrements,        \
                                   .currentIndex   = 0,                   \
                                   .name           = #_name,              \
                                   .status         = TPROF_STOPPED,       \
                                   .statisticsMode = _statisticsMode}

/**
 * @brief Status of the profiler
 *
 */
typedef enum tProfStatus_t
{
  TPROF_ERROR = 0, /*!< Error in the operation of the profiler */
  TPROF_RUNNING, /*!< Profiler is started */
  TPROF_STOPPED, /*!< Profiler is stopped. Statistics can be calculated */
  TPROF_FULL /*!< Profiler has reached its full capacity. Statistics can be calculated */
} tProfStatus_t;

/**
 * @brief Configures if statistics shall be calculated automatically when the profiler gets full.
 *
 */
typedef enum tProfStatistics_t
{
  STATISTICS_MANUAL = 0, /*!< No automatic calculation. The user has to do it manually */
  STATISTICS_AUTOLITE,   /*!< Calculate and get reduced statistics */
  STATISTICS_AUTOFULL    /*!< Calculate and get complete statistics  */
} tProfStatistics_t;

/**
 * @brief Time profiler data. Note that the profiler does not know the time measurement unit (ms, us, etc...)
 * That depends on the clock implementation
 *
 */
typedef struct tProf_t
{
  double            tStddev;        /*!< Standard deviation */
  double            tAverage;       /*!< Average time measured */
  uint32_t          tMin;           /*!< Minimum time measured */
  uint32_t          tMax;           /*!< Maximum time measured */
  uint32_t          tStart;         /*!< Starting time of the current measurement */
  uint32_t          tStop;          /*!< Stopping time of the current measurement */
  uint32_t *        tIncrements;    /*!< Array to store the measurements */
  uint32_t          nIncrements;    /*!< Max number of measurements in this profiler */
  uint32_t          currentIndex;   /*!< Index of the current measurement */
  char *            name;           /*!< Name of this profiler*/
  tProfStatus_t     status;         /*!< Status */
  tProfStatistics_t statisticsMode; /*!< How to present statistics when the profiler gets full */
} tProf_t;

/**
 * @brief Store the current time before executing the function being profiled
 *
 * @param profiler Profiler to be used to store the data
 */
void tProfStart(tProf_t * profiler);

/**
 * @brief Store the current time after executiong the function being profiled
 *
 * @param profiler Profiler to be used to store the data
 */
void tProfStop(tProf_t * profiler);

/**
 * @brief Calculate some statistics on the measurements performed
 *
 * @param profiler Profiler to be used
 */
void tProfCalculateStatistics(tProf_t * profiler);

/**
 * @brief Output the statistics of a profiler. To be implemented differently in different hosts / environments
 *
 * @param profiler Profiler to get the statistics from
 * @param detailed_statistics True if detailed statistics are required, i.e.: Printing all measurements besides
 * the min/max/average etc...
 */
void tProfGetStatistics(const tProf_t * profiler, bool detailedStatistics);

#endif
