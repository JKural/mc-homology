cmake_minimum_required(VERSION 3.23)

project(mc-homology)

include(CTest)
include(FetchContent)

fetchcontent_declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG 52eb8108c5bdec04579160ae17225d66034bd723
)

find_package(ZLIB REQUIRED)

if(BUILD_TESTING)
  fetchcontent_makeavailable(googletest)
  include(GoogleTest)
endif()

add_subdirectory(mc-homology)
add_subdirectory(mcSavefileParsers)
