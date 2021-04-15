#include <QApplication>
#include <QStyleFactory>
#include "ui/FormMain.hpp"
#include "utils/Define.hpp"
#include "utils/Paint.hpp"

int main(int argc, char *argv[]) {
  qRegisterMetaTypeStreamOperators<draw::Selector>(TO_LITERAL_STRING(Draw::Selector));

  QApplication a(argc, argv);
  QApplication::setStyle(QStyleFactory::create("cleanlooks"));
  QApplication::setOrganizationName("dym_conelove");
  QApplication::setApplicationName("2shoot");
  FormMain formMain;
  formMain.show();

  return QApplication::exec();
}
