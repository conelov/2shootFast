//
// Created by dym on 09.04.2021.
//

#include "FiguresPanel.hpp"
#include "ui_FiguresPanel.h"
#include <QDebug>
#include <QPainter>

namespace
{
QPixmap pixmapFromAlphaMap(QColor const &color, QString const &pathToAMap)
{
  QPixmap aMap(pathToAMap);
  QPainter painter(&aMap);
  painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
  painter.setPen(color);
  painter.setBrush(color);
  painter.drawRect(aMap.rect());
  painter.end();
  return std::move(aMap);
}
} // namespace

FiguresPanel::~FiguresPanel()= default;

FiguresPanel::FiguresPanel(QWidget *parent, const Qt::WindowFlags f)
    : FiguresPanel({}, parent, f)
{}

FiguresPanel::FiguresPanel(const QColor &colorIcon, QWidget *parent, Qt::WindowFlags f)
    : QWidget(parent, f)
    , ui(new Ui::FiguresPanel)
{
  ui->setupUi(this);
  setColorFigure(colorIcon);

  for (auto const button : findChildren<QPushButton *>()) {
    if (!button)
      continue;
    QObject::connect(button, &QPushButton::clicked, this, &FiguresPanel::guardMultiCheckButton);
    QObject::connect(button, &QPushButton::clicked, this, [this] { emit figureChange(buttonFigurePressedId()); });
  }
}

int FiguresPanel::buttonFigurePressedId() const
{
  if (!buttonFigureCurrentPressed)
    return -1;
  auto const listButton= findChildren<QPushButton *>();
  auto const currBIt   = std::find(listButton.cbegin(), listButton.cend(), buttonFigureCurrentPressed);
  assert(currBIt != listButton.cend());
  return std::distance(listButton.cbegin(), currBIt);
}

void FiguresPanel::guardMultiCheckButton()
{
  auto const sendB= qobject_cast<QPushButton *>(sender());
  if (buttonFigureCurrentPressed == sendB) {
    buttonFigureCurrentPressed= nullptr;
  } else if (buttonFigureCurrentPressed) {
    buttonFigureCurrentPressed->setChecked(false);
    buttonFigureCurrentPressed= sendB;
  } else {
    buttonFigureCurrentPressed= sendB;
  }
}

void FiguresPanel::setColorFigure(const QColor &color)
{
  ui->pushButton_circle->setIcon(pixmapFromAlphaMap(color, ":/res/img/circle.png"));
  ui->pushButton_circle->setIconSize({ 32, 32 });

  ui->pushButton_square->setIcon(pixmapFromAlphaMap(color, ":/res/img/square.png"));
  ui->pushButton_square->setIconSize({ 32, 32 });

  ui->pushButton_line->setIcon(pixmapFromAlphaMap(color, ":/res/img/line.png"));
  ui->pushButton_line->setIconSize({ 32, 32 });
}