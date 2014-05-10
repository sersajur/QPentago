#ifndef GRAPHICQUADRANT_H
#define GRAPHICQUADRANT_H
#include <QGraphicsWidget>
#include <QState>
#include <QStateMachine>

class GraphicQuadrant: public QGraphicsWidget
{
    Q_OBJECT
public:
    GraphicQuadrant();
    GraphicQuadrant(qreal x, qreal y, qreal length, QPointF vector, int num, QGraphicsItem* parent = 0);
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPointF getVirginPos() const {return _virgin_pos;}
    qreal getLength(){return _length;}
protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    qreal _duration;
    QPointF _vec, __p0, __p1;
    int _num;
    qreal _length;
    QPointF _virgin_pos;
};

#endif // GRAPHICQUADRANT_H
