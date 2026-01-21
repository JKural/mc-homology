add_executable(helloworld_test)

target_sources(helloworld_test
  PRIVATE
    test.cpp
)

target_link_libraries(helloworld_test
  PRIVATE
    GTest::gtest_main
)

gtest_discover_tests(helloworld_test)
