//
// Created by dym on 10.04.2021.
//

#include "FormDraw.hpp"
#include "FormMain.hpp"
#include "ui_FormDraw.h"
#include <FigureSelectorAdapter.hpp>
#include <QColorDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <Scene.hpp>
#include <UserInputCreator.hpp>

#ifdef TO_LITERAL_STRING
  #error "TO_LITERAL_STRING is defined"
#endif
#define TO_LITERAL_STRING(in) #in

FormDraw::~FormDraw()
{
  QSettings settings= FormMain::getGlobalQSetting();
  settings.setValue(TO_LITERAL_STRING(colorFigure), colorFigure);
  settings.setValue(TO_LITERAL_STRING(figureSelector), QVariant::fromValue(*figureSelector));
}
FormDraw::FormDraw(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , ui(new Ui::FormDraw)
    , figureSelector(new FigureSelectorAdapter)
{
  QSettings settings= FormMain::getGlobalQSetting();
  colorFigure       = settings.value(TO_LITERAL_STRING(colorFigure)).value<QColor>();
  *figureSelector   = settings.value(TO_LITERAL_STRING(figureSelector)).value<FigureSelectorAdapter>();

  ui->setupUi(this);

  ui->figuresPanel->setFigure(*figureSelector);
  ui->figuresPanel->setColorFigure(colorFigure);

  QObject::connect(ui->pushButton_sceneNew, &QPushButton::clicked, this, &FormDraw::sceneNew);
  QObject::connect(ui->pushButton_sceneLoad, &QPushButton::clicked, this, &FormDraw::sceneLoad);
  QObject::connect(ui->pushButton_sceneSave, &QPushButton::clicked, this, &FormDraw::sceneSafe);
  QObject::connect(ui->pushButton_setColor, &QPushButton::clicked, this, &FormDraw::setColor);

  QObject::connect(ui->figuresPanel, &FiguresPanel::figureChange, this, &FormDraw::setFigure);
}
void FormDraw::sceneNew()
{
  scene.reset(new Scene(QRectF{ -300, -250, 600, 500 }));
  scene->setObjectName(ui->lineEdit_title->text());
  {
    auto const handle     = new UserInputCreator(scene.get());
    handle->figureSelector= figureSelector.get();
    handle->colorPainting = &colorFigure;
    scene->inputHandler.reset(handle);
    ui->graphicsView->setScene(scene.get());
  }
}
void FormDraw::sceneLoad()
{
  QFile file(ui->lineEdit_title->text() + "_scene.json");
  if (!file.open(QIODevice::ReadOnly))
    assert(false);

  scene.reset(new Scene(QJsonDocument::fromJson(QByteArray(file.readAll())).object()));
  {
    auto const handle     = new UserInputCreator(scene.get());
    handle->figureSelector= figureSelector.get();
    handle->colorPainting = &colorFigure;
    scene->inputHandler.reset(handle);
    ui->graphicsView->setScene(scene.get());
  }

  ui->lineEdit_title->setText(scene->objectName());
}
void FormDraw::sceneSafe()
{
  QFile file(ui->lineEdit_title->text() + "_scene.json");
  if (!file.open(QIODevice::WriteOnly))
    assert(false);

  file.write(QJsonDocument(scene->serialize()).toJson());
}
void FormDraw::setColor()
{
  colorFigure= QColorDialog::getColor(colorFigure, this);
  ui->figuresPanel->setColorFigure(colorFigure);
}
void FormDraw::setFigure(int const i)
{
  figureSelector->type= FigureSelectorAdapter::shape;
  figureSelector->i= i;
}
