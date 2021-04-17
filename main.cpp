#include <QApplication>
#include <QStyleFactory>
#include "ui/FormMain.hpp"

int main(int argc, char *argv[]) {

  QApplication a(argc, argv);
  QApplication::setStyle(QStyleFactory::create("cleanlooks"));
  QApplication::setOrganizationName("dym_conelove");
  QApplication::setApplicationName("2shoot");
  FormMain formMain;
  formMain.show();

  return QApplication::exec();
}
