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

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "unity.h"
#include <MocktimeProfiler_clock.h>

#include "timeProfiler.h"

void setUp(void)
{
  MocktimeProfiler_clock_Init();
  MocktimeProfiler_clock_Verify();
}

void tearDown(void)
{
  /* Nothing to tear down yet */
}

void test_tProf_init(void)
{
  TPROF_INIT(sample, 10, STATISTICS_MANUAL);

  TEST_ASSERT_EQUAL_DOUBLE(0.0, sample.tStddev);
  TEST_ASSERT_EQUAL_DOUBLE(0.0, sample.tAverage);
  TEST_ASSERT_EQUAL_UINT32(UINT32_MAX, sample.tMin);
  TEST_ASSERT_EQUAL_UINT32(0, sample.tMax);
  TEST_ASSERT_EQUAL_UINT32(0, sample.tStart);
  TEST_ASSERT_EQUAL_UINT32(0, sample.tStop);
  TEST_ASSERT_EQUAL_UINT32(0, sample.currentIndex);
  TEST_ASSERT_EQUAL_UINT32(10, sample.nIncrements);
  TEST_ASSERT_EQUAL_STRING("sample", sample.name);
  TEST_ASSERT_EQUAL_UINT32(TPROF_STOPPED, sample.status);
}

void test_tProf_stats_maxmin(void)
{
  TPROF_INIT(sample, 10, STATISTICS_MANUAL);
  tProfReadClock_ExpectAndReturn(1);
  tProfReadClock_ExpectAndReturn(3);
  tProfStart(&sample);
  tProfStop(&sample);
  tProfReadClock_ExpectAndReturn(1);
  tProfReadClock_ExpectAndReturn(7);
  tProfStart(&sample);
  tProfStop(&sample);
  tProfCalculateStatistics(&sample);
  tProfGetStatistics(&sample, true);
  TEST_ASSERT_EQUAL_UINT32(2, sample.tIncrements[0]);
  TEST_ASSERT_EQUAL_UINT32(6, sample.tIncrements[1]);
  TEST_ASSERT_EQUAL_UINT32(2, sample.tMin);
  TEST_ASSERT_EQUAL_UINT32(6, sample.tMax);
}

void test_tProf_overflow(void)
{
  TPROF_INIT(sample, 10, STATISTICS_MANUAL);
  for (uint32_t i = 0; i < sample.nIncrements + 1; i++)
  {
    tProfReadClock_ExpectAndReturn(1);
    tProfReadClock_ExpectAndReturn(3);
    tProfStart(&sample);
    tProfStop(&sample);
  }
  TEST_ASSERT_EQUAL_INT(TPROF_FULL, sample.status);
  TEST_ASSERT_EQUAL_UINT32(sample.nIncrements, sample.currentIndex);
}

void test_tProf_incorrectOperation(void)
{
  TPROF_INIT(sample, 10, STATISTICS_MANUAL);

  tProfReadClock_ExpectAndReturn(1);

  tProfStart(&sample);
  tProfCalculateStatistics(&sample);

  TEST_ASSERT_EQUAL_INT(TPROF_ERROR, sample.status);
}

void test_tProf_emptyProfiler(void)
{
  TPROF_INIT(sample, 10, STATISTICS_MANUAL);

  tProfCalculateStatistics(&sample);

  TEST_ASSERT_EQUAL_INT(TPROF_STOPPED, sample.status);
}

void test_tProf_statisticsSimple(void)
{
  TPROF_INIT(sample, 10, STATISTICS_MANUAL);
  for (uint32_t i = 0; i < sample.nIncrements + 1; i++)
  {
    tProfReadClock_ExpectAndReturn(1);
    tProfReadClock_ExpectAndReturn(3);
    tProfStart(&sample);
    tProfStop(&sample);
  }
  tProfCalculateStatistics(&sample);
  tProfGetStatistics(&sample, false);
}

void test_tProf_statisticsDetailed(void)
{
  TPROF_INIT(sample, 10, STATISTICS_MANUAL);
  for (uint32_t i = 0; i < sample.nIncrements + 1; i++)
  {
    tProfReadClock_ExpectAndReturn(1);
    tProfReadClock_ExpectAndReturn(i + 1);
    tProfStart(&sample);
    tProfStop(&sample);
  }
  tProfCalculateStatistics(&sample);
  tProfGetStatistics(&sample, true);
}

void test_tProf_statisticsStddevAverage(void)
{
  TPROF_INIT(sample, 10, STATISTICS_MANUAL);
  for (uint32_t i = 0; i < sample.nIncrements + 1; i++)
  {
    tProfReadClock_ExpectAndReturn(1);
    tProfReadClock_ExpectAndReturn(i + 1);
    tProfStart(&sample);
    tProfStop(&sample);
  }
  tProfCalculateStatistics(&sample);
  TEST_ASSERT_DOUBLE_WITHIN(0.01, 2.8722813, sample.tStddev);
  TEST_ASSERT_DOUBLE_WITHIN(0.01, 4.5, sample.tAverage);
}

void test_tProf_statisticsAutolite(void)
{
  TPROF_INIT(sample, 10, STATISTICS_AUTOLITE);
  for (uint32_t i = 0; i < sample.nIncrements; i++)
  {
    tProfReadClock_ExpectAndReturn(1);
    tProfReadClock_ExpectAndReturn(i + 1);
    tProfStart(&sample);
    tProfStop(&sample);
  }
}

void test_tProf_statisticsAutofull(void)
{
  TPROF_INIT(sample, 10, STATISTICS_AUTOFULL);
  for (uint32_t i = 0; i < sample.nIncrements; i++)
  {
    tProfReadClock_ExpectAndReturn(1);
    tProfReadClock_ExpectAndReturn(i + 1);
    tProfStart(&sample);
    tProfStop(&sample);
  }
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_tProf_init);
  RUN_TEST(test_tProf_stats_maxmin);
  RUN_TEST(test_tProf_overflow);
  RUN_TEST(test_tProf_incorrectOperation);
  RUN_TEST(test_tProf_emptyProfiler);

  RUN_TEST(test_tProf_statisticsSimple);
  RUN_TEST(test_tProf_statisticsDetailed);
  RUN_TEST(test_tProf_statisticsStddevAverage);
  RUN_TEST(test_tProf_statisticsAutolite);
  RUN_TEST(test_tProf_statisticsAutofull);

  return UNITY_END();
}