add_subdirectory(node)

add_library(
    utilities_tree
    OBJECT
    quad_tree.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:utilities_tree>
    PARENT_SCOPE)
