#include "graphicboard.h"
#include <QPixmap>
#include <QGraphicsItem>
#include <QApplication>

#define SAVEPATH  (QApplication::applicationDirPath()+"/Save/test")

GraphicBoard::~GraphicBoard()
{
    for(int i = 0; i < _stones.size(); i++)
        for(int j = 0; j < _stones[0].size(); j++)
            delete _stones[i][j];

    for(int i = 0; i < _quadrants.size(); i++)
    {
        delete _quadrants[i];
        delete _pos_animations[i];
        delete _start_states[i];
        delete _finish_states[i];
    }
    delete bt_save;
    delete bt_leave;
    delete bt_send;
//    delete menu_panel;
}

GraphicBoard::GraphicBoard(qreal x, qreal y, qreal width, qreal height, QObject *parent):
    QGraphicsScene(x,y,width,height,parent)
{
    QPixmap pixmap(":/Graphic_source/b_ground_game.jpg");
    QGraphicsItem* bg_item = addPixmap(pixmap.scaled(width,height,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    bg_item->setPos(x,y);
    pixmap.load(":/Graphic_source/b_board.png");
    QGraphicsItem* board_base_item = addPixmap(pixmap.scaled(390,390,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    board_base_item->setParentItem(bg_item);
    board_base_item->setPos(height*0.15,height*0.15);

    _quadrants.resize(4);
    _quadrants[0] = new GraphicQuadrant(200, -10, 200, QPointF(1,-1), 0, board_base_item);
    _quadrants[1] = new GraphicQuadrant(-10, -10, 200, QPointF(-1,-1), 1, board_base_item);
    _quadrants[2] = new GraphicQuadrant(-10, 200, 200, QPointF(-1,1), 2, board_base_item);
    _quadrants[3] = new GraphicQuadrant(200, 200, 200, QPointF(1,1), 3, board_base_item);

    _stones.resize(6);
    for(int i = 0; i < 6; i++)
        _stones[i].resize(6);

    qreal dw = 30;
    qreal r = (200 - 3*dw + 10)/6;
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
        {
            _stones[i  ][j+3] = new GraphicStone(Qt::darkRed, i, j+3, r, _quadrants[0]);
            _stones[i  ][j  ] = new GraphicStone(Qt::darkRed, i, j, r, _quadrants[1]);
            _stones[i+3][j  ] = new GraphicStone(Qt::darkRed, i+3, j, r, _quadrants[2]);
            _stones[i+3][j+3] = new GraphicStone(Qt::darkRed, i+3, j+3, r, _quadrants[3]);
            _stones[i  ][j  ]->setPos((dw-10)/2 + r + j*(2*r+dw),(dw-10)/2 + r + i*(2*r+dw));
            _stones[i+3][j  ]->setPos((dw-10)/2 + r + j*(2*r+dw),(dw-10)/2 + r + i*(2*r+dw));
            _stones[i  ][j+3]->setPos((dw-10)/2 + r + j*(2*r+dw),(dw-10)/2 + r + i*(2*r+dw));
            _stones[i+3][j+3]->setPos((dw-10)/2 + r + j*(2*r+dw),(dw-10)/2 + r + i*(2*r+dw));
        }
    //create menu panel
    menu_panel = addRect(0,0,300,sceneRect().height(),Qt::NoPen,QBrush(Qt::Dense4Pattern));
    menu_panel->setPos(sceneRect().x()+sceneRect().width()-300,sceneRect().y());

    QGraphicsProxyWidget* gw_msg_io = addWidget(&message_output);
    gw_msg_io->setParentItem(menu_panel);
    message_output.setGeometry(10,10,300-20,300);
    gw_msg_io = addWidget(&message_input);
    gw_msg_io->setParentItem(menu_panel);
    message_input.setGeometry(10,message_output.height()+20,300-20,100);
    message_output.setFont(QFont("Cursive", 16));
    message_input.setFont(QFont("Cursive", 16));
    bt_send = new GraphicButton(QString("Send"),menu_panel);
    bt_save = new GraphicButton(QString("Save"),menu_panel);
    bt_leave = new GraphicButton(QString("Leave"),menu_panel);
    bt_send->setPos(300 - (10 + bt_send->boundingRect().width()/2), message_input.y()+message_input.height()+bt_send->boundingRect().height()+5);
    bt_save->setPos(10+bt_leave->boundingRect().width()/2,menu_panel->rect().height() - bt_save->boundingRect().height()/2- 10);
    bt_leave->setPos(300 - (10 + bt_leave->boundingRect().width()/2), menu_panel->rect().height() - bt_leave->boundingRect().height()/2 - 10);
    
    //here board_base_item may be scaled and scaling also changes childrens

    connect(this, SIGNAL(quadrant_clicked_to_rotate(int,int)), this, SLOT(on_click_to_rotate(int,int)));
    connect(this, SIGNAL(returned(int)),this, SLOT(on_returned(int)));
    connect(bt_leave, SIGNAL(pressed()),this,SIGNAL(leave_game()));
    connect(bt_send, SIGNAL(pressed()),this,SLOT(on_send_pressed()));
    connect(bt_save, SIGNAL(pressed()),this,SLOT(on_save_pressed()));

    //---begin fucking irrationality
    _pos_animations.resize(_quadrants.size());
    _start_states.resize(_quadrants.size());
    _finish_states.resize(_quadrants.size());
    for(int i=0; i < _quadrants.size(); i++)
    {
        _start_states[i] = new QState;
        _finish_states[i] = new QState;
        _pos_animations[i] = new QPropertyAnimation(_quadrants[i],"pos");
        _start_states[i]->assignProperty(_quadrants[i], "pos", _quadrants[0]->pos());
        _finish_states[i]->assignProperty(_quadrants[i], "pos", _quadrants[0]->getVirginPos());
        _machine.addState(_finish_states[i]);
        _machine.addState(_start_states[i]);
        _pos_animations[i]->setTargetObject(_quadrants[i]);
        _pos_animations[i]->setPropertyName("pos");
        _pos_animations[i]->setDuration(1000);
        _pos_animations[i]->setEasingCurve(QEasingCurve::InOutBack);
        _animations.addAnimation(_pos_animations[i]);
        trans = _start_states[i]->addTransition(this,SIGNAL(animated()),_finish_states[i]);
        trans->addAnimation(&_animations);
    }
    //---end fucking irrationality

    _setGamePhase(GraphicBoard::GamePhase(WAIT_STONE));
}

void GraphicBoard::enable_save(bool b)
{
    bt_save->setVisible(b);
    bt_save->setEnabled(b);
}

void GraphicBoard::on_send_pressed()
{
    QString str = message_input.toPlainText().trimmed();
    if(str!="")
    {
        message_output.setTextColor(Qt::green);
        message_output.append(">> " + str);
        emit message_sended(str);
    }
    message_input.clear();
}

void GraphicBoard::on_save_pressed()
{
    show_message("Saving game...");
    emit save_game(SAVEPATH);
}

void GraphicBoard::draw_stone(int i, int j, QColor col_fill)
{
    _stones[i][j]->Fill(col_fill);
    _setGamePhase(WAIT_ROTATION);
    update();
}

void GraphicBoard::show_message(const QString &text)
{
    message_output.setTextColor(Qt::blue);
    message_output.append(">> " + text);
}

void GraphicBoard::on_click_to_rotate(int num, int turn)
{
    int i0 = (num < 2)? 0 : 3;
    int j0 = ((!num) || (num == 3))? 3 : 0;
    GraphicStone* tmp;
    QVector<QVector<QPointF> > tmp_pos;
    tmp_pos.resize(3);
    for(int i = 0; i<3; i++){
        tmp_pos[i].resize(3);
        for(int j = 0; j<3; j++)
            tmp_pos[i][j] = _stones[i0+i][j0+j]->pos();
    }
    if(turn>0)
        for(int j = 0; j < 2; j++)
        {
            tmp = _stones[i0][j0+j];
            _stones[i0][j0+j] = _stones[i0 + 3 - 1 - j][j0];           
            _stones[i0 + 3 - 1 - j][j0] = _stones[i0 + 3 - 1][j0 + 2 - j];
            _stones[i0 + 3 - 1][j0 + 2 - j] = _stones[i0 + j][j0 + 2];
            _stones[i0 + j][j0 + 2] = tmp;
        }
    else
        for(int j = 0; j < 2; j++)
        {
            tmp = _stones[i0][j0+j];
            _stones[i0][j0+j] = _stones[i0 + j][j0 + 2];
            _stones[i0 + j][j0 + 2] = _stones[i0 + 3 - 1][j0 + 2 - j];
            _stones[i0 + 3 - 1][j0 + 2 - j] = _stones[i0 + 3 - 1 - j][j0];
            _stones[i0 + 3 - 1 - j][j0] = tmp;
        }
    for (int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++)
           _stones[i0 + i][j0 + j]->setId(i0 + i,j0 + j);
    for(int i = 0; i<3; i++)
        for(int j = 0; j<3; j++)
           _stones[i0+i][j0+j]->setPos(tmp_pos[i][j]);
    update();
    show_message("Q<"+QString::number(num)+">: "+((turn<0)?"Left":"Right"));
    emit quadrant_rotated(IView::quadrant(num+1), IView::turn(((turn<0)?0:1)));
    _setGamePhase(GraphicBoard::WAIT_STONE); //actualy, here must be WAIT_SMTH, and WAIT_STONE must be emited by PRESENTER-->VIEW
}

void GraphicBoard::on_returned(int num)//repair it
{
    _machine.setInitialState(_start_states[num]);
    _machine.start();
    animated();//smth wrong, it dosn't work
    _machine.stop();
}

void GraphicBoard::_setGamePhase(GamePhase phase)
{
    switch(phase){
    case GraphicBoard::WAIT_STONE:
        for(int i = 0; i < _quadrants.size(); i++)
            _quadrants[i]->setAcceptedMouseButtons(Qt::NoButton);
        for(int i = 0; i < _stones.size(); i++)
            for(int j = 0; j < _stones[0].size(); j++)
                _stones[i][j]->setAcceptedMouseButtons(Qt::LeftButton);
        break;
    case GraphicBoard::WAIT_ROTATION:
        for(int i = 0; i < _quadrants.size(); i++)
            _quadrants[i]->setAcceptedMouseButtons(Qt::LeftButton);
        for(int i = 0; i < _stones.size(); i++)
            for(int j = 0; j < _stones[0].size(); j++)
                _stones[i][j]->setAcceptedMouseButtons(Qt::NoButton);
        break;
    case GraphicBoard::WAIT_SMTH://disable hall board abilities
        for(int i = 0; i < _quadrants.size(); i++)
            _quadrants[i]->setAcceptedMouseButtons(Qt::NoButton);
        for(int i = 0; i < _stones.size(); i++)
            for(int j = 0; j < _stones[0].size(); j++)
                _stones[i][j]->setAcceptedMouseButtons(Qt::NoButton);
        break;
    default://enable hall board abilities
        for(int i = 0; i < _quadrants.size(); i++)
            _quadrants[i]->setAcceptedMouseButtons(Qt::LeftButton);
        for(int i = 0; i < _stones.size(); i++)
            for(int j = 0; j < _stones[0].size(); j++)
                _stones[i][j]->setAcceptedMouseButtons(Qt::LeftButton);
        break;
    }
}
