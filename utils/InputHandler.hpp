//
// Created by dym on 15.04.2021.
//

#ifndef INC_2SHOOT_INPUTHANDLER_HPP
#define INC_2SHOOT_INPUTHANDLER_HPP
#include <memory>
class Scene;
class QGraphicsSceneMouseEvent;
namespace draw::method
{
class Base;
}

struct InputHandlerBase
{
  virtual ~InputHandlerBase();
  virtual void mousePressEvent(Scene *, QGraphicsSceneMouseEvent *event)  = 0;
  virtual void mouseReleaseEvent(Scene *, QGraphicsSceneMouseEvent *event)= 0;
  virtual void mouseMoveEvent(Scene *, QGraphicsSceneMouseEvent *event)   = 0;
};

class DrawHandler: public InputHandlerBase {
  struct PainterPath;
  std::unique_ptr<PainterPath> path;

public:
  draw::method::Base const * method{};

  ~DrawHandler() override;
  DrawHandler();
  void mousePressEvent(Scene *, QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(Scene *, QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(Scene *, QGraphicsSceneMouseEvent *event) override;
};

#endif // INC_2SHOOT_INPUTHANDLER_HPP
