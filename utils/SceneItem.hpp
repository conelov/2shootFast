//
// Created by dym on 15.04.2021.
//

#ifndef INC_2SHOOT_SCENEITEM_HPP
#define INC_2SHOOT_SCENEITEM_HPP
#include <QGraphicsItem>
#include <memory>

namespace draw
{
class Base;
}

class SceneItem: public QGraphicsItem {

  QRectF _boundRect;
  std::unique_ptr<const draw::Base> drawMethod;

  SceneItem();

public:
  ~SceneItem() override;
  SceneItem(draw::Base const *method, QRectF bound, QGraphicsItem *parent= {});
  QRectF boundingRect() const override;
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  void resize(QRectF rectNew);
  friend QDataStream &operator<<(QDataStream &stream, SceneItem const *item);
  friend QDataStream &operator>>(QDataStream &stream, SceneItem *&item);
};

#endif // INC_2SHOOT_SCENEITEM_HPP
