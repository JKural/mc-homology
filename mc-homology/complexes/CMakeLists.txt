add_library(complexes)

target_sources(complexes
  PRIVATE
    src/cubical_complex.cpp
    src/utils.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/complexes/cubical_complex.h
      include/complexes/utils.h
)

target_compile_features(complexes PUBLIC cxx_std_23)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

