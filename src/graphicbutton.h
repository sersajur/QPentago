#ifndef GRAPHICBUTTON_H
#define GRAPHICBUTTON_H

#include <QGraphicsWidget>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionGraphicsItem>

#define GB_WIDTH 100
#define GB_HEIGHT 40
class GraphicButton: public QGraphicsWidget
{
    Q_OBJECT
public:
    GraphicButton();
    GraphicButton(const QString& caption, QGraphicsItem *parent = 0);
    ~GraphicButton() { }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:
    void pressed();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    QString _caption;
};

#endif // GRAPHICBUTTON_H
