project(WidgetsApp)

find_package(Qt5Widgets REQUIRED)

set (SOURCE
  View/graphicboard.cpp
  View/graphicboard.h
  View/graphicbutton.cpp
  View/graphicbutton.h
  View/graphicmenu.cpp
  View/graphicmenu.h
  View/graphicquadrant.cpp
  View/graphicquadrant.h
  View/graphicstone.cpp
  View/graphicstone.h
  Game.cpp
  Game.h
  main.cpp
  View.cpp
  View.h
)

set (RESOURCE
  View/source.qrc
)

add_executable(${PROJECT_NAME}
  ${SOURCE}
  ${RESOURCE}
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# The Qt5Widgets_LIBRARIES variable also includes QtGui and QtCore
target_link_libraries(${PROJECT_NAME}
  Qt5::Widgets
  PentagoLib
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES
    AUTOMOC ON
    AUTORCC ON
)
