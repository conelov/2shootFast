//
// Created by dym on 15.04.2021.
//

#ifndef INC_2SHOOT_SCENEITEM_HPP
#define INC_2SHOOT_SCENEITEM_HPP
#include <QGraphicsItem>
#include <memory>

namespace draw
{
class Painter;
}

class SceneItem: public QGraphicsItem {
public:
  const std::unique_ptr<draw::Painter> drawMethod;

  virtual ~SceneItem();
  SceneItem(QGraphicsItem *parent= {});
};

#endif // INC_2SHOOT_SCENEITEM_HPP
