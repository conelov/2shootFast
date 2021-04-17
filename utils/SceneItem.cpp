//
// Created by dym on 15.04.2021.
//
#include "SceneItem.hpp"
#include "PrintMethods.hpp"

SceneItem::~SceneItem()= default;
SceneItem::SceneItem() = default;
SceneItem::SceneItem(draw::Base const *method, QRectF bound, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , _boundRect(bound)
    , drawMethod(static_cast<draw::Base *>(QMetaType::create(method->id(), method)))
{}
QRectF SceneItem::boundingRect() const
{
  return _boundRect;
}
void SceneItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  drawMethod->paint(painter, _boundRect);
}
void SceneItem::resize(QRectF rectNew)
{
  if (rectNew != _boundRect) {
    prepareGeometryChange();
    _boundRect= rectNew;
  }
}
QDataStream &operator<<(QDataStream &stream, const SceneItem *item)
{
  assert(item);
  stream << item->_boundRect << item->drawMethod.get();

  return stream;
}
QDataStream &operator>>(QDataStream &stream, SceneItem *&item)
{
  assert(!item);
  item= new SceneItem;
  draw::Base *draw{};
  stream >> item->_boundRect >> draw;
  item->drawMethod.reset(draw);

  return stream;
}
