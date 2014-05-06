#include "View.h"
#include <QApplication>
#include <QDebug>
#include <QtCore/qobject.h>
#include"Tester_.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View w;
    Tester_ t(&w);
    t.showText("start");
    t.showNum(20);
    w.show();

    return a.exec();
}
