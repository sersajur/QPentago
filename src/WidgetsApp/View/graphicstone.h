#ifndef GRAPHICSTONE_H
#define GRAPHICSTONE_H
#include <QGraphicsWidget>

#define STONE_RADIUS 30
class GraphicStone: public QGraphicsWidget
{
    Q_OBJECT
public:
    GraphicStone(QGraphicsItem* parent = 0);
    GraphicStone(const QColor& default_col, int i, int j, qreal radius, QGraphicsItem* parent = 0);
    GraphicStone(const QColor& default_col, const QColor& fill_col, QGraphicsItem* parent = 0);//

    void Fill(const QColor& fill_col);
    void Empty();
    bool isFilled(){return _fill_flag;}
    void setId(int i, int j){_i=i;_j=j;}
    qreal getR(){return _radius;}

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
signals:
    void clicked();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
private slots:
    void on_click(){ isFilled()?Empty():Fill(Qt::black);}//
private:
    QColor _default_color;
    QColor _fill_color;
    bool _fill_flag;
    int _i, _j;
    qreal _radius;

};

#endif // GRAPHICSTONE_H
