//
// Created by dym on 15.04.2021.
//
#include "SceneItem.hpp"
#include "Paint.hpp"

SceneItem::~SceneItem()= default;
SceneItem::SceneItem(draw::Painter const &method, QRectF bound, QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , _boundRect(bound)
    , drawMethod(new draw::Painter(method))
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
