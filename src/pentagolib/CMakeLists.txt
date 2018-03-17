project(pentagolib)

set(HDRS
  include/pentagolib/Board.h
  include/pentagolib/Game.h
  include/pentagolib/iview.h
  include/pentagolib/Player.h
  include/pentagolib/Referee.h
  include/pentagolib/serialization.h
)

set(SOURCE
  src/Board.cpp
  src/Referee.cpp
  src/serialization.cpp
  src/Game.cpp
  src/Player.cpp
)

add_library(${PROJECT_NAME} ${SOURCE})
