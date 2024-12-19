add_library(
    utilities_cache
    OBJECT
    cache.cpp
    replacement_policy_factory.cpp
    lru_replacement_policy.cpp
    lruk_replacement_policy.cpp
    lfu_replacement_policy.cpp)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:utilities_cache>
    PARENT_SCOPE)
