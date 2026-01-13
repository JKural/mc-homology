cmake_minimum_required(VERSION 3.23)
project(mc-homology)

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
