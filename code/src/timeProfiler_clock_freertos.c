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

#include "FreeRTOS.h"
#include "task.h"

/* TODO Review data type selection for this clock to identify overflows */

/**
 * @brief Gets the number of ticks since vTaskStartScheduler was called. Convert to miliseconds with the pdTICKS_TO_MS macro
 * #define pdTICKS_TO_MS( xTicks )   ( ( TickType_t ) ( ( uint64_t ) ( xTicks ) * 1000 / configTICK_RATE_HZ ) )
 *
 * @return uint32_t Ticks
 */
uint32_t tProfReadClock(void)
{
  return (uint32_t)xTaskGetTickCount();
}