project(PentagoLib)

find_package(Qt5Core REQUIRED)

set(HDRS
  include/PentagoLib/Board.h
  include/PentagoLib/iview.h
  include/PentagoLib/Player.h
  include/PentagoLib/Referee.h
  include/PentagoLib/serialization.h
)

set(SOURCE
  src/Board.cpp
  src/Referee.cpp
  src/serialization.cpp
  src/Player.cpp
)

add_library(${PROJECT_NAME}
  ${SOURCE}
  ${HDRS}
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME}
  Qt5::Core
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES
    AUTOMOC ON
)

add_subdirectory(test)
