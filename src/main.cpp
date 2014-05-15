#include "View.h"
#include "Presenter/Game.h"
#include <QApplication>
#include <QDebug>
#include <QtCore/qobject.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game& game = Game::GetInstance();
    game.Show();

    return a.exec();
}
