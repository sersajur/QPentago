#include "tester_.h"
#include <string>
Tester_::Tester_(View* v)

        {
            view = v;
            QObject::connect(view,SIGNAL(Load_game(std::string)),this,SLOT(showText(std::string)));
            QObject::connect(view,SIGNAL(Host_game(std::string)),this,SLOT(showText(std::string)));
            QObject::connect(view,SIGNAL(Join_game(std::string)),this,SLOT(showText(std::string)));
            QObject::connect(view,SIGNAL(New_game(int)),this,SLOT(showNum(int)));
        }

