cmake_minimum_required(VERSION 3.10)

find_package(SFML 2.5.1 COMPONENTS system window graphics REQUIRED)

file(GLOB_RECURSE UTILITY_VISUALIZE_TEST_SOURCES "${PROJECT_SOURCE_DIR}/visualization/*/*viz.cpp")

foreach (utilities_visualize_test_source ${UTILITY_VISUALIZE_TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(utilities_visualize_test_filename ${utilities_visualize_test_source} NAME)
    string(REPLACE ".cpp" "" utilities_visulize_test_name ${utilities_visualize_test_filename})

    add_executable(${utilities_visulize_test_name} EXCLUDE_FROM_ALL ${utilities_visualize_test_source})

    target_link_libraries(${utilities_visulize_test_name} sfml-system sfml-window sfml-graphics utilities)

    set_target_properties(${utilities_visulize_test_name}
            PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/viz"
            COMMAND ${utilities_visulize_test_name}
            )
endforeach ()
