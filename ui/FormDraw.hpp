//
// Created by dym on 15.04.2021.
//

#ifndef INC_2SHOOT_FORMDRAW_HPP
#define INC_2SHOOT_FORMDRAW_HPP
#include <QWidget>
#include <memory>

namespace Ui
{
class FormDraw;
}
class PainterManager;
class Scene;

class FormDraw final: public QWidget {
  Q_OBJECT

  constexpr static const QSize iconSize= { 32, 32 };
  QScopedPointer<Ui::FormDraw> ui;

  Scene *sceneActive{};
  std::shared_ptr<PainterManager> paintManager{};
  QColor color;

  class PushButtonIcon;
  PushButtonIcon *buttonActive{};

public:
  ~FormDraw() override;
  FormDraw(QWidget *parent= {}, Qt::WindowFlags f= {});

private:
  Q_SLOT void colorChange();
  Q_SLOT void sceneNew();

  QList<PushButtonIcon *> allToolButton();
};

#endif // INC_2SHOOT_FORMDRAW_HPP
