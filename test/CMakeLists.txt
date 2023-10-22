# Create an executable called 'tester' from the test.c file

include_directories(../code/include)

add_executable(test_tProf test_timeProfiler.c ../code/src/timeProfiler.c)

# Link the tester executable with the Profiler_Lib and unity static libraries created somewhere else
target_link_libraries(test_tProf Unity_CMock)

# Notify CTest that this is a test (?)
add_test(test_tProf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_tProf)

set(REPORT_FILE coverage_report.txt)

add_custom_target(unittest
    DEPENDS test_tProf
    COMMAND ctest --output-junit ${CMAKE_SOURCE_DIR}/reports/ctest-results.xml
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

add_custom_command(TARGET unittest
    COMMAND echo "Collecting coverage"
    COMMAND gcovr -r ${CMAKE_SOURCE_DIR} -e ${CMAKE_SOURCE_DIR}/test -e ${CMAKE_SOURCE_DIR}/external --html-details ${CMAKE_SOURCE_DIR}/reports/coverage.html --sonarqube ${CMAKE_SOURCE_DIR}/reports/sonar_coverage.xml --txt > ${CMAKE_SOURCE_DIR}/reports/${REPORT_FILE}
    COMMAND gcovr -r ${CMAKE_SOURCE_DIR} -e ${CMAKE_SOURCE_DIR}/test -e ${CMAKE_SOURCE_DIR}/external --txt --branches >> ${CMAKE_SOURCE_DIR}/reports/${REPORT_FILE}
    COMMAND cat ${CMAKE_SOURCE_DIR}/reports/${REPORT_FILE}
    )
