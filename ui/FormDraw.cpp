//
// Created by dym on 15.04.2021.
//
#include "FormDraw.hpp"
#include "ui_FormDraw.h"

#include "FormMain.hpp"
#include "moc/Scene.hpp"
#include "utils/Define.hpp"
#include "utils/InputManager.hpp"
#include "utils/Paint.hpp"

#include <QColorDialog>
#include <QVBoxLayout>

#include <QDebug>

FormDraw::~FormDraw()
{
  if (sceneActive) {
    ui->graphicsView->setScene(nullptr);
    sceneActive->setParent(nullptr);
    delete sceneActive;
  }

  QSettings settings= FormMain::getGlobalQSetting();
  settings.setValue(TO_LITERAL_STRING(color), color);

  for (int type{}; type < draw::paintersCount; ++type) {
    for (int i{}; i < buttons[type].size(); ++i) {
      if (buttons[type][i].first != buttonActive)
        continue;
      settings.setValue("buttonActiveType", type);
      settings.setValue("buttonActiveIndex", i);
    }
  }
}
FormDraw::FormDraw(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , ui(new Ui::FormDraw)
    , paintManager(new PainterManager)
{
  QSettings settings= FormMain::getGlobalQSetting();
  color             = settings.value(TO_LITERAL_STRING(color), QColor(Qt::yellow)).value<QColor>();

  ui->setupUi(this);
  auto const &methods= *qobject_cast<FormMain *>(parent)->methods;
  assert(draw::paintersCount == ui->toolBox->count());

  for (size_t type{}; type < draw::paintersCount; ++type) {
    auto const layout= new QVBoxLayout;
    for (size_t i{}; i < methods.methods[type].size(); ++i) {
      auto const button= new QPushButton;
      buttons[type].push_back(ButtonPainterPair{ button, new draw::Painter(methods.methods[type][i]) });
      button->setCheckable(true);
      button->setIconSize(iconSize);

      QObject::connect(
          button,
          &QPushButton::pressed,
          this,
          [this, type]
          {
            auto const sendB= qobject_cast<QPushButton *>(sender());
            if (buttonActive == sendB) {
              buttonActive         = nullptr;
              paintManager->painter= nullptr;
            } else {
              if (buttonActive) {
                buttonActive->setChecked(false);
                buttonActive= sendB;
              } else {
                buttonActive= sendB;
              }
              paintManager->painter= std::find_if(
                                         buttons[type].cbegin(),
                                         buttons[type].cend(),
                                         [sendB](ButtonPainterPair const &i) { return i.first == sendB; })
                                         ->second.get();
            }
          });
      layout->addWidget(button);
    }
    layout->addStretch();
    layout->setMargin(this->layout()->margin());
    layout->setSpacing(this->layout()->spacing());
    ui->toolBox->widget(type)->setLayout(layout);
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

  {
    int buttonActiveType = settings.value(TO_LITERAL_STRING(buttonActiveType), -1).value<decltype(buttonActiveType)>(),
        buttonActiveIndex= settings.value(TO_LITERAL_STRING(buttonActiveIndex), -1).value<decltype(buttonActiveIndex)>();
    assert(buttonActiveType < draw::paintersCount);

    if (buttonActiveType != -1) {
      assert(buttonActiveIndex != -1);
      buttons[buttonActiveType][buttonActiveIndex].first->click();
    }
  }
  colorChange();
}

void FormDraw::colorChange()
{
  for (size_t type{}; type < draw::paintersCount; ++type) {
    for (auto const &button : buttons[type]) {
      QPixmap pixmap(iconSize);
      pixmap.fill(Qt::transparent);
      QPainter painter(&pixmap);
      QPen pen= color;
      pen.setWidth(4);
      QBrush brush= color;
      brush.setColor(color.darker(125));
      button.second->paint(&painter, { 0, 0, iconSize.width(), iconSize.height() }, pen, &brush);
      button.first->setIcon(QIcon(std::move(pixmap)));
    }
  }
}
void FormDraw::sceneNew()
{
  assert(!sceneActive);
  sceneActive             = new Scene;
  sceneActive->managerWeak= std::weak_ptr(this->paintManager);

  ui->graphicsView->setScene(sceneActive);
}