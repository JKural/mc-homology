add_library(algebra INTERFACE)

target_sources(algebra
  INTERFACE
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/algebra/algebraic_concepts.h
      include/algebra/chain_complex.h
      include/algebra/integer.h
      include/algebra/matrix.h
      include/algebra/matrix_algorithms.h
      include/algebra/modulo_fields.h
      include/algebra/number_theory.h
      include/algebra/z2_field.h
      include/algebra/detail/matrix_utils.h
)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
