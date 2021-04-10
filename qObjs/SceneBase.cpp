//
// Created by dym on 10.04.2021.
//

#include "SceneBase.hpp"
#include <QGraphicsPolygonItem>
#include <QJsonArray>
#include <QJsonObject>

#include <QDebug>
#include <utility>

#ifdef TO_LITERAL_STRING
  #error "TO_LITERAL_STRING is defined"
#endif
#define TO_LITERAL_STRING(in) #in

void SceneBase::GraphicsItemDeleter::operator()(QGraphicsItem *item) const
{
  scene->removeItem(item);
  delete item;
}

SceneBase::GraphicsItemDeleter::GraphicsItemDeleter(QGraphicsScene *sceneIn)
    : scene(sceneIn)
{}

SceneBase::~SceneBase()
{}

SceneBase::SceneBase(QPolygonF polygonFIn, QObject *parent)
    : QGraphicsScene(parent)
    , _borderPolygon(std::move(polygonFIn))
    , _borderLines(
          [this]
          {
            std::remove_cvref_t<decltype(_borderLines)> ret;
            assert(_borderPolygon.isClosed());
            for (auto prevIt= _borderPolygon.cbegin(), currIt= std::next(prevIt); currIt != _borderPolygon.cend();
                 prevIt= std::exchange(currIt, std::next(currIt))) {
              ret.emplace_back(addLine(prevIt->x(), prevIt->y(), currIt->x(), currIt->y(), QPen(Qt::black)), this);
            }
            return std::move(ret);
          }())
{}

SceneBase::SceneBase(QJsonObject const & jsonObject, QObject *parent)
    : SceneBase(
          [&jsonObject]() { return deserializePolygonF(jsonObject[TO_LITERAL_STRING(_borderPolygon)].toArray()); }(), parent)
{
  setObjectName(jsonObject["name"].toString());
  setSceneRect(deserializeRectF(jsonObject["scenePos"].toArray()));
}

QJsonObject SceneBase::serialize() const
{
  QJsonObject serial;
  serial["name"]                           = objectName();
  serial[TO_LITERAL_STRING(_borderPolygon)]= serialize(_borderPolygon);
  serial["scenePos"]                       = serialize(sceneRect());

  return std::move(serial);
}

QJsonArray SceneBase::serialize(QPointF const pointF)
{
  return QJsonArray() << pointF.x() << pointF.y();
}
QJsonArray SceneBase::serialize(const QPolygonF &polygonF)
{
  QJsonArray array;
  for (auto const point : polygonF)
    array.push_back(serialize(point));

  return std::move(array);
}
QJsonArray SceneBase::serialize(QRectF const rectF)
{
  qDebug() << "serial rect" << rectF << '\n';
  return QJsonArray() << rectF.x() << rectF.y() << rectF.width() << rectF.height();
}

QPointF SceneBase::deserializePointF(const QJsonArray &array)
{
  return QPointF(array[0].toDouble(), array[1].toDouble());
}
QPolygonF SceneBase::deserializePolygonF(const QJsonArray &array)
{
  QPolygonF polygonF;
  for (auto const &point : array)
    polygonF.push_back(deserializePointF(point.toArray()));
  return std::move(polygonF);
}
QRectF SceneBase::deserializeRectF(const QJsonArray &array)
{
  qDebug() << "DeSerial rect" <<QRectF(array[0].toDouble(), array[1].toDouble(), array[2].toDouble(), array[3].toDouble()) << '\n';
  return QRectF(array[0].toDouble(), array[1].toDouble(), array[2].toDouble(), array[3].toDouble());
}