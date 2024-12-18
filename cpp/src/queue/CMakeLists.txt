add_library(
  utilities_queue
  OBJECT
  mpmc_blocking.cpp)

set(ALL_OBJECT_FILES
  ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:utilities_queue>
  PARENT_SCOPE)