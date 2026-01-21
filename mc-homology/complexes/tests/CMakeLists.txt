add_executable(complexes_test)

target_sources(complexes_test
  PRIVATE
    cubical_complex_test.cpp
)

target_link_libraries(complexes_test
  PRIVATE
    complexes
    GTest::gtest_main
)

gtest_discover_tests(complexes_test)
