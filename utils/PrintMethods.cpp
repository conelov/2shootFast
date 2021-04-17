//
// Created by dym on 17.04.2021.
//
#include "PrintMethods.hpp"
#include <QPainter>
namespace draw
{
QDataStream &operator<<(QDataStream &stream, const Base *method)
{
  stream << method->id()
         //#ifndef NDEBUG
         //         << method->role()
         //#endif
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

  //#ifndef NDEBUG
  //  {
  //    Base::TypeUnderlying typeUnderlying{};
  //    stream >> typeUnderlying;
  //    Base::Role role= static_cast<Base::Role>(typeUnderlying);
  //    assert(type >= Base::Role::filling && role <= Base::Role::RoleMax);
  //    assert(item->type() == role);
  //  }
  //#endif
  stream >> item->pen >> item->brush;

  return stream;
}

int Line::id() const
{
  return qMetaTypeId<Line>();
}
void Line::paint(QPainter *painter, QRectF rect) const
{
  painter->setRenderHint(QPainter::Antialiasing);
  painter->setPen(pen);
  painter->drawLine(rect.x(), rect.y(), rect.x() + rect.width(), rect.y() + rect.height());
}
Base::Role Line::role() const
{
  return Base::Role::filling;
}
Base::Type Line::type() const
{
  return Base::Type::non_rotatable;
}

int Rectangle::id() const
{
  return qMetaTypeId<Rectangle>();
}
void Rectangle::paint(QPainter *painter, QRectF rect) const
{
  {
    auto const dec= pen.width() / 2.;
    rect.adjust(dec, dec, -dec, -dec);
  }
  painter->setPen(pen);
  painter->setBrush(brush);
  painter->drawRect(rect);
}
Base::Role Rectangle::role() const
{
  return filling;
}
Base::Type Rectangle::type() const
{
  return non_rotatable;
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
Base::Role Circle::role() const
{
  return filling;
}
Base::Type Circle::type() const
{
  return non_rotatable;
}

int PTriangle::id() const
{
  return qMetaTypeId<PTriangle>();
}
void PTriangle::paint(QPainter *painter, QRectF bound) const
{
  {
    QPen penLocal= pen;
    penLocal.setWidth(2);
    penLocal.setColor(Qt::red);
    painter->setPen(pen);
  }
  {
    auto const dec= pen.width() / 2.;
    bound.adjust(dec, dec, -dec, -dec);
  }
  painter->setBrush(brush);
  painter->drawPolygon(
      QPolygon() << QPoint{ static_cast<int>(bound.x() + bound.width() / 2.), static_cast<int>(bound.y()) }
                 << QPoint{ static_cast<int>(bound.x()), static_cast<int>(bound.y() + bound.height()) }
                 << QPoint{ static_cast<int>(bound.x() + bound.width()), static_cast<int>(bound.y() + bound.height()) }
                 << QPoint{ static_cast<int>(bound.x() + bound.width() / 2.), static_cast<int>(bound.y()) });
}
Base::Role PTriangle::role() const
{
  return player;
}
Base::Type PTriangle::type() const
{
  return rotatable;
}
} // namespace draw