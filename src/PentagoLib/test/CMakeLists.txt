project(PentagoLibTests)

set (SOURCE
  BoardTests.cpp
  main.cpp
)

add_executable(${PROJECT_NAME}
  ${SOURCE}
)

target_link_libraries(${PROJECT_NAME}
  gtest
  PentagoLib
)
