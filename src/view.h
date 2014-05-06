#ifndef VIEW_H
#define VIEW_H
#include "iview.h"
#include <QGraphicsView>
#include "graphicmenu.h"


class View: public QGraphicsView, public IView
{
    Q_OBJECT
public:
    explicit View(QWidget* parent = 0);
    virtual ~View();
public:
    virtual void Set_control_settings(control_setting param);
    virtual void Draw_stone(int row, int col, color c);
    virtual void Show_message(std::string text);
signals:
    void New_game(int player_count);
    void Save_game(std::string file_name);
    void Load_game(std::string file_name);
    void Join_game(std::string join_connect_data);
    void Host_game(std::string host_connect_data);
    void Put_stone(int row, int col);
    void Rotate(quadrant num, turn t);
    void Leave();
protected:
    void resizeEvent(QResizeEvent *event);
private:
    QGraphicsScene* menu_scene;
};

#endif // VIEW_H
