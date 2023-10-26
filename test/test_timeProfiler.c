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
#include "timeProfiler.h"
#include "unity.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include <MocktimeProfiler_clock.h>

void setUp(void)
{
  /* Nothing to setup yet */
}

void tearDown(void)
{
  /* Nothing to tear down yet */
}

void test_tProf_foo(void)
{
  TEST_ASSERT_EQUAL(true, tProf_foo());
}

void test_tProf_init(void)
{
  TPROF_INIT(sample, 10);

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
  TPROF_INIT(sample, 10);
  tProfReadClock_ExpectAndReturn(1);
  tProfReadClock_ExpectAndReturn(3);
  tProfStart(&sample);
  tProfStop(&sample);
  tProfReadClock_ExpectAndReturn(1);
  tProfReadClock_ExpectAndReturn(7);
  tProfStart(&sample);
  tProfStop(&sample);
  tProfCalculateStatistics(&sample);
  TEST_ASSERT_EQUAL_UINT32(2, sample.tIncrements[0]);
  TEST_ASSERT_EQUAL_UINT32(6, sample.tIncrements[1]);
  TEST_ASSERT_EQUAL_UINT32(2, sample.tMin);
  TEST_ASSERT_EQUAL_UINT32(6, sample.tMax);
}

void test_tProf_overflow(void)
{
  TPROF_INIT(sample, 10);
  for (uint32_t i = 0; i < sample.nIncrements + 1; i++)
  {
    tProfReadClock_ExpectAndReturn(1);
    tProfReadClock_ExpectAndReturn(3);
    tProfStart(&sample);
    tProfStop(&sample);
  }
  TEST_ASSERT_EQUAL_INT(TPROF_FULL, sample.status);
  TEST_ASSERT_EQUAL_UINT32(sample.nIncrements,sample.currentIndex);
}

void test_tProf_incorrectOperation(void)
{
  TPROF_INIT(sample, 10);

  tProfReadClock_ExpectAndReturn(1);

  tProfStart(&sample);
  tProfCalculateStatistics(&sample);

  TEST_ASSERT_EQUAL_INT(TPROF_ERROR, sample.status);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_tProf_foo);
  RUN_TEST(test_tProf_init);
  RUN_TEST(test_tProf_stats_maxmin);
  RUN_TEST(test_tProf_overflow);
  RUN_TEST(test_tProf_incorrectOperation);
  return UNITY_END();
}