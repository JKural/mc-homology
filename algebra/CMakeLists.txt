add_library(algebra)

target_sources(algebra
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/algebraic_concepts.h
      include/matrix.h
      include/modulo_fields.h
      include/number_theory.h
      include/z2_field.h
)

target_compile_features(algebra PUBLIC cxx_std_23)
set_target_properties(algebra
  PROPERTIES
    LINKER_LANGUAGE CXX
)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
