#ifndef TESTER__H
#define TESTER__H
#include "view.h"
#include <QDebug>
class Tester_: public QObject
{
    Q_OBJECT
public:
    explicit Tester_(View* v = 0);

public slots:
    void showText(std::string s){ qDebug() << QString::fromStdString(s);}
    void showNum(int n){ qDebug() << n;}
private:
    View* view;
};
#endif // TESTER__H
