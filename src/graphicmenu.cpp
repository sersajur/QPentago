#include "graphicmenu.h"
#include <QGraphicsProxyWidget>
#include <graphicquadrant.h>
#include <QApplication>
#define SAVEPATH  (QApplication::applicationDirPath()+"/Save/test")
GraphicMenu::~GraphicMenu()
{
    delete bt_New;
    delete bt_Host;
    delete bt_Join;
    delete bt_Load;
    delete bt_Exit;
    delete bt_New_one;
    delete bt_New_two;
    delete bt_Back;
    delete bt_Join_ok;
    delete w_line_edit_IP;
    delete new_menu_parent;
    delete load_menu_parent;
    delete join_menu_parent;
    delete host_menu_parent;
}

GraphicMenu::GraphicMenu(qreal x, qreal y, qreal width, qreal height, QObject * parent):
    QGraphicsScene(x,y,width,height,parent)
{//main
    main_menu_parent = addRect(-125,-200,250,400,Qt::NoPen,QBrush(Qt::gray,Qt::Dense4Pattern));
    main_menu_parent->setPos(0,0);
    bt_New = new GraphicButton(QString("New game"),main_menu_parent);
    bt_Load = new GraphicButton(QString("Load game"),main_menu_parent);
    bt_Join = new GraphicButton(QString("Join game"),main_menu_parent);
    bt_Host = new GraphicButton(QString("Host game"),main_menu_parent);
    bt_Exit = new GraphicButton(QString("Exit"),main_menu_parent);
    qreal h = bt_New->boundingRect().height()+10;
    bt_New->setPos(0, -2*h);
    bt_Load->setPos(0, -h);
    bt_Join->setPos(0, 0);
    bt_Host->setPos(0, h);
    bt_Exit->setPos(0, 2*h);
//new
    new_menu_parent = new QGraphicsRectItem();
    new_menu_parent->setPos(0,0);
    bt_New_one = new GraphicButton(QString("One Player"),new_menu_parent);
    bt_New_two = new GraphicButton(QString("Two Players"),new_menu_parent);
    bt_New_one->setPos(0, -h);
    bt_New_two->setPos(0, 0);
//load
    load_menu_parent = new QGraphicsRectItem();
    load_menu_parent->setPos(0,0);
//join
    join_menu_parent = new QGraphicsRectItem();
    join_menu_parent->setPos(0,0);
    w_line_edit_IP = new QLineEdit();
    w_line_edit_IP->setMaxLength(20);
    w_line_edit_IP->setFont(QFont("Cursive",12));
    bt_Join_ok = new GraphicButton(QString("OK"));
    QGraphicsProxyWidget* g_line_edit_IP = addWidget(w_line_edit_IP);
    QGraphicsTextItem *g_text = addText("Input host IP:",QFont("Cursive",18));
    g_text->setDefaultTextColor(Qt::white);

    g_text->setPos(-g_line_edit_IP->size().width()/2,0);
    g_line_edit_IP->setPos(-g_line_edit_IP->size().width()/2,h);
    bt_Join_ok->setPos(0,2*h);

    QGraphicsItem *panel = addRect(-g_line_edit_IP->size().width()/2-4, 0, g_line_edit_IP->size().width() + 8,g_line_edit_IP->size().height()+2*h,Qt::NoPen,QBrush(Qt::Dense4Pattern));

    g_line_edit_IP->setParentItem(panel);
    g_text->setParentItem(panel);
    bt_Join_ok->setParentItem(panel);
    panel->setParentItem(join_menu_parent);
//host
    host_menu_parent = new QGraphicsRectItem();
    host_menu_parent->setPos(0,0);

    bt_Back = new GraphicButton(QString("Back"),new_menu_parent);
    bt_Back->setPos(0,4*h);
//connections
    connect(bt_New,SIGNAL(pressed()),this,SLOT(on_New_game_pressed()));
    connect(bt_Load,SIGNAL(pressed()),this,SLOT(on_Load_game_pressed()));
    connect(bt_Join,SIGNAL(pressed()),this,SLOT(on_Join_game_pressed()));
    connect(bt_Host,SIGNAL(pressed()),this,SLOT(on_Host_game_pressed()));
    connect(bt_Exit,SIGNAL(pressed()),this,SIGNAL(Exit_game_pressed()));

    connect(bt_New_one,SIGNAL(pressed()),this,SLOT(on_One_player_pressed()));
    connect(bt_New_two,SIGNAL(pressed()),this,SLOT(on_Two_player_pressed()));
    connect(bt_Join_ok,SIGNAL(pressed()),this,SLOT(on_Join_ok_pressed()));

    connect(bt_Back,SIGNAL(pressed()),this,SLOT(on_Back_pressed()));

    addItem(main_menu_parent);
}

//SLOTS
void GraphicMenu::on_New_game_pressed()
{
    bt_Back->setParentItem(new_menu_parent);
    removeItem(main_menu_parent);
    addItem(new_menu_parent);
    update();
}

void GraphicMenu::on_One_player_pressed()
{
    emit New_game_selected(1);
}

void GraphicMenu::on_Two_player_pressed()
{
    emit New_game_selected(2);
}

void GraphicMenu::on_Load_game_pressed()
{

//    bt_Back->setParentItem(load_menu_parent); //uncomment, when user will be able to make a choice
//    removeItem(main_menu_parent);
//    addItem(load_menu_parent);
//    update();
   emit Load_game_selected(QString(SAVEPATH).toStdString());
}

void GraphicMenu::on_Join_game_pressed()
{
    bt_Back->setParentItem(join_menu_parent);
    removeItem(main_menu_parent);
    addItem(join_menu_parent);
    w_line_edit_IP->clear();
    update();
}

void GraphicMenu::on_Join_ok_pressed()
{
    emit Join_game_selected(QString(QString::number(PENTAGO_DEFAULT_PORT) + ":" + w_line_edit_IP->text()).toStdString());
}

void GraphicMenu::on_Host_game_pressed()
{
    emit Host_game_selected(QString::number(PENTAGO_DEFAULT_PORT).toStdString());
}

void GraphicMenu::on_Back_pressed()
{
    removeItem(bt_Back->parentItem());
    addItem(main_menu_parent);
    update();
}

