#include "View.h"
#include <QApplication>
#include <QDebug>
#include <QtCore/qobject.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    View w;
    w.show();

    return a.exec();
}
