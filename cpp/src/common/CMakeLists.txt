add_library(
  utilities_common
  OBJECT
  types/rect.cpp
  types/vector2d.cpp)

set(ALL_OBJECT_FILES
  ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:utilities_common>
  PARENT_SCOPE)
