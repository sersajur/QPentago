#include "graphicstone.h"
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <graphicboard.h>

GraphicStone::GraphicStone(QGraphicsItem* parent):
    QGraphicsWidget(parent),
    _default_color(232,149,166),
    _fill_color(_default_color),
    _fill_flag(false)

{
    setAcceptHoverEvents(true);
    setCacheMode(DeviceCoordinateCache);
}

GraphicStone::GraphicStone(const QColor& default_col, int i, int j, qreal radius, QGraphicsItem* parent):
    QGraphicsWidget(parent),
    _default_color(default_col),
    _fill_color(default_col),
    _fill_flag(false),
    _i(i),
    _j(j),
    _radius(radius)
{
    setAcceptHoverEvents(true);
    setCacheMode(DeviceCoordinateCache);
    setZValue(4);
    connect(this,SIGNAL(clicked()),this,SLOT(on_click()));//
}

GraphicStone::GraphicStone(const QColor& default_col, const QColor& fill_col, QGraphicsItem* parent):
    QGraphicsWidget(parent),
    _default_color(default_col),
    _fill_color(fill_col),
    _fill_flag(true)

{
    setAcceptHoverEvents(true);
    setCacheMode(DeviceCoordinateCache);
}

void GraphicStone::Fill(const QColor &fill_col)
{
    _fill_color = fill_col;
    _fill_flag = true;
    update();
}

void GraphicStone::Empty()
{
    _fill_color = _default_color;
    _fill_flag = false;
    update();
}

QRectF GraphicStone::boundingRect() const
{
    return QRectF(-_radius,-_radius,2*_radius,2*_radius);
}

QPainterPath GraphicStone::shape() const
{
    QPainterPath path;
    path.addEllipse(boundingRect());
    return path;
}

void GraphicStone::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
    QRectF r = boundingRect();
    QRadialGradient grad(r.center(),r.height(),r.topLeft());
    grad.setSpread(QGradient::PadSpread);
    grad.setColorAt(_fill_flag ? 1 : 0, _fill_flag ? _fill_color : _default_color);
    grad.setColorAt(_fill_flag ? 0 : 1, _fill_flag ? Qt::darkGray : Qt::white );
    painter->setPen(Qt::NoPen);
    painter->setBrush(grad);
    painter->drawPath(shape());

    if((acceptedMouseButtons() & Qt::LeftButton) && (!_fill_flag) && (option->state & QStyle::State_MouseOver))
    {
        painter->setBrush(QBrush(Qt::green,Qt::Dense7Pattern));
        painter->drawPath(shape());
    }
}

void GraphicStone::mousePressEvent(QGraphicsSceneMouseEvent*)
{
    GraphicBoard* scene_parent = dynamic_cast<GraphicBoard*>(scene()); //dependence on GraphicBoard
    emit scene_parent->Stone_clicked(_i,_j);
   // QGraphicsItem::mousePressEvent(event);
}


