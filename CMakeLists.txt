cmake_minimum_required(VERSION 3.23)

project(mc-homology)

include(CTest)
include(FetchContent)
include(GNUInstallDirs)
include(cmake/InstallLib.cmake)

fetchcontent_declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG 52eb8108c5bdec04579160ae17225d66034bd723
)

fetchcontent_declare(
  mcSavefileParsers
  GIT_REPOSITORY https://github.com/JKural/mcSavefileParsers.git
  GIT_TAG 984843b45a6d465ffc814b94fc698121ab432c25
)

if(BUILD_TESTING)
  fetchcontent_makeavailable(googletest)
  include(GoogleTest)
endif()

fetchcontent_makeavailable(mcSavefileParsers)

add_subdirectory(mc-homology)
