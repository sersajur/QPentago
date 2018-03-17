#ifndef VIEW_H
#define VIEW_H
#include <QGraphicsView>
#include "iview.h"
#include "View/graphicmenu.h"
#include "View/graphicboard.h"
#include "qdebug.h"

class View: public QGraphicsView, public IView
{
    Q_OBJECT
public:
    explicit View(QWidget* parent = 0);
    virtual ~View();
public slots:
    void Set_control_settings(IView::control_setting param);
    void Draw_stone(int row, int col, IView::color c);
    void Show_message(string text);//todo
signals:
    void New_game(int player_count);
    void Save_game(std::string file_name);
    void Load_game(std::string file_name);
    void Join_game(std::string join_connect_data);
    void Host_game(std::string host_connect_data);
    void Put_stone(int row, int col);
    void Rotate(IView::quadrant, IView::turn);
    void Leave();

    void Mesage_sended(std::string);//user type some message
private slots:
    void on_msg_sended(QString str);//QString -> toStdString
    void on_leave_game();
    void on_save_game(QString); //QString -> toStdString
protected:
    void resizeEvent(QResizeEvent *event);
private:
    GraphicMenu* menu_scene;
    GraphicBoard* board_scene;
};

#endif // VIEW_H
