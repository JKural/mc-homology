add_library(algebra INTERFACE)

target_sources(algebra
  INTERFACE
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/algebraic_concepts.h
      include/chain_complex.h
      include/integer.h
      include/matrix.h
      include/matrix_algorithms.h
      include/modulo_fields.h
      include/number_theory.h
      include/z2_field.h
      include/detail/matrix_utils.h
)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
