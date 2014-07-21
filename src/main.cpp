#include "View.h"
#include "Presenter/Game.h"
#include <QApplication>
#include <QDebug>
#include <QtCore/qobject.h>

int main(int argc, char *argv[])
{
    QApplication *a = new QApplication(argc, argv);
    Game::GetInstance().Show();

    return a->exec();
}
