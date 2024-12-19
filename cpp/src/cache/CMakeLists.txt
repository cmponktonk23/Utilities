add_subdirectory(replacement_policies)

add_library(
    utilities_cache
    OBJECT
    cache.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:utilities_cache>
    PARENT_SCOPE)
