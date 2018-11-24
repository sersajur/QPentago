#ifndef QTVIEW_H
#define QTVIEW_H

#include <PentagoLib/View.h>

QT_FORWARD_DECLARE_CLASS(QGraphicsView)

class GraphicMenu;
class GraphicBoard;

class QtView: public View
{
    Q_OBJECT
public:
    QtView();
    ~QtView() override;

    // View
    void doSetControlSettings(View::ControlSetting param) override;
    void doDrawStone(int row, int col, View::Color c) override;
    void doShowMessage(std::string text) override;//todo

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

#endif // QTVIEW_H
