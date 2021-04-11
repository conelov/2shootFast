#include "ui/FormMain.hpp"
#include <QApplication>
#include <QDebug>
#include <QStyleFactory>
#include <FigureSelectorAdapter.hpp>

#ifdef TO_LITERAL_STRING
#error "TO_LITERAL_STRING is defined"
#endif
#define TO_LITERAL_STRING(in) #in

int main(int argc, char *argv[]) {
    qRegisterMetaTypeStreamOperators<FigureSelectorAdapter>(TO_LITERAL_STRING(FigureSelectorAdapter));
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("cleanlooks"));
    QApplication::setOrganizationName("dym_conelove");
    QApplication::setApplicationName("2shoot");

    FormMain formMain;
    formMain.show();

    return QApplication::exec();
}
