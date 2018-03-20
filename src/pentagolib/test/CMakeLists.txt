project(PentagoLibTests)

set (SOURCE
  DummyTest.cpp
  main.cpp
)

add_executable(${PROJECT_NAME}
  ${SOURCE}
)

target_link_libraries(${PROJECT_NAME}
  libgtest
  libgmock
)
