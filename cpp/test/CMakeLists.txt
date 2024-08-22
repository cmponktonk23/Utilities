cmake_minimum_required(VERSION 3.10)

include(GoogleTest)

file(GLOB_RECURSE UTILITY_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*/*test.cpp")

# #####################################################################################################################
# MAKE TARGETS
# #####################################################################################################################

# #########################################
# "make check-tests"
# #########################################
add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)
add_custom_target(check-tests COMMAND ${CMAKE_CTEST_COMMAND} --verbose)
# add_custom_target(check-public-ci-tests COMMAND ${CMAKE_CTEST_COMMAND} --verbose -E "\"SQLLogicTest|Trie|BPlusTreeContentionTest\"")

# #########################################
# "make XYZ_test"
# #########################################
foreach (utilities_test_source ${UTILITY_TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(utilities_test_filename ${utilities_test_source} NAME)
    string(REPLACE ".cpp" "" utilities_test_name ${utilities_test_filename})

    # Add the test target separately and as part of "make check-tests".
    add_executable(${utilities_test_name} EXCLUDE_FROM_ALL ${utilities_test_source})
    add_dependencies(build-tests ${utilities_test_name})
    add_dependencies(check-tests ${utilities_test_name})

    gtest_discover_tests(${utilities_test_name}
            EXTRA_ARGS
            --gtest_color=auto
            --gtest_output=xml:${CMAKE_BINARY_DIR}/test/${utilities_test_name}.xml
            --gtest_catch_exceptions=0
            DISCOVERY_TIMEOUT 120
            PROPERTIES
            TIMEOUT 120
            )

    target_link_libraries(${utilities_test_name} utilities gtest gmock_main)

    # Set test target properties and dependencies.
    set_target_properties(${utilities_test_name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
            COMMAND ${utilities_test_name}
            )
endforeach ()

# set(UTILITY_SLT_SOURCES
#         "${PROJECT_SOURCE_DIR}/test/sql/p0.01-lower-upper.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p0.02-function-error.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p0.03-string-scan.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.01-seqscan.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.02-insert.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.03-update.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.04-delete.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.05-index-scan.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.06-empty-table.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.07-simple-agg.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.08-group-agg-1.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.09-group-agg-2.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.10-simple-join.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.11-multi-way-join.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.12-repeat-execute.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.13-nested-index-join.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.14-hash-join.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.15-multi-way-hash-join.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.16-sort-limit.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.17-topn.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.18-integration-1.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.19-integration-2.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.leaderboard-q1.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.leaderboard-q2.slt"
#         "${PROJECT_SOURCE_DIR}/test/sql/p3.leaderboard-q3.slt"
#         )

# add_custom_target(test-p3 ${CMAKE_CTEST_COMMAND} -R SQLLogicTest)

# #########################################
# "make XYZ_test"
# #########################################
# foreach (utilities_test_source ${UTILITY_SLT_SOURCES})
#     # Create a human readable name.
#     get_filename_component(utilities_test_filename ${utilities_test_source} NAME)
#     string(REPLACE ".slt" "" utilities_filename_wo_suffix "${utilities_test_filename}")
#     string(REPLACE ".slt" "" utilities_test_name "SQLLogicTest.${utilities_filename_wo_suffix}")
#     add_test(NAME ${utilities_test_name} COMMAND "${CMAKE_BINARY_DIR}/bin/utilities-sqllogictest" ${utilities_test_source} --verbose -d --in-memory)
#     add_custom_target(${utilities_filename_wo_suffix}_test COMMAND "${CMAKE_BINARY_DIR}/bin/utilities-sqllogictest" "${utilities_test_source}" --verbose -d --in-memory)
#     add_dependencies(${utilities_filename_wo_suffix}_test sqllogictest)
# endforeach ()

# add_dependencies(test-p3 sqllogictest)

# # Must build sqllogictest before checking tests
# add_dependencies(build-tests sqllogictest)
# add_dependencies(check-tests sqllogictest)
