#ifndef GRAPHICMENU_H
#define GRAPHICMENU_H

#include <QGraphicsScene>
#include <QLineEdit>
#include "graphicbutton.h"

#define GS_WIDTH 8*GB_WIDTH
#define GS_HEIGHT 15*GB_HEIGHT
#define PENTAGO_DEFAULT_PORT 8082

class GraphicMenu: public QGraphicsScene
{
    Q_OBJECT
public:
    GraphicMenu(qreal x = -GS_WIDTH/2, qreal y = -GS_HEIGHT/2, qreal width = GS_WIDTH, qreal height = GS_HEIGHT, QObject * parent = 0);
    ~GraphicMenu();

private slots:
    void on_New_game_pressed();
    void on_One_player_pressed();
    void on_Two_player_pressed();
    void on_Load_game_pressed();
    void on_Join_game_pressed();
    void on_Join_ok_pressed();
    void on_Host_game_pressed();
    void on_Back_pressed();

signals:
    void Exit_game_pressed();
    void New_game_selected(int player_count);
    void Join_game_selected(std::string host_address);
    void Host_game_selected(std::string host_port);
private:
    QGraphicsItem   *main_menu_parent,
                    *new_menu_parent,
                    *load_menu_parent,
                    *join_menu_parent,
                    *host_menu_parent;
    GraphicButton   *bt_New,
                    *bt_New_one,
                    *bt_New_two,
                    *bt_Load,//
                    *bt_Join,
                    *bt_Join_ok,
                    *bt_Host,
                    *bt_Exit,
                    *bt_Back;
    QLineEdit   *w_line_edit_IP;
};

#endif // GRAPHICMENU_H
