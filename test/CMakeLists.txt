include_directories(../examples mocks)

add_executable(test_tProf 
  test_timeProfiler.c
  ../examples/example_statistics_printf.c
  mocks/MocktimeProfiler_clock.c
  )

target_link_libraries(test_tProf timeProfiler unity cmock m)

add_test(test_tProf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_tProf)

set(REPORT_FILE coverage_report.txt)

add_custom_target(test_coverage
    DEPENDS test_tProf
    COMMAND find ./ -name "*.gcda" -exec rm {} "\;"
    COMMAND ctest -V --output-on-failure --output-junit ${CMAKE_SOURCE_DIR}/reports/ctest-results.xml
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

add_custom_command(TARGET test_coverage POST_BUILD
    COMMAND echo "Collecting coverage"
    COMMAND gcovr -r ${CMAKE_SOURCE_DIR} -e ${CMAKE_SOURCE_DIR}/test -e ${CMAKE_SOURCE_DIR}/.deps --html-details ${CMAKE_SOURCE_DIR}/reports/coverage.html --sonarqube ${CMAKE_SOURCE_DIR}/reports/sonar_coverage.xml --txt > ${CMAKE_SOURCE_DIR}/reports/${REPORT_FILE}
    COMMAND gcovr -r ${CMAKE_SOURCE_DIR} -e ${CMAKE_SOURCE_DIR}/test -e ${CMAKE_SOURCE_DIR}/.deps --txt --txt-metric branch >> ${CMAKE_SOURCE_DIR}/reports/${REPORT_FILE}
    COMMAND cat ${CMAKE_SOURCE_DIR}/reports/${REPORT_FILE}
    )
