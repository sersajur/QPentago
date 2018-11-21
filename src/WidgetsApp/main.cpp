#include "View.h"
#include <PentagoLib/Game.h>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    auto view { std::make_shared<View>() };
    Game game(view);
    view->ShowWindow();

    return app.exec();
}
