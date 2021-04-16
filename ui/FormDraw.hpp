//
// Created by dym on 15.04.2021.
//

#ifndef INC_2SHOOT_FORMDRAW_HPP
#define INC_2SHOOT_FORMDRAW_HPP
#include "utils/Define.hpp"
#include <QWidget>
#include <memory>

namespace Ui
{
class FormDraw;
}
class QPushButton;
class PainterManager;
class Scene;
namespace draw
{
class Painter;
} // namespace draw

class FormDraw final: public QWidget {
  Q_OBJECT

  constexpr static const QSize iconSize= { 32, 32 };
  QScopedPointer<Ui::FormDraw> ui;

  Scene *sceneActive{};
  std::shared_ptr<PainterManager> paintManager;
  QColor color;

  QPushButton *buttonActive{};
  using ButtonPainterPair= std::pair<QPushButton *, std::unique_ptr<draw::Painter>>;
  std::vector<ButtonPainterPair> buttons[draw::paintersCount];

public:
  ~FormDraw() override;
  FormDraw(QWidget *parent= {}, Qt::WindowFlags f= {});

private:
  Q_SLOT void colorChange();
  Q_SLOT void sceneNew();
};

#endif // INC_2SHOOT_FORMDRAW_HPP
