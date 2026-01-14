add_executable(algebra_test)

target_sources(algebra_test
  PRIVATE
    algebraic_concepts_test.cpp
    modulo_fields_test.cpp
    number_theory_test.cpp
)

target_link_libraries(algebra_test
  PRIVATE
    algebra
    GTest::gtest_main
)

gtest_discover_tests(algebra_test)
