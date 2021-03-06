//
// Created by dym on 15.04.2021.
//
#include "FormDraw.hpp"
#include "ui_FormDraw.h"

#include "FormMain.hpp"
#include "moc/Scene.hpp"
#include "utils/Define.hpp"
#include "utils/InputHandler.hpp"
#include "utils/PrintMethods.hpp"

#include <QColorDialog>
#include <QDataStream>
#include <QFile>
#include <QVBoxLayout>

#include <QDebug>

class FormDraw::PushButtonIcon: public QPushButton {
  Q_OBJECT

public:
  std::unique_ptr<draw::Base> method{};
};

FormDraw::~FormDraw()
{
  if (sceneActive) {
    ui->graphicsView->setScene(nullptr);
    sceneActive->setParent(nullptr);
    delete sceneActive;
  }

  QSettings settings= FormMain::getGlobalQSetting();
  settings.setValue(TO_LITERAL_STRING(color), color);

  if (buttonActive) {
    settings.setValue("buttonActiveId", buttonActive->method->id());
  }
}
FormDraw::FormDraw(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , ui(new Ui::FormDraw)
    , handler(new DrawHandler)
{
  QSettings settings= FormMain::getGlobalQSetting();
  color             = settings.value(TO_LITERAL_STRING(color), QColor(Qt::yellow)).value<QColor>();

  ui->setupUi(this);
  assert(ui->toolBox->count() == draw::Base::Type::max_value + 1);

  QVBoxLayout *layouts[draw::Base::Type::max_value + 1];
  for (auto &la : layouts)
    la= new QVBoxLayout;
  for (auto const id : draw::allId) {
    auto const but= new PushButtonIcon;
    but->setCheckable(true);
    but->method.reset(static_cast<draw::Base *>(QMetaType::create(id)));
    QObject::connect(
        but,
        &QPushButton::pressed,
        this,
        [this]
        {
          auto const sendB= qobject_cast<PushButtonIcon *>(sender());
          if (buttonActive == sendB) {
            buttonActive   = nullptr;
            handler->method= nullptr;
          } else {
            if (buttonActive) {
              buttonActive->setChecked(false);
              buttonActive= sendB;
            } else {
              buttonActive= sendB;
            }
            handler->method= buttonActive->method.get();
          }
        });
    layouts[but->method->type()]->addWidget(but);
  }
  for (size_t i{}; i < draw::Base::Type::max_value + 1; ++i) {
    layouts[i]->addStretch();
    layouts[i]->setMargin(this->layout()->margin());
    layouts[i]->setSpacing(this->layout()->spacing());
    ui->toolBox->widget(i)->setLayout(layouts[i]);
  }

  QObject::connect(
      ui->pushButton_setColor,
      &QPushButton::pressed,
      this,
      [this]
      {
        color= QColorDialog::getColor(color, this);
        colorChange();
      });

  QObject::connect(ui->pushButton_sceneNew, &QPushButton::pressed, this, &FormDraw::sceneNew);
  QObject::connect(ui->pushButton_sceneLoad, &QPushButton::pressed, this, &FormDraw::sceneLoad);
  QObject::connect(ui->pushButton_sceneSave, &QPushButton::pressed, this, &FormDraw::sceneSafe);

  colorChange();

  if (auto const setId= settings.value("buttonActiveId", -1).value<int>(); setId != -1) {
    auto const list     = allToolButton();
    auto const activeBIt= std::ranges::find_if(list, [setId](PushButtonIcon *i) { return i->method->id() == setId; });
    assert(activeBIt != list.end());
    (*activeBIt)->click();
  }
}

void FormDraw::colorChange()
{
  QPen pen= color;
  pen.setWidth(4);
  QBrush brush= color.darker(125);
  QPixmap pixmap(iconSize);

  for (auto const bu : allToolButton()) {
    bu->method->pen  = pen;
    bu->method->brush= brush;

    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    bu->method->paint(&painter, { { 0, 0 }, iconSize });
    bu->setIcon(pixmap);
  }
}
void FormDraw::sceneNew()
{
  assert(!sceneActive);
  sceneActive             = new Scene(Scene::defaultBorders());
  sceneActive->handlerWeak= handler;

  ui->graphicsView->setScene(sceneActive);
}
void FormDraw::sceneLoad()
{
  assert(!sceneActive);
  {
    QFile file(ui->lineEdit_title->text() + "_scene.txt");
    if (!file.open(QIODevice::ReadOnly))
      assert(false);
    QByteArray buffer= file.readAll();
    QDataStream stream(&buffer, QIODevice::ReadOnly);
    sceneActive             = new Scene;
    sceneActive->handlerWeak= handler;
    stream >> *sceneActive;
  }
  ui->graphicsView->setScene(sceneActive);
}
void FormDraw::sceneSafe()
{
  assert(sceneActive);

  QByteArray buffer;
  {
    QDataStream stream(&buffer, QIODevice::WriteOnly);
    stream << *sceneActive;
  }
  QFile file(ui->lineEdit_title->text() + "_scene.txt");
  if (!file.open(QIODevice::WriteOnly))
    assert(false);
  file.write(buffer);
}

QList<FormDraw::PushButtonIcon *> FormDraw::allToolButton()
{
  QList<FormDraw::PushButtonIcon *> ret;
  for (size_t i{}; i < ui->toolBox->count(); ++i) {
    ret.append(ui->toolBox->findChildren<PushButtonIcon *>());
  }
  return std::move(ret);
}

#include "FormDraw.moc"