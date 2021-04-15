//
// Created by dym on 15.04.2021.
//

#ifndef INC_2SHOOT_PAINT_HPP
#define INC_2SHOOT_PAINT_HPP
#include <QMetaType>
#include <QRectF>
#include <QPen>

class QPainter;

namespace draw
{
struct Painter
{
  using Paint_fn= void (*)(QPainter *, QRectF, QPen const &, QBrush const *);
  Paint_fn paintFn{};
  QRectF boundingRect{};
  QPen penBind;
  std::optional<QBrush> brushBind{};

  void paint(QPainter *painter, QRectF rect, QColor const &color) const;
};

struct Selector
{
  enum Type : quint8
  {
    none,
    shape,
    player
  } type                  = none;
  using TypeEnumUnderlying= std::underlying_type_t<Type>;

  quint8 pos{};

  operator bool() const;
};

class Methods {
public:
  template<typename T>
  using Container                    = std::vector<T>;
  static constexpr auto paintersCount= 2;
  Container<Painter> methods[paintersCount];

  Methods();

  Painter const & operator[](Selector select);
};
} // namespace draw
Q_DECLARE_METATYPE(draw:: Selector)

QDataStream &operator<<(QDataStream &stream,draw:: Selector item);
QDataStream &operator>>(QDataStream &stream,draw:: Selector &item);

#endif // INC_2SHOOT_PAINT_HPP
