//
// Created by dym on 10.04.2021.
//

#include "FormDraw.hpp"
#include "SceneCreator.hpp"
#include "ui_FormDraw.h"
#include <QColorDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

#ifdef TO_LITERAL_STRING
  #error "TO_LITERAL_STRING is defined"
#endif
#define TO_LITERAL_STRING(in) #in

namespace
{
QSettings getGlobalQSetting()
{
#ifdef linux
  return QSettings(QCoreApplication::applicationDirPath() + "/settings.cfg", QSettings::NativeFormat);
#else
  return QSettings{};
#endif
}
} // namespace

class FormDraw::SceneManager {
  FormDraw *form;
  QScopedPointer<SceneCreator> scene;

public:
  SceneManager()= delete;
  SceneManager(FormDraw *formIn)
      : form(formIn)
  {}
  SceneCreator *reset(SceneCreator *sceneIn)
  {
    scene.reset(sceneIn);

    form->ui->graphicsView->setScene(scene.get());
    scene->setObjectName(form->ui->lineEdit_title->text());
    scene->colorFigure= form->colorFigure;
    QObject::connect(form->ui->figuresPanel, &FiguresPanel::figureChange, scene.get(), &SceneCreator::setDrawingFigureId);

    return operator SceneCreator *();
  }
  [[nodiscard]] SceneCreator *operator->() const
  {
    return operator SceneCreator *();
  }
  [[nodiscard]] SceneCreator *get() const
  {
    return operator SceneCreator *();
  }
  [[nodiscard]] operator SceneCreator *() const
  {
    return scene.get();
  }
};

FormDraw::~FormDraw()
{
  QSettings settings= getGlobalQSetting();
  settings.setValue(TO_LITERAL_STRING(colorFigure), colorFigure);

  QJsonObject json;
  json["title"]= objectName();
}

FormDraw::FormDraw(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , ui(new Ui::FormDraw)
    , scene(new SceneManager(this))

{
  QSettings settings= getGlobalQSetting();
  colorFigure       = settings.value(TO_LITERAL_STRING(colorFigure)).value<QColor>();

  ui->setupUi(this);

  scene->reset(new SceneCreator(QRectF{ -300, -250, 600, 500 }));
  ui->figuresPanel->setColorFigure(colorFigure);

  QObject::connect(
      ui->pushButton_setColor,
      &QPushButton::clicked,
      this,
      [this]
      {
        colorFigure= QColorDialog::getColor(colorFigure, this);
        scene->get()->setColorFigure(colorFigure);
        ui->figuresPanel->setColorFigure(colorFigure);
      });

  QObject::connect(
      ui->pushButton_sceneSave,
      &QPushButton::clicked,
      this,
      [this]
      {
        QFile file(scene->get()->objectName() + "_scene.json");
        if (!file.open(QIODevice::WriteOnly))
          assert(false);

        file.write(QJsonDocument(scene->get()->serialize()).toJson());
      });

  QObject::connect(
      ui->pushButton_sceneLoad,
      &QPushButton::clicked,
      this,
      [this]
      {
        QFile file(scene->get()->objectName() + "_scene.json");
        if (!file.open(QIODevice::ReadOnly))
          assert(false);

        scene->reset(new SceneCreator(QJsonDocument::fromJson(QByteArray(file.readAll())).object()));
      });
}