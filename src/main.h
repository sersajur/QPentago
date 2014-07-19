#ifndef MAIN_H
#define MAIN_H

#include "iview.h"

#include <QObject>

class PresenterEmulator: QObject {
  Q_OBJECT

  IView & view;
  using GAME_LAYOUT = IView::GAME_LAYOUT;
  using LOBBY_STATUS = IView::LOBBY_STATUS;
  using GAME_MODE = IView::GAME_MODE;

  using QUADRANT = IView::QUADRANT;
  using DIRECTION = IView::DIRECTION;

public slots:
  void Put_stone(int row, int col) {
    view.Put_stone(row,col,0x007f00);
    view.Enable_rotate_quadrant();
  }

  void Rotate_quadrant(QUADRANT q,DIRECTION d) {
    view.Rotate_quadrant(q,d);
    view.Disable_rotate_quadrant();
  }

  void Set_game_layout(GAME_LAYOUT layout) {
    if(layout==GAME_LAYOUT::LOBBY) {
        view.Request_show_lobby(2);
      } else if (layout==GAME_LAYOUT::GAME) {
        view.Clear_board();
      }
    else view.Set_game_layout(layout);
  }

  void Show_lobby(int players_count) {
    view.Set_lobby_params(LOBBY_STATUS::NORMAL,L"New game",players_count);
    view.Set_game_layout(GAME_LAYOUT::LOBBY);
  }


public:
  PresenterEmulator(IView& v): view(v) {
    setObjectName("Tests");

    view.Set_game_layout(GAME_LAYOUT::MAIN_MENU);
    connect(&view,SIGNAL(Request_enter_game_layout(GAME_LAYOUT)),SLOT(Set_game_layout(GAME_LAYOUT)));
    view.connect(&view,SIGNAL(Request_set_game_mode(GAME_MODE)),SLOT(Set_game_mode(GAME_MODE)));
    connect(&view,SIGNAL(Request_put_stone(int,int)),SLOT(Put_stone(int,int)));
    connect(&view,SIGNAL(Request_rotate_quadrant(QUADRANT,DIRECTION)),SLOT(Rotate_quadrant(QUADRANT,DIRECTION)));
    connect(&view,SIGNAL(Request_show_lobby(int)),SLOT(Show_lobby(int)));
  }

  ~PresenterEmulator() {}
};

#endif // MAIN_H
