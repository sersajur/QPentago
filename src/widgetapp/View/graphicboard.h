#ifndef GRAPHICBOARD_H
#define GRAPHICBOARD_H

#include <QGraphicsScene>
#include "graphicquadrant.h"
#include "graphicstone.h"
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QAbstractTransition>
#include <QSignalTransition>
#include <QFinalState>
#include "iview.h"
#include <QTextBrowser>
#include "graphicbutton.h"
#include <QGraphicsProxyWidget>
class GraphicBoard: public QGraphicsScene
{
    Q_OBJECT
public:
    enum GamePhase{WAIT_STONE, WAIT_ROTATION, WAIT_SMTH};
public:
    GraphicBoard(qreal x=0, qreal y=0, qreal width=0, qreal height=0, QObject* parent = 0);
    ~GraphicBoard();
    void draw_stone(int i, int j, QColor col_fill);
    void show_message(const QString& text);
    void enable_save(bool b);
    void setGamePhase(GamePhase phase);
signals:
    void Stone_clicked(int row, int col);
    void quadrant_clicked_to_rotate(int num, int turn);
    void returned(int);
    void animated();
    void quadrant_rotated(IView::quadrant num, IView::turn turn);

    void load_game(QString);
    void leave_game();
    void save_game(QString);
    void message_sended(QString str);
private slots:
    void on_click_to_rotate(int num, int turn);
    void on_returned(int num);
    void on_send_pressed();
    void on_save_pressed();
    void clean_board();
    void clean_log();
private:

private:
    QVector<GraphicQuadrant*> _quadrants;
    QVector<QVector<GraphicStone*> > _stones;

    QStateMachine _machine;
    QVector<QState*> _finish_states;
    QVector<QState*> _start_states;
    QVector<QPropertyAnimation*> _pos_animations;
    QParallelAnimationGroup _animations;
    QAbstractTransition* trans;

    QGraphicsRectItem *menu_panel;
    QTextBrowser message_output;
    QTextEdit message_input;
    GraphicButton *bt_leave,
                  *bt_save,
                  *bt_send;

    GamePhase  _phase;
};

#endif // GRAPHICBOARD_H
