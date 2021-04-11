//
// Created by dym on 11.04.2021.
//

#ifndef INC_2SHOOT_SERIALIZE_HPP
#define INC_2SHOOT_SERIALIZE_HPP
#include <QColor>
#include <QJsonArray>
#include <QPointF>
#include <QPolygonF>
#include <QRectF>

namespace json
{
QJsonArray serialize(QPointF);
QJsonArray serialize(const QPolygonF &);
QJsonArray serialize(QRectF);
QString serialize(const QColor &);

QPointF deserializePointF(QJsonArray const &);
QPolygonF deserializePolygonF(QJsonArray const &);
QRectF deserializeRectF(QJsonArray const &);
QColor deserializeColor(const QString &);

} // namespace json
#endif // INC_2SHOOT_SERIALIZE_HPP
