////
//// Created by dym on 10.04.2021.
////
//
//#include "FormDraw.hpp"
//#include "SceneCreator.hpp"
//#include "ui_FormDraw.h"
//#include <QColorDialog>
//#include <QFile>
//#include <QJsonDocument>
//#include <QJsonObject>
//#include <QSettings>
//
//#ifdef TO_LITERAL_STRING
//  #error "TO_LITERAL_STRING is defined"
//#endif
//#define TO_LITERAL_STRING(in) #in
//
// namespace
//{
// QSettings getGlobalQSetting()
//{
//#ifdef linux
//  return QSettings(QCoreApplication::applicationDirPath() + "/settings.cfg", QSettings::NativeFormat);
//#else
//  return QSettings{};
//#endif
//}
//} // namespace
//
// class FormDraw::SceneManager {
//  FormDraw *form;
//  QScopedPointer<SceneCreator> scene;
//
// public:
//  SceneManager()= delete;
//  SceneManager(FormDraw *formIn)
//      : form(formIn)
//  {}
//  SceneCreator *reset(SceneCreator *sceneIn)
//  {
//    scene.reset(sceneIn);
//
//    form->ui->graphicsView->setScene(scene.get());
//    scene->setObjectName(form->ui->lineEdit_title->text());
//    scene->colorFigure= form->colorFigure;
//    QObject::connect(form->ui->figuresPanel, &FiguresPanel::figureChange, scene.get(), &SceneCreator::setDrawingFigureId);
//
//    return operator SceneCreator *();
//  }
//  [[nodiscard]] SceneCreator *operator->() const
//  {
//    return operator SceneCreator *();
//  }
//  [[nodiscard]] SceneCreator *get() const
//  {
//    return operator SceneCreator *();
//  }
//  [[nodiscard]] operator SceneCreator *() const
//  {
//    return scene.get();
//  }
//};
//
// FormDraw::~FormDraw()
//{
//  QSettings settings= getGlobalQSetting();
//  settings.setValue(TO_LITERAL_STRING(colorFigure), colorFigure);
//
//  QJsonObject json;
//  json["title"]= objectName();
//}
//
// FormDraw::FormDraw(QWidget *parent, Qt::WindowFlags f)
//    : QWidget(parent, f)
//    , ui(new Ui::FormDraw)
//    , scene(new SceneManager(this))
//
//{
//  QSettings settings= getGlobalQSetting();
//  colorFigure       = settings.value(TO_LITERAL_STRING(colorFigure)).value<QColor>();
//
//  ui->setupUi(this);
//
//  scene->reset(new SceneCreator(QRectF{ -300, -250, 600, 500 }));
//  ui->figuresPanel->setColorFigure(colorFigure);
//
//  QObject::connect(
//      ui->pushButton_setColor,
//      &QPushButton::clicked,
//      this,
//      [this]
//      {
//        colorFigure= QColorDialog::getColor(colorFigure, this);
//        scene->get()->setColorFigure(colorFigure);
//        ui->figuresPanel->setColorFigure(colorFigure);
//      });
//
//  QObject::connect(
//      ui->pushButton_sceneSave,
//      &QPushButton::clicked,
//      this,
//      [this]
//      {
//        QFile file(scene->get()->objectName() + "_scene.json");
//        if (!file.open(QIODevice::WriteOnly))
//          assert(false);
//
//        file.write(QJsonDocument(scene->get()->serialize()).toJson());
//      });
//
//  QObject::connect(
//      ui->pushButton_sceneLoad,
//      &QPushButton::clicked,
//      this,
//      [this]
//      {
//        QFile file(scene->get()->objectName() + "_scene.json");
//        if (!file.open(QIODevice::ReadOnly))
//          assert(false);
//
//        scene->reset(new SceneCreator(QJsonDocument::fromJson(QByteArray(file.readAll())).object()));
//      });
//}

//
// Created by dym on 10.04.2021.
//

#include "FormDraw.hpp"
#include "FormMain.hpp"
#include "ui_FormDraw.h"
#include <QColorDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>
#include <SceneBase.hpp>
#include <UserInputCreator.hpp>

#ifdef TO_LITERAL_STRING
  #error "TO_LITERAL_STRING is defined"
#endif
#define TO_LITERAL_STRING(in) #in

FormDraw::~FormDraw()
{
  QSettings settings= FormMain::getGlobalQSetting();
  settings.setValue(TO_LITERAL_STRING(colorFigure), colorFigure);
  settings.setValue(TO_LITERAL_STRING(figureSelector), figureSelector);
}
FormDraw::FormDraw(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , ui(new Ui::FormDraw)
{
  QSettings settings= FormMain::getGlobalQSetting();
  colorFigure       = settings.value(TO_LITERAL_STRING(colorFigure)).value<QColor>();
  figureSelector    = settings.value(TO_LITERAL_STRING(figureSelector)).value<int>();

  ui->setupUi(this);

  ui->figuresPanel->setColorFigure(colorFigure);

  QObject::connect(ui->pushButton_sceneNew, &QPushButton::clicked, this, &FormDraw::sceneNew);
  QObject::connect(ui->pushButton_sceneLoad, &QPushButton::clicked, this, &FormDraw::sceneLoad);
  QObject::connect(ui->pushButton_sceneSave, &QPushButton::clicked, this, &FormDraw::sceneSafe);

  QObject::connect(ui->figuresPanel, &FiguresPanel::figureChange, this, [this](int i) { figureSelector= i; });
}
void FormDraw::sceneNew()
{
  scene.reset(new SceneBase(QRectF{ -300, -250, 600, 500 }));
  {
    auto const handle     = new UserInputCreator(scene.get());
    handle->figureSelector= &figureSelector;
    handle->colorPainting = &colorFigure;
    scene->_inputHandler.reset(handle);
    ui->graphicsView->setScene(scene.get());
  }
}
void FormDraw::sceneLoad()
{
  QFile file(ui->lineEdit_title->text() + "_scene.json");
  if (!file.open(QIODevice::ReadOnly))
    assert(false);

  scene.reset(new SceneBase(QJsonDocument::fromJson(QByteArray(file.readAll())).object()));
  {
    auto const handle     = new UserInputCreator(scene.get());
    handle->figureSelector= &figureSelector;
    handle->colorPainting = &colorFigure;
    scene->_inputHandler.reset(handle);
    ui->graphicsView->setScene(scene.get());
  }
}
void FormDraw::sceneSafe()
{}
