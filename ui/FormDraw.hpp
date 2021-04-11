//
// Created by dym on 10.04.2021.
//

#ifndef INC_2SHOOT_FORMDRAW_HPP
#define INC_2SHOOT_FORMDRAW_HPP
#include <QWidget>

namespace Ui
{
class FormDraw;
}
class Scene;

class FormDraw final: public QWidget {
  Q_OBJECT

  QScopedPointer<Ui::FormDraw> ui;
  QScopedPointer<Scene> scene;
  int figureSelector = -1;
  QColor colorFigure;

public:
  ~FormDraw() override;
  FormDraw(QWidget *parent= {}, Qt::WindowFlags f= {});

private:
  Q_SLOT void sceneNew();
  Q_SLOT void sceneLoad();
  Q_SLOT void sceneSafe();
  Q_SLOT void setColor();
};

#endif // INC_2SHOOT_FORMDRAW_HPP
