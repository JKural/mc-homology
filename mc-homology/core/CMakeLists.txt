add_library(core)
add_library(${CMAKE_PROJECT_NAME}::core ALIAS core)

target_sources(core
  PRIVATE
    src/complex.cpp
    src/cubical_complex_3d.cpp
    src/homology.cpp
    src/homology_printing_strategy.cpp
    src/latex_wrapper.cpp
    src/manager.cpp
    src/options.cpp
    src/parser.cpp
    src/text_drawable.cpp
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
      include/core/manager.h
      include/core/options.h
      include/core/parser.h
      include/core/polymorphic.h
      include/core/text_drawable.h
)

target_link_libraries(core
  PUBLIC
    algebra
    complexes
    mcSavefileParsers::mcSavefileParsers
)

target_compile_features(core PUBLIC cxx_std_23)
set_target_properties(core
  PROPERTIES
    LINKER_LANGUAGE CXX
)

install_lib(core)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
