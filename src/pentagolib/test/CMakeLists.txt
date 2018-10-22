project(PentagoLibTests)

add_executable(${PROJECT_NAME}
  DummyTest.cpp
)

target_link_libraries(${PROJECT_NAME}
  gtest gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})
