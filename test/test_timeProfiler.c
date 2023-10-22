#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include "timeProfiler.h"
#include "unity.h"

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

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_tProf_foo);
  return UNITY_END();
}