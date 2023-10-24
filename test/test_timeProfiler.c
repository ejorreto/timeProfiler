#include "timeProfiler.h"
#include "unity.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

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
  TEST_ASSERT_EQUAL_UINT32(0, sample.tStopUsec);
  TEST_ASSERT_EQUAL_UINT32(0, sample.currentIndex);
  TEST_ASSERT_EQUAL_UINT32(10, sample.nIncrements);
  TEST_ASSERT_EQUAL_STRING("sample", sample.name);
  TEST_ASSERT_EQUAL_UINT32(TPROF_STOPPED, sample.status);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_tProf_foo);
  RUN_TEST(test_tProf_init);
  return UNITY_END();
}