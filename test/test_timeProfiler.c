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

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_tProf_foo);
  RUN_TEST(test_tProf_init);
  RUN_TEST(test_tProf_stats_maxmin);
  return UNITY_END();
}