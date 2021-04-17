//
// Created by dym on 15.04.2021.
//

#ifndef INC_2SHOOT_SCENEITEM_HPP
#define INC_2SHOOT_SCENEITEM_HPP
#include <QGraphicsItem>
#include <memory>

namespace draw::method
{
class Base;
}

class SceneItem: public QGraphicsItem {

  QRectF _boundRect;
  const std::unique_ptr<const draw::method::Base> drawMethod;
public:
  ~SceneItem() override;
  SceneItem(draw::method::Base const *method,  QRectF bound, QGraphicsItem *parent= {});
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  void resize(QRectF rectNew);
};

#endif // INC_2SHOOT_SCENEITEM_HPP
