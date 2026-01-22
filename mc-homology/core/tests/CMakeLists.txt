add_executable(core_test)

target_sources(core_test
  PRIVATE
    polymorphic_test.cpp
)

target_link_libraries(core_test
  PRIVATE
    core
    GTest::gtest_main
)

gtest_discover_tests(core_test)
