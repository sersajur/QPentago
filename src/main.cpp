#include "glview.h"
#include <QApplication>
#include <QObject>
#include <QDir>

class PresenterEmulator {
  GLview & view;
public:
  PresenterEmulator(GLview& v): view(v) {
    view.Set_game_layout(GLview::GAME_LAYOUT::MAIN_MENU);
    view.connect(&view,SIGNAL(Request_enter_game_layout(GAME_LAYOUT)),SLOT(Set_game_layout(GAME_LAYOUT)));
  }

};

int main(int argc, char *argv[])
{
  setlocale(LC_ALL,"");
  QDir appDir = QFileInfo(QString::fromLocal8Bit(argv[0])).absolutePath();
  QString tmpPath = appDir.absolutePath();
  QApplication::addLibraryPath(tmpPath+"/QTplaggins");
  QApplication a(argc, argv);
  GLview w;
  PresenterEmulator presenter(w);
  w.dumpObjectInfo();
  (void)presenter;
  return a.exec();
}
