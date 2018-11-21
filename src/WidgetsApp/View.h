#ifndef VIEW_H
#define VIEW_H

#include <PentagoLib/iview.h>

QT_FORWARD_DECLARE_CLASS(QGraphicsView)

class GraphicMenu;
class GraphicBoard;

class View: public IView
{
    Q_OBJECT
public:
    View();
    ~View() override;

    // IView
    void Do_Set_control_settings(IView::control_setting param) override;
    void Do_Draw_stone(int row, int col, IView::color c) override;
    void Do_Show_message(string text) override;//todo

    void ShowWindow();

private slots:
    void on_msg_sended(QString str);//QString -> toStdString
    void on_leave_game();
    void on_save_game(QString); //QString -> toStdString

private:
    QGraphicsView *view;
    GraphicMenu* menu_scene;
    GraphicBoard* board_scene;
};

#endif // VIEW_H
