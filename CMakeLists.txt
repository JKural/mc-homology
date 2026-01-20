cmake_minimum_required(VERSION 3.23)

project(mc-homology)

include(CTest)
include(FetchContent)

fetchcontent_declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG 52eb8108c5bdec04579160ae17225d66034bd723
)

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

if(BUILD_TESTING)
  fetchcontent_makeavailable(googletest)
  include(GoogleTest)
  add_subdirectory(tests)
endif()

add_subdirectory(algebra)
add_subdirectory(complexes)
add_subdirectory(core)
add_subdirectory(parsers)

