#include "view.h"

View::View(QWidget *parent):
    QGraphicsView(parent)
{
    setGeometry(300,300,GS_WIDTH,GS_HEIGHT);
    menu_scene = new GraphicMenu(-width()/2,-height()/2,width(),height(),this);
    board_scene = new GraphicBoard(-width()/2,-height()/2,width(),height(),this);

    QIcon icon(QPixmap(":/Graphic_source/icon.png"));
    setWindowIcon(icon);

    setWindowTitle("Pentago");
    setFixedSize(this->size());//TODO:equally resize
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setCacheMode(QGraphicsView::CacheBackground);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    connect(menu_scene, SIGNAL(New_game_selected(int)), this, SIGNAL(New_game(int)));
    connect(menu_scene, SIGNAL(Join_game_selected(std::string)), this, SIGNAL(Join_game(std::string)));
    connect(menu_scene, SIGNAL(Host_game_selected(std::string)), this, SIGNAL(Host_game(std::string)));
    connect(menu_scene, SIGNAL(Exit_game_pressed()), this, SLOT(close()));
    connect(menu_scene, SIGNAL(Load_game_selected(std::string)),this,SIGNAL(Load_game(std::string)));

    connect(board_scene, SIGNAL(quadrant_rotated(IView::quadrant,IView::turn)), this, SIGNAL(Rotate(IView::quadrant,IView::turn)));
    connect(board_scene, SIGNAL(Stone_clicked(int, int)), this, SIGNAL(Put_stone(int,int)));
    connect(board_scene, SIGNAL(leave_game()),this,SLOT(on_leave_game()));
    connect(board_scene, SIGNAL(save_game(QString)),this,SLOT(on_save_game(QString)));

    connect(board_scene, SIGNAL(message_sended(QString)), this, SLOT(on_msg_sended(QString)));

    Set_control_settings(View::MENU);//it must be called in presenter after view constructing
}

void View::resizeEvent(QResizeEvent *event)// but now size is fixed
{
    QGraphicsView::resizeEvent(event);
    fitInView(sceneRect(),Qt::KeepAspectRatio);
}


void View::Set_control_settings(control_setting param)
{
    switch(param){
    case View::MENU:{
        QPixmap menu_bg(":/Graphic_source/b_ground.jpg");
        setBackgroundBrush(menu_bg.scaled(GS_WIDTH/2,GS_HEIGHT/2,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        setScene(menu_scene);
        break;}
    case View::LOCAL_GAME:{
        setScene(board_scene);
        board_scene->enable_save(true);
        break;}
    case View::NETWORK_GAME:{
        QPixmap menu_bg(":/Graphic_source/b_ground_game.jpg");
        setBackgroundBrush(menu_bg.scaled(GS_WIDTH,GS_HEIGHT,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
        setScene(board_scene);
        board_scene->enable_save(false);
        break;}
    default:
        break;
    }
}
void View::Draw_stone(int row, int col, color c)
{
    board_scene->show_message("S<" + QString::number(row) + ','+QString::number(col)+'>'+((c==IView::BLACK)?": Black":": White"));
    board_scene->draw_stone(row, col, ((c==IView::BLACK)?Qt::black:Qt::white));
}

void View::Show_message(string text)
{
    board_scene->show_message(QString::fromStdString(text));
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

View::~View()
{
    delete menu_scene;
    delete board_scene;
}
