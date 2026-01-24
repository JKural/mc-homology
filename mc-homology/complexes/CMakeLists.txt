add_library(complexes)
add_library(${CMAKE_PROJECT_NAME}::complexes ALIAS complexes)

target_sources(complexes
  PRIVATE
    src/cubical_complex.cpp
    src/utils.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/complexes/compute_chain_complex.h
      include/complexes/cubical_complex.h
      include/complexes/utils.h
)

target_link_libraries(complexes PUBLIC algebra)
target_compile_features(complexes PUBLIC cxx_std_23)

install_lib(complexes)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()

