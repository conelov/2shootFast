//
// Created by dym on 17.04.2021.
//
#include "PrintMethods.hpp"
#include <QPainter>
namespace draw::method
{
Base::Type draw::method::Filling::type() const
{
  return filling;
}
Base::Type Player::type() const 
{
  return player;
}
int Line::id() const 
{
  return qMetaTypeId<Line>();
}
void Line::paint(QPainter *painter, QRectF rect) const
{
  painter->setPen(pen);
  painter->drawLine(rect.x(), rect.y(), rect.x() + rect.width(), rect.y() + rect.height());
}
int Rectangle::id() const 
{
  return qMetaTypeId<Rectangle>();
}
void Rectangle::paint(QPainter *painter, QRectF rect) const
{
  auto const dec= pen.width() / 2.;
  rect.adjust(dec, dec, -dec, -dec);
  painter->setPen(pen);
  painter->setBrush(brush);
  painter->drawRect(rect);
}
int Circle::id() const 
{
  return qMetaTypeId<Circle>();
}
void Circle::paint(QPainter *painter, QRectF rect) const
{
  painter->setRenderHint(QPainter::Antialiasing);
  auto const dec= pen.width() / 2.;
  rect.adjust(dec, dec, -dec, -dec);
  painter->setPen(pen);
  painter->setBrush(brush);
  painter->drawEllipse(rect);
}
} // namespace draw::methods