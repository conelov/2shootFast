//
// Created by dym on 17.04.2021.
//

#ifndef INC_2SHOOT_PRINTMETHODS_HPP
#define INC_2SHOOT_PRINTMETHODS_HPP
#include <QMetaType>
#include <memory>
#include <QPen>
#include <QBrush>
class QPainter;

namespace draw::method
{
class Base {
public:
  enum Type : quint8
  {
    filling= 0,
    player,
    max_value= player
  };
  //
  //  std::unique_ptr<QPen> pen{};
  //  std::unique_ptr<QBrush> brush{};

  QPen pen{};
  QBrush brush{};

  virtual ~Base()                                          = default;
  virtual int id() const                                          = 0;
  virtual Type type() const                                       = 0;
  virtual void paint(QPainter *painter, QRectF bound) const= 0;
};

class Filling: public Base {
public:
  Type type() const  override;
};

class Player: public Base {
public:
  Type type() const  override;
};

class Line;
class Rectangle;
class Circle;
} // namespace draw::methods
Q_DECLARE_METATYPE(draw::method::Line)
Q_DECLARE_METATYPE(draw::method::Rectangle)
Q_DECLARE_METATYPE(draw::method::Circle)
namespace draw::method
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

const int allId[]{ qMetaTypeId<Line>(), qMetaTypeId<Rectangle>(), qMetaTypeId<Circle>() };
constexpr auto allIdCount= sizeof(allId) / sizeof(allId[0]);
} // namespace draw::methods
#endif // INC_2SHOOT_PRINTMETHODS_HPP
