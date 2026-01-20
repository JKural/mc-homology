add_library(core)

target_sources(core
  PRIVATE
    src/cubical_complex.cpp
    src/utils.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/complex.h
      include/cubical_complex.h
      include/homology.h
      include/simplicial_complex.h
      include/utils.h
)

target_compile_features(core PUBLIC cxx_std_23)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
