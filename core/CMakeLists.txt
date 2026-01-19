add_library(core)

target_sources(core
  PUBLIC
    FILE_SET HEADER
    BASE_DIRS
      include
    FILES
      include/complex.h
      include/cubical_complex.h
      include/homology.h
      include/simplicial_complex.h
      include/utils.h
)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
