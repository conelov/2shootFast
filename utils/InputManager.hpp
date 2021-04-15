//
// Created by dym on 15.04.2021.
//

#ifndef INC_2SHOOT_INPUTMANAGER_HPP
#define INC_2SHOOT_INPUTMANAGER_HPP
#include <memory>

class QGraphicsSceneMouseEvent;
class QColor;
namespace draw
{
class Selector;
}
struct InputManagerBase
{
  virtual ~InputManagerBase();
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)  = 0;
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)= 0;
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event)   = 0;
};

struct PainterManager: public InputManagerBase
{
  std::unique_ptr< QColor> color;
  std::unique_ptr<draw::Selector> selector;

  ~PainterManager() override;
  PainterManager() ;
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // INC_2SHOOT_INPUTMANAGER_HPP
