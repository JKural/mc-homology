add_executable(mc-homology)

target_sources(mc-homology
  PRIVATE
    src/main.cpp
    src/helloworld.cpp

  PRIVATE
    FILE_SET HEADERS
    BASE_DIRS
      include
    FILES
      include/helloworld.h
)

target_compile_features(mc-homology
  PRIVATE
    cxx_std_23
)

add_subdirectory(algebra)
add_subdirectory(complexes)
add_subdirectory(core)
