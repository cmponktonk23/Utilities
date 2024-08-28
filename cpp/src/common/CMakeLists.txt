add_library(
  utilities_common
  OBJECT
  geometries/rect.cpp
  geometries/vector2d.cpp)

set(ALL_OBJECT_FILES
  ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:utilities_common>
  PARENT_SCOPE)
