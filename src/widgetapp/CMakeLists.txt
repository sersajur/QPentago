project(Pentago)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets REQUIRED)

# Tell CMake to run moc when necessary:
set(CMAKE_AUTOMOC ON)

# Tell CMake to run rcc when necessary:
set(CMAKE_AUTORCC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
#include_directories(${Qt5Widgets_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
#add_definitions(${Qt5Widgets_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
#set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")


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
  main.cpp
  View.cpp
  View.h
)

set (RESOURCE
  View/source.qrc
)

add_executable(${PROJECT_NAME} ${SOURCE} ${RESOURCE})

# The Qt5Widgets_LIBRARIES variable also includes QtGui and QtCore
target_link_libraries(${PROJECT_NAME}
  Qt5::Widgets
  pentagolib
)
