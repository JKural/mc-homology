add_library(core)

target_sources(core
  PRIVATE
    src/cubical_complex_3d.cpp
    src/homology.cpp
    src/homology_printing_strategy.cpp
    src/latex_wrapper.cpp
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/core/algebra_homology.h
      include/core/complex.h
      include/core/cubical_complex_3d.h
      include/core/homology.h
      include/core/homology_printing_strategy.h
      include/core/latex_wrapper.h
      include/core/parser.h
      include/core/polymorphic.h
      include/core/text_drawable.h
)

target_link_libraries(core PUBLIC algebra complexes)
target_compile_features(core PUBLIC cxx_std_23)
set_target_properties(core
  PROPERTIES
    LINKER_LANGUAGE CXX
)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
