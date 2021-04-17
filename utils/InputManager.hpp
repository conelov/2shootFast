//
// Created by dym on 15.04.2021.
//

#ifndef INC_2SHOOT_INPUTMANAGER_HPP
#define INC_2SHOOT_INPUTMANAGER_HPP
#include <memory>
class Scene;
class QGraphicsSceneMouseEvent;
namespace draw::methods{
class Base;
}

struct InputManagerBase
{
  virtual ~InputManagerBase();
  virtual void mousePressEvent(Scene *, QGraphicsSceneMouseEvent *event)  = 0;
  virtual void mouseReleaseEvent(Scene *, QGraphicsSceneMouseEvent *event)= 0;
  virtual void mouseMoveEvent(Scene *, QGraphicsSceneMouseEvent *event)   = 0;
};

class PainterManager: public InputManagerBase {
  struct PainterPath;
  std::unique_ptr<PainterPath> path;

public:
  draw::methods::Base const * method{};

  ~PainterManager() override;
  PainterManager();
  void mousePressEvent(Scene *, QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(Scene *, QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(Scene *, QGraphicsSceneMouseEvent *event) override;
};

#endif // INC_2SHOOT_INPUTMANAGER_HPP
