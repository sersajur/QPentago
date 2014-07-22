#include "View.h"
#include "Presenter/Game.h"

#include <QApplication>

#include <memory> //std::unique_ptr

// The Game may construct some QWidget-s and not release them
// before a->exec() return. (Because Game::GetInstance() actually return a static Game object)
// "application" is static and will be initialised the very first one
// so it will be destructed the very last one (3.6.3) (Game object will be initialised in Game::GetInstance())
// and any QWidgets in Game will be destructed safely
static std::unique_ptr<QApplication> application;

int main(int argc, char *argv[])
{
    application.reset(new QApplication(argc, argv));
    Game::GetInstance().Show();

    return application->exec();
}
