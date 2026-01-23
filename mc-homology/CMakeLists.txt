add_executable(mc-homology)

target_sources(mc-homology
  PRIVATE
    src/main.cpp
)

target_link_libraries(mc-homology PRIVATE core)

target_compile_features(mc-homology
  PRIVATE
    cxx_std_23
)

add_subdirectory(algebra)
add_subdirectory(complexes)
add_subdirectory(core)
