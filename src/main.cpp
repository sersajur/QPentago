#include "main.h"

#include <QApplication>
#include <QDir>

#include "glview.h"

int main(int argc, char *argv[])
{
  setlocale(LC_ALL,"");
  QDir appDir = QFileInfo(QString::fromLocal8Bit(argv[0])).absolutePath();
  QString tmpPath = appDir.absolutePath();
#ifdef QT_DEBUG
  QApplication::addLibraryPath(tmpPath+"/QTplaggins");
#else
  QApplication::setLibraryPaths({tmpPath+"/QTplaggins"});
#endif

  QApplication a(argc, argv);
  GLview w;
  PresenterEmulator presenter(w);
  w.dumpObjectInfo();
  (void)presenter;
  return a.exec();
}
