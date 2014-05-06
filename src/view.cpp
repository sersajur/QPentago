#include "view.h"

View::View(QWidget *parent):
    QGraphicsView(parent)
{
    setGeometry(300,300,GS_WIDTH,GS_HEIGHT);
    menu_scene = new GraphicMenu(-width()/2,-height()/2,width(),height(),this);
    setScene(menu_scene);

    QPixmap menu_bg(":/Graphic_source/b_ground.png");
    setBackgroundBrush(menu_bg.scaled(GS_WIDTH/2,GS_HEIGHT/2,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

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

}

void View::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    fitInView(sceneRect(),Qt::KeepAspectRatio);
}


void View::Set_control_settings(control_setting param)
{
    //
}
void View::Draw_stone(int row, int col, color c)
{
    //
}

void View::Show_message(string text)
{
    //
}

View::~View()
{
    delete menu_scene;
}
