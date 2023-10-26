#ifndef TIMEPROFILER_CLOCK_H
#define TIMEPROFILER_CLOCK_H

#include <stdint.h>

/**
 * @brief Get a clock value from the system. To be implemented differently on different environments
 *
 * @return uint32_t Clock value, each clock implementation can return this value in different units (ms, us, etc...)
 */
uint32_t tProfReadClock(void);

#endif
