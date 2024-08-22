add_library(
    utilities_tree_node
    OBJECT
    quad_tree_node.cpp
    quad_tree_internal_node.cpp
    quad_tree_leaf_node.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:utilities_tree_node>
    PARENT_SCOPE)
