//
// Created by dym on 15.04.2021.
//

#ifndef INC_2SHOOT_FORMDRAW_HPP
#define INC_2SHOOT_FORMDRAW_HPP
#include <QWidget>
#include "utils/Define.hpp"

namespace Ui
{
class FormDraw;
}
class QPushButton;
class PainterManager;


class FormDraw final: public QWidget {
  Q_OBJECT

  constexpr static const QSize iconSize= { 32, 32 };
  QScopedPointer<Ui::FormDraw> ui;
  QSharedPointer<PainterManager> paintManager;
  QPushButton *buttonActive{};
  std::vector<QPushButton *> buttons[draw::paintersCount];

public:
  ~FormDraw() override;
  FormDraw(QWidget *parent= {}, Qt::WindowFlags f= {});

private:
  Q_SLOT void colorChange();
};

#endif // INC_2SHOOT_FORMDRAW_HPP
