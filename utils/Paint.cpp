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
Selector::operator bool() const
{
  return type != none;
}
Methods::Methods()
{
  /// FIGURE
  {
    /// LINE
    QRectF const stdRect(-10, -10, 20, 20);
    QColor const stdColor= Qt::blue;
    QBrush const stdBrush= stdColor;
    QPen const stdPen(stdBrush, 4);
    methods[0].push_back(Painter{ [](QPainter *painter, QRectF rect, QPen const &pen, QBrush const *brush)
                                   {
                                     painter->setPen(pen);
                                     painter->drawLine(rect.x(), rect.y(), rect.x() + rect.width(), rect.y() + rect.height());
                                     Q_UNUSED(brush)
                                   },
                                   stdRect,
                                   stdPen });
    /// RECTANGLE
    methods[0].push_back(Painter{ [](QPainter *painter, QRectF rect, QPen const &pen, QBrush const *brush)
                                   {
                                     auto const dec= pen.width() / 2.;
                                     rect.adjust(dec, dec, -dec, -dec);
                                     painter->setPen(pen);
                                     painter->setBrush(*brush);
                                     painter->drawRect(rect);
                                   },
                                   stdRect,
                                   stdPen,
                                   stdBrush });
    /// CIRCLE
    methods[0].push_back(Painter{ [](QPainter *painter, QRectF rect, QPen const &pen, QBrush const *brush)
                                   {
                                    painter->setRenderHint(QPainter::Antialiasing);
                                    auto const dec= pen.width() / 2.;
                                     rect.adjust(dec, dec, -dec, -dec);
                                     painter->setPen(pen);
                                     painter->setBrush(*brush);
                                     painter->drawEllipse(rect);
                                   },
                                   stdRect,
                                   stdPen,
                                   stdBrush });
  }
  /// PLAYER
  {
    QRectF const stdRect(-10, -10, 20, 20);
    QColor const stdColor= Qt::cyan;
    QBrush const stdBrush= stdColor;
    QPen const stdPen(stdBrush, 5);
  }
}

void Painter::paint(QPainter *painter, QRectF rect, const QColor &color) const
{
  QPen pen= penBind;
  pen.setColor(color);
  if (brushBind) {
    QBrush brush= brushBind.value();
    brush.setColor(color.darker(125));
    paintFn(painter, rect, pen, &brush);
  } else
    paintFn(painter, rect, pen, {});
}
} // namespace draw
QDataStream &operator<<(QDataStream &stream, draw::Selector const item)
{
  stream << item.type << item.pos;
  return stream;
}
QDataStream &operator>>(QDataStream &stream, draw::Selector &item)
{
  draw::Selector::TypeEnumUnderlying type;
  stream >> type >> item.pos;
  item.type= static_cast<draw::Selector::Type>(type);
  return stream;
}
