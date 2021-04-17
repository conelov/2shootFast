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
  enum Type : quint8
  {
    filling= 0,
    player,
    max_value= player
  };
  using TypeUnderlying= std::underlying_type_t<Type>;

  QPen pen{};
  QBrush brush{};

  virtual ~Base()                                          = default;
  virtual int id() const                                   = 0;
  virtual Type type() const                                = 0;
  virtual void paint(QPainter *painter, QRectF bound) const= 0;

  friend QDataStream &operator<<(QDataStream &stream, Base const *method);
  friend QDataStream &operator>>(QDataStream &stream, Base *&method);
};

class Filling: public Base {
public:
  Type type() const override;
};

class Player: public Base {
public:
  Type type() const override;
};

class Line;
class Rectangle;
class Circle;
} // namespace draw
Q_DECLARE_METATYPE(draw::Line)
Q_DECLARE_METATYPE(draw::Rectangle)
Q_DECLARE_METATYPE(draw::Circle)
namespace draw
{
class Line: public Filling {
public:
  int id() const override;
  void paint(QPainter *painter, QRectF bound) const override;
};
class Rectangle: public Filling {
public:
  int id() const override;
  void paint(QPainter *painter, QRectF bound) const override;
};
class Circle: public Filling {
public:
  int id() const override;
  void paint(QPainter *painter, QRectF bound) const override;
};

const std::invoke_result_t<decltype(&Base::id), Base> allId[]{ qMetaTypeId<Line>(),
                                                               qMetaTypeId<Rectangle>(),
                                                               qMetaTypeId<Circle>() };
constexpr auto allIdCount= sizeof(allId) / sizeof(allId[0]);
} // namespace draw
#endif // INC_2SHOOT_PRINTMETHODS_HPP
