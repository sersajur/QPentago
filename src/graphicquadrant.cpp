#include "graphicquadrant.h"
#include <QPainterPath>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <qmath.h>
#include "graphicboard.h"
#include <QGraphicsSceneMouseEvent>

#define MAX_TOTAL_MOVEMENT 100
#define SIGNUM(x) ((x)?(x)/qAbs<qreal>(x):0)

GraphicQuadrant::GraphicQuadrant()
{

}

GraphicQuadrant::GraphicQuadrant(qreal x, qreal y, qreal length, QPointF vector, int num, QGraphicsItem* parent):
    _duration(0),
    _vec(vector),
    _num(num),
    _length(length),
    _virgin_pos(x,y)
{
    switch(num){
    case 0:
        __p0.setX(0);
        __p0.setY(length);
        __p1.setX(length);
        __p1.setY(0);
        break;
    case 1:
        __p0.setX(length);
        __p0.setY(length);
        __p1.setX(0);
        __p1.setY(0);
        break;
    case 2:
        __p0.setX(length);
        __p0.setY(0);
        __p1.setX(0);
        __p1.setY(length);
        break;
    case 3:
        __p0.setX(0);
        __p0.setY(0);
        __p1.setX(length);
        __p1.setY(length);
        break;
    default:
        break;
    }
    setPos(x,y);
    setParentItem(parent);
    setFlags(QGraphicsItem::ItemSendsGeometryChanges|
             QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
    setZValue(3);
    setTransformOriginPoint(_length/2,_length/2);
}

QRectF GraphicQuadrant::boundingRect() const
{
    return QRectF(0, 0, _length, _length);
}

QPainterPath GraphicQuadrant::shape() const
{
    QPainterPath path;
    path.addRoundedRect(boundingRect(),15,15);
    return path;
}

void GraphicQuadrant::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
    painter->setBrush(QColor(204,0,31));
    painter->drawPath(shape());
    if((acceptedMouseButtons() & Qt::LeftButton) && (flags() & QGraphicsItem::ItemIsMovable) && (option->state & QStyle::State_MouseOver))
    {
        painter->setBrush(QBrush(Qt::green,Qt::Dense7Pattern));
        painter->drawPath(shape());
    }
}

QVariant GraphicQuadrant::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionChange && scene())
    {
        QPointF new_pos = value.toPointF();
        qreal dx = new_pos.x() - x();
        qreal dy = new_pos.y() - y();
        qreal dvalue = (dx*_vec.x() + dy*_vec.y())/qSqrt(_vec.x()*_vec.x()+_vec.y()*_vec.y());
        if(dvalue + _duration >= MAX_TOTAL_MOVEMENT-5)
            dvalue = MAX_TOTAL_MOVEMENT - _duration;
        else if(dvalue + _duration <= 5)
            dvalue = -_duration;
        if(!dvalue)
            return pos();
        _duration+=dvalue;
        dx = SIGNUM(_vec.x()*dvalue)*qSqrt(dvalue*dvalue*_vec.x()*_vec.x()/(_vec.x()*_vec.x()+_vec.y()*_vec.y()));
        dy = SIGNUM(_vec.y()*dvalue)*qSqrt(dvalue*dvalue - dx*dx);
        new_pos.setX(x() + dx);
        new_pos.setY(y() + dy);
        return new_pos;
    }
    return QGraphicsItem::itemChange(change, value);
}

void GraphicQuadrant::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(_duration==MAX_TOTAL_MOVEMENT)
    {
        QPointF a = __p1 - __p0;
        QPointF b = event->pos() - __p0;
        qreal res = a.x()*b.y() - a.y()*b.x();
        if(SIGNUM(res))
            emit dynamic_cast<GraphicBoard*>(scene())->quadrant_clicked_to_rotate(_num, SIGNUM(res));
    }
    emit dynamic_cast<GraphicBoard*>(scene())->returned(_num);
    QGraphicsItem::mouseReleaseEvent(event);
}
