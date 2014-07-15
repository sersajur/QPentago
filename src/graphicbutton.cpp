#include "graphicbutton.h"


GraphicButton::GraphicButton()
{

}

GraphicButton::GraphicButton(const QString& caption, QGraphicsItem *parent):
    QGraphicsWidget(parent),
    _caption(caption)
{
     setAcceptHoverEvents(true);
     setCacheMode(DeviceCoordinateCache);
}

QRectF GraphicButton::boundingRect() const
{
    return QRectF(-GB_WIDTH/2,-GB_HEIGHT/2,GB_WIDTH,GB_HEIGHT);
}

QPainterPath GraphicButton::shape() const
{
    QPainterPath path;
    QRectF r = boundingRect();
    path.addRoundedRect(r,r.height()/4,r.height()/4);
    return path;

}

void GraphicButton::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
    bool down = option->state & QStyle::State_Sunken;
    QRectF r = boundingRect();
    QLinearGradient grad(r.topLeft(), r.bottomRight());

    if(down)
        painter->translate(2,2);

    grad.setColorAt(down ? 1 : 0, option->state & QStyle::State_MouseOver ? Qt::lightGray : Qt::darkGray);
    grad.setColorAt(down ? 0 : 1, Qt::black);
    painter->setPen(Qt::NoPen);
    painter->setBrush(grad);
    painter->drawPath(shape());

    painter->setPen(option->state & QStyle::State_MouseOver ? Qt::green : Qt::white);
    QFont f("Cursive",r.height()/3,QFont::Normal);
    painter->setFont(f);
    painter->drawText(boundingRect(),_caption,Qt::AlignHCenter|Qt::AlignVCenter);
}

void GraphicButton::mousePressEvent(QGraphicsSceneMouseEvent*)
{
    emit pressed();
    update();
}

void GraphicButton::mouseReleaseEvent(QGraphicsSceneMouseEvent*)
{
    update();
}
