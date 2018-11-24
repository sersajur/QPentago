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

QtView::QtView()
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

    connect(menu_scene, SIGNAL(New_game_selected(int)), this, SIGNAL(newGame(int)));
    connect(menu_scene, SIGNAL(Join_game_selected(std::string)), this, SIGNAL(joinGame(std::string)));
    connect(menu_scene, SIGNAL(Host_game_selected(std::string)), this, SIGNAL(hostGame(std::string)));
    connect(menu_scene, SIGNAL(Exit_game_pressed()), view, SLOT(close()));
    connect(menu_scene, SIGNAL(Load_game_selected(std::string)),this,SIGNAL(loadGame(std::string)));

    connect(board_scene, SIGNAL(quadrant_rotated(View::quadrant,View::turn)), this, SIGNAL(rotate(View::quadrant,View::turn)));
    connect(board_scene, SIGNAL(Stone_clicked(int, int)), this, SIGNAL(putStone(int,int)));
    connect(board_scene, SIGNAL(leave_game()),this,SLOT(on_leave_game()));
    connect(board_scene, SIGNAL(saveGame(QString)),this,SLOT(on_save_game(QString)));

    connect(board_scene, SIGNAL(message_sended(QString)), this, SLOT(on_msg_sended(QString)));

    connect(menu_scene, SIGNAL(game_go_to_start()),board_scene, SLOT(clean_board()));
    connect(menu_scene, SIGNAL(game_go_to_start()),board_scene, SLOT(clean_log()));
    setControlSettings(QtView::Menu);//it must be called in presenter after view constructing
}

QtView::~QtView()
{
    delete view;
}

void QtView::doSetControlSettings(View::ControlSetting param)
{
    switch(param){
    case QtView::Menu:{
        QPixmap menu_bg(":/Graphic_source/b_ground.jpg");
        view->setBackgroundBrush(menu_bg.scaled(GS_WIDTH/2,GS_HEIGHT/2,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        view->setScene(menu_scene);
        break;}
    case QtView::LocalGame:{
        view->setScene(board_scene);
        board_scene->enable_save(true);
        board_scene->setGamePhase(GraphicBoard::WAIT_STONE);
        break;}
    case QtView::NetworkGame:{
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
void QtView::doDrawStone(int row, int col, View::Color c)
{
    board_scene->show_message("S<" + QString::number(row) + ','+QString::number(col)+'>'+((c==View::Color::Black)?": Black":": White"));
    board_scene->drawStone(row, col, ((c==View::Color::Black)?Qt::black:Qt::white));
}

void QtView::doShowMessage(std::string text)
{
    board_scene->show_message(QString::fromStdString(text));
}

void QtView::ShowWindow()
{
    view->show();
}

void QtView::on_msg_sended(QString str)
{
    emit messageSended(str.toStdString());
}

void QtView::on_save_game(QString path)
{
    emit saveGame(path.toStdString());
}

void QtView::on_leave_game()
{
    setControlSettings(View::Menu);// ?it must be called in presenter?
    emit leave();
}
