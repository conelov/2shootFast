//
// Created by dym on 08.04.2021.
//

#include "FormMain.hpp"
#include "ui_FormMain.h"

FormMain::~FormMain()
{
}

FormMain::FormMain(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
    , ui(new Ui::FormMain)
{
  ui->setupUi(this);


  resize(800, 600);
}
