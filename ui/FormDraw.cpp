//
// Created by dym on 15.04.2021.
//
#include "FormDraw.hpp"
#include "FormMain.hpp"
#include "ui_FormDraw.h"
#include "utils/InputManager.hpp"
#include "utils/Paint.hpp"
#include <QColorDialog>
#include <QVBoxLayout>
#include <utils/Define.hpp>

#include <QDebug>

FormDraw::~FormDraw()
{
  QSettings settings= FormMain::getGlobalQSetting();
  settings.setValue(TO_LITERAL_STRING(color), *paintManager->color);
  settings.setValue(TO_LITERAL_STRING(selector), QVariant::fromValue(*paintManager->selector));
}
FormDraw::FormDraw(QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , ui(new Ui::FormDraw)
    , paintManager(new PainterManager)
{
  auto &color   = *paintManager->color;
  auto &selector= *paintManager->selector;
  {
    QSettings settings= FormMain::getGlobalQSetting();
    color             = settings.value(TO_LITERAL_STRING(color), QColor(Qt::yellow)).value<QColor>();
    selector          = settings.value(TO_LITERAL_STRING(selector)).value<draw::Selector>();
  }
  ui->setupUi(this);
  auto const & methods= *qobject_cast<FormMain *>(parent)->methods;
  assert(draw::paintersCount == ui->toolBox->count());

  for (size_t type{}; type < draw::paintersCount; ++type) {
    auto const layout= new QVBoxLayout;
    for (size_t i{}; i < methods.methods[type].size(); ++i) {
      auto const button= new QPushButton;
      buttons[type].push_back(button);
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
              buttonActive               = nullptr;
              paintManager->selector->type= draw::Selector::none;
            } else {
              if (buttonActive) {
                buttonActive->setChecked(false);
                buttonActive= sendB;
              } else {
                buttonActive= sendB;
              }
              paintManager->selector->type= static_cast<draw::Selector::Type>(type + 1);
              paintManager->selector->pos=
                  std::distance(buttons[type].cbegin(), std::find(buttons[type].cbegin(), buttons[type].cend(), sendB));
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
        *paintManager->color= QColorDialog::getColor(*paintManager->color, this);
        colorChange();
      });

  if (selector) {
    buttons[selector.type - 1].at(selector.pos)->click();
    ui->toolBox->setCurrentIndex(selector.type - 1);
  }
  colorChange();
}

void FormDraw::colorChange()
{
  auto const & methods= *qobject_cast<FormMain *>(parent())->methods;
  for (size_t type{}; type < draw::paintersCount; ++type) {
    for (size_t i{}; i < buttons[type].size(); ++i) {
      QPixmap pixmap(iconSize);
      pixmap.fill(Qt::transparent);
      QPainter painter(&pixmap);
      methods.methods[type].at(i).paint(&painter, { 0, 0, iconSize.width(), iconSize.height() }, *paintManager->color);
      buttons[type].at(i)->setIcon(QIcon(std::move(pixmap)));
    }
  }
}
