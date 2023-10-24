#ifndef TIMEPROFILER_H
#define TIMEPROFILER_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Create and initialize a profiler named '_name' with _nIncrements max measurements
 *
 */
#define TPROF_INIT(_name, _nIncrements)       \
  uint32_t tIncrements_##_name[_nIncrements]; \
  tProf_t  _name = {                          \
       .tStddev       = 0.0,                   \
       .tAverage     = 0.0,                   \
       .tMin         = UINT32_MAX,            \
       .tMax         = 0,                     \
       .tStart       = 0,                     \
       .tStopUsec    = 0,                     \
       .tIncrements  = tIncrements_##_name,   \
       .nIncrements  = _nIncrements,          \
       .currentIndex = 0,                     \
       .name         = #_name,                \
       .status       = TPROF_STOPPED}

/**
 * @brief Status of the profiler
 *
 */
typedef enum tProfStatus_t
{
  TPROF_ERROR = 0,
  TPROF_RUNNING,
  TPROF_STOPPED,
  TPROF_FULL
} tProfStatus_t;

/**
 * @brief Time profiler data. Note that the profiler does not know the time measurement unit (ms, us, etc...)
 * That depends on the clock implementation
 *
 */
typedef struct tProf_t
{
  double        tStddev;       /*!< Standard deviation */
  double        tAverage;     /*!< Average time measured */
  uint32_t      tMin;         /*!< Minimum time measured */
  uint32_t      tMax;         /*!< Maximum time measured */
  uint32_t      tStart;       /*!< Starting time of the current measurement */
  uint32_t      tStopUsec;    /*!< Stopping time of the current measurement */
  uint32_t *    tIncrements;  /*!< Array to store the measurements */
  uint32_t      nIncrements;  /*!< Max number of measurements in this profiler */
  uint32_t      currentIndex; /*!< Index of the current measurement */
  char *        name;         /*!< Name of this profiler*/
  tProfStatus_t status;       /*!< Status */
} tProf_t;

/**
 * @brief Foo a time profiler
 *
 * @return true time profiler fooed
 * @return false error fooing time profiler
 */
bool tProf_foo(void);

#endif
