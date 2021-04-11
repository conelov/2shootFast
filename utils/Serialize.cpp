//
// Created by dym on 11.04.2021.
//

#include "Serialize.hpp"
#include <QVariant>

QJsonArray json::serialize(QPointF const pointF)
{
  return QJsonArray() << pointF.x() << pointF.y();
}
QJsonArray json::serialize(const QPolygonF &polygonF)
{
  QJsonArray array;
  for (auto const point : polygonF)
    array.push_back(serialize(point));

  return std::move(array);
}
QJsonArray json::serialize(QRectF const rectF)
{
  return QJsonArray() << rectF.x() << rectF.y() << rectF.width() << rectF.height();
}
QString json::serialize(QColor const &color)
{
  return color.operator QVariant().toByteArray();
}

QPointF json::deserializePointF(const QJsonArray &array)
{
  return QPointF(array[0].toDouble(), array[1].toDouble());
}
QPolygonF json::deserializePolygonF(const QJsonArray &array)
{
  QPolygonF polygonF;
  for (auto const &point : array)
    polygonF.push_back(deserializePointF(point.toArray()));
  return std::move(polygonF);
}
QRectF json::deserializeRectF(const QJsonArray &array)
{
  return QRectF(array[0].toDouble(), array[1].toDouble(), array[2].toDouble(), array[3].toDouble());
}
QColor json::deserializeColor(const QString &str)
{
  return QVariant(str.toUtf8()).value<QColor>();
}
