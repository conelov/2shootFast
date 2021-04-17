//
// Created by dym on 17.04.2021.
//

#ifndef INC_2SHOOT_PRINTMETHODS_HPP
#define INC_2SHOOT_PRINTMETHODS_HPP
#include <QBrush>
#include <QMetaType>
#include <QPen>
#include <memory>
class QPainter;

namespace draw
{
class Base {
public:
  enum Role : quint8
  {
    filling= 0,
    player,
    RoleMax  = player,
    RoleCount= RoleMax + 1
  } /*const role*/;
  using TypeUnderlying= std::underlying_type_t<Role>;
  enum Type : TypeUnderlying
  {
    rotatable= 0,
    non_rotatable,
    TypeMax  = non_rotatable,
    TypeCount= TypeMax + 1
  } /*const type*/;

  QPen pen{};
  QBrush brush{};

  virtual ~Base()                                          = default;
  virtual int id() const                                   = 0;
  virtual Role role() const                                = 0;
  virtual Type type() const                                = 0;
  virtual void paint(QPainter *painter, QRectF bound) const= 0;

  friend QDataStream &operator<<(QDataStream &stream, Base const *method);
  friend QDataStream &operator>>(QDataStream &stream, Base *&method);
};

class Line;
class Rectangle;
class Circle;

class PTriangle;
} // namespace draw
Q_DECLARE_METATYPE(draw::Line)
Q_DECLARE_METATYPE(draw::Rectangle)
Q_DECLARE_METATYPE(draw::Circle)
Q_DECLARE_METATYPE(draw::PTriangle)
namespace draw
{
class Line: public Base {
public:
  int id() const override;
  Role role() const override;
  Type type() const override;
  void paint(QPainter *painter, QRectF bound) const override;
};
class Rectangle: public Base {
public:
  int id() const override;
  Role role() const override;
  Type type() const override;
  void paint(QPainter *painter, QRectF bound) const override;
};
class Circle: public Base {
public:
  int id() const override;
  Role role() const override;
  Type type() const override;
  void paint(QPainter *painter, QRectF bound) const override;
};

class PTriangle: public Base {
public:
  int id() const override;
  Role role() const override;
  Type type() const override;
  void paint(QPainter *painter, QRectF bound) const override;
};

const std::invoke_result_t<decltype(&Base::id), Base> allId[]{
  qMetaTypeId<Line>(), qMetaTypeId<Rectangle>(), qMetaTypeId<Circle>(), qMetaTypeId<PTriangle>()
};
constexpr auto allIdCount= sizeof(allId) / sizeof(allId[0]);
} // namespace draw
#endif // INC_2SHOOT_PRINTMETHODS_HPP
