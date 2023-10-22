# Create an executable called 'tester' from the test.c file

include_directories(../code/include)

add_executable(test_tProf test_timeProfiler.c ../code/src/timeProfiler.c)

# Link the tester executable with the Profiler_Lib and unity static libraries created somewhere else
target_link_libraries(test_tProf Unity_CMock)

# Notify CTest that this is a test (?)
add_test(test_tProf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_tProf)