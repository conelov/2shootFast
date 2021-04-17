//
// Created by dym on 17.04.2021.
//
#include "PrintMethods.hpp"
#include <QPainter>
namespace draw
{
Base::Type draw::Filling::type() const
{
  return filling;
}
Base::Type Player::type() const
{
  return player;
}
QDataStream &operator<<(QDataStream &stream, const Base *method)
{
  stream << method->id()
#ifndef NDEBUG
         << method->type()
#endif
         << method->pen << method->brush;
  return stream;
}
QDataStream &operator>>(QDataStream &stream, Base *&item)
{
  assert(!item);
  std::invoke_result_t<decltype(&Base::id), Base> id{};
  stream >> id;
  assert(std::ranges::count(allId, id) == 1 && id != QMetaType::UnknownType);

  item= static_cast<Base *>(QMetaType::create(id));

#ifndef NDEBUG
  {
    Base::TypeUnderlying typeUnderlying{};
    stream >> typeUnderlying;
    Base::Type type= static_cast<Base::Type>(typeUnderlying);
    assert(type >= Base::Type::filling && type <= Base::Type::max_value);
    assert(item->type() == type);
  }
#endif
  stream >> item->pen >> item->brush;

  return stream;
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
} // namespace draw