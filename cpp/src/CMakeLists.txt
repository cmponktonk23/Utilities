add_subdirectory(tree)
add_subdirectory(common)

add_library(utilities STATIC ${ALL_OBJECT_FILES})

set(UTILITY_LIBS
        utilities_common
        utilities_tree
        utilities_tree_node
        )

# find_package(Threads REQUIRED)

# set(UTILITY_THIRDPARTY_LIBS
#         utilities_murmur3
#         duckdb_pg_query
#         fmt
#         libfort::fort
#         Threads::Threads
#         )

target_link_libraries(
        utilities
        ${UTILITY_LIBS}
        # ${UTILITY_THIRDPARTY_LIBS}
        )

target_include_directories(
        utilities PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
