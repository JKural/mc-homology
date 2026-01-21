add_library(core)

target_sources(core
  PRIVATE
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/complex.h
      include/homology.h
)

target_compile_features(core PUBLIC cxx_std_23)
set_target_properties(core
  PROPERTIES
    LINKER_LANGUAGE CXX
)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
