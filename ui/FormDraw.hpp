//
// Created by dym on 10.04.2021.
//

#ifndef INC_2SHOOT_FORMDRAW_HPP
#define INC_2SHOOT_FORMDRAW_HPP
#include <QWidget>

namespace Ui{
class FormDraw;
}

class FormDraw final : public QWidget{
  Q_OBJECT
  class SceneManager;

  QScopedPointer<Ui::FormDraw> ui;
  QScopedPointer<SceneManager> scene;
  QColor colorFigure;

public:
  ~FormDraw() override;
  FormDraw(QWidget *parent = {}, Qt::WindowFlags f = {});

private:
};

#endif // INC_2SHOOT_FORMDRAW_HPP
