#include "View.h"

#include <QGraphicsView>
#include <QResizeEvent>

#include "View/graphicmenu.h"
#include "View/graphicboard.h"

namespace
{

class AutoFittableGraphicsView: public QGraphicsView
{
public:
    using QGraphicsView::QGraphicsView;

protected:
    void resizeEvent(QResizeEvent *event) override
    {
        QGraphicsView::resizeEvent(event);
        fitInView(sceneRect(),Qt::KeepAspectRatio);
    }
};

} // anonymous

View::View()
    : view(new AutoFittableGraphicsView)
{
    view->setGeometry(300,300,GS_WIDTH,GS_HEIGHT);
    menu_scene = new GraphicMenu(-view->width()/2,-view->height()/2,view->width(),view->height(),view);
    board_scene = new GraphicBoard(-view->width()/2,-view->height()/2,view->width(),view->height(),view);

    view->setWindowIcon(QPixmap(":/Graphic_source/icon.png"));

    view->setWindowTitle("Pentago");
    view->setFixedSize(view->size());//TODO:equally resize
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    connect(menu_scene, SIGNAL(New_game_selected(int)), this, SIGNAL(New_game(int)));
    connect(menu_scene, SIGNAL(Join_game_selected(std::string)), this, SIGNAL(Join_game(std::string)));
    connect(menu_scene, SIGNAL(Host_game_selected(std::string)), this, SIGNAL(Host_game(std::string)));
    connect(menu_scene, SIGNAL(Exit_game_pressed()), view, SLOT(close()));
    connect(menu_scene, SIGNAL(Load_game_selected(std::string)),this,SIGNAL(Load_game(std::string)));

    connect(board_scene, SIGNAL(quadrant_rotated(IView::quadrant,IView::turn)), this, SIGNAL(Rotate(IView::quadrant,IView::turn)));
    connect(board_scene, SIGNAL(Stone_clicked(int, int)), this, SIGNAL(Put_stone(int,int)));
    connect(board_scene, SIGNAL(leave_game()),this,SLOT(on_leave_game()));
    connect(board_scene, SIGNAL(save_game(QString)),this,SLOT(on_save_game(QString)));

    connect(board_scene, SIGNAL(message_sended(QString)), this, SLOT(on_msg_sended(QString)));

    connect(menu_scene, SIGNAL(game_go_to_start()),board_scene, SLOT(clean_board()));
    connect(menu_scene, SIGNAL(game_go_to_start()),board_scene, SLOT(clean_log()));
    Set_control_settings(View::MENU);//it must be called in presenter after view constructing
}

View::~View()
{
    delete view;
}

void View::Do_Set_control_settings(IView::control_setting param)
{
    switch(param){
    case View::MENU:{
        QPixmap menu_bg(":/Graphic_source/b_ground.jpg");
        view->setBackgroundBrush(menu_bg.scaled(GS_WIDTH/2,GS_HEIGHT/2,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        view->setScene(menu_scene);
        break;}
    case View::LOCAL_GAME:{
        view->setScene(board_scene);
        board_scene->enable_save(true);
        board_scene->setGamePhase(GraphicBoard::WAIT_STONE);
        break;}
    case View::NETWORK_GAME:{
        QPixmap menu_bg(":/Graphic_source/b_ground_game.jpg");
        view->setBackgroundBrush(menu_bg.scaled(GS_WIDTH,GS_HEIGHT,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        view->setScene(board_scene);
        board_scene->enable_save(false);
        board_scene->setGamePhase(GraphicBoard::WAIT_STONE);//
        break;}
    default:
        break;
    }
}
void View::Do_Draw_stone(int row, int col, IView::color c)
{
    board_scene->show_message("S<" + QString::number(row) + ','+QString::number(col)+'>'+((c==IView::BLACK)?": Black":": White"));
    board_scene->draw_stone(row, col, ((c==IView::BLACK)?Qt::black:Qt::white));
}

void View::Do_Show_message(std::string text)
{
    board_scene->show_message(QString::fromStdString(text));
}

void View::ShowWindow()
{
    view->show();
}

void View::on_msg_sended(QString str)
{
    emit Mesage_sended(str.toStdString());
}

void View::on_save_game(QString path)
{
    emit Save_game(path.toStdString());
}

void View::on_leave_game()
{
    Set_control_settings(IView::MENU);// ?it must be called in presenter?
    emit Leave();
}
