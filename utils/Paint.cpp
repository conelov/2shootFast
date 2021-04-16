//
// Created by dym on 15.04.2021.
//
#include "Paint.hpp"
#include <QDataStream>
#include <QLine>
#include <QPainter>

#include <QDebug>

namespace draw
{
//Painter::Painter(Painter::Paint_fn fn, QRectF bound, QPen const &pen, decltype(brushBind) brush)
//    : paintFn(fn)
//    , boundingRect(bound)
//    , penBind(pen)
//    , brushBind(std::move(brush))
//{}
Painter::Painter(Painter::Paint_fn fn, const QPen &pen, const QBrush *brush)
    : paintFn(fn)
    , penBind(pen)
    , brushBind(brush ? decltype(brushBind)(*brush) : std::nullopt)
{}
void Painter::paint(QPainter *painter, QRectF bound, const QPen &pen, const QBrush *brush)
{
  penBind = pen;
  if(brush)
    brushBind= *brush;
  paintFn(painter, bound, penBind, brushBind ? &*brushBind : nullptr);
}
void Painter::paint(QPainter *painter, QRectF bound) const
{
  paintFn(painter, bound, penBind, brushBind ? &*brushBind : nullptr);
}

//Selector::operator bool() const
//{
//  return type != none;
//}

Methods::Methods()
{
  /// FIGURE
  {
    QColor const stdColor= Qt::blue;
    QBrush const stdBrush= stdColor;
    QPen const stdPen(stdBrush, 4);
    /// LINE
    methods[0].push_back(Painter(
        [](QPainter *painter, QRectF rect, QPen const &pen, QBrush const *brush)
        {
          painter->setPen(pen);
          painter->drawLine(rect.x(), rect.y(), rect.x() + rect.width(), rect.y() + rect.height());
          Q_UNUSED(brush)
        },
        stdPen));
    /// RECTANGLE
    methods[0].push_back(Painter(
        [](QPainter *painter, QRectF rect, QPen const &pen, QBrush const *brush)
        {
          auto const dec= pen.width() / 2.;
          rect.adjust(dec, dec, -dec, -dec);
          painter->setPen(pen);
          painter->setBrush(*brush);
          painter->drawRect(rect);
        },
        stdPen,
        &stdBrush));
    /// CIRCLE
    methods[0].push_back(Painter(
        [](QPainter *painter, QRectF rect, QPen const &pen, QBrush const *brush)
        {
          painter->setRenderHint(QPainter::Antialiasing);
          auto const dec= pen.width() / 2.;
          rect.adjust(dec, dec, -dec, -dec);
          painter->setPen(pen);
          painter->setBrush(*brush);
          painter->drawEllipse(rect);
        },
        stdPen,
        &stdBrush));
  }
  /// PLAYER
  {
    QRectF const stdRect(-10, -10, 20, 20);
    QColor const stdColor= Qt::cyan;
    QBrush const stdBrush= stdColor;
    QPen const stdPen(stdBrush, 5);
  }
}
//Painter const &Methods::operator[](Selector select) const
//{
//  assert(select);
//  return methods[select.type - 1][select.pos];
//}
} // namespace draw
//QDataStream &operator<<(QDataStream &stream, draw::Selector const item)
//{
//  stream << item.type << item.pos;
//  return stream;
//}
//QDataStream &operator>>(QDataStream &stream, draw::Selector &item)
//{
//  draw::Selector::TypeEnumUnderlying type;
//  stream >> type >> item.pos;
//  item.type= static_cast<draw::Selector::Type>(type);
//  return stream;
//}
