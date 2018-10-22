#include "View.h"
#include <PentagoLib/Game.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication *application = new QApplication(argc, argv);
    Game::GetInstance().Show();

    return application->exec();
}