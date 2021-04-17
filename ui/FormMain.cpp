//
// Created by dym on 15.04.2021.
//
#include "FormMain.hpp"
#include "ui_FormMain.h"
#include <QGraphicsScene>

QSettings FormMain::getGlobalQSetting()
{
#ifdef linux
  return QSettings(QCoreApplication::applicationDirPath() + "/settings.cfg", QSettings::NativeFormat);
#else
  return QSettings{};
#endif
}

FormMain::~FormMain()= default;
FormMain::FormMain(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
    , ui(new Ui::FormMain)
{
  ui->setupUi(this);

  resize(800, 600);
}
