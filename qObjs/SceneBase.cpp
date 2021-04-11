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

QJsonObject SceneBase::DrawingPath::serialize() const
{
  QJsonObject json;
  json["pos"]  = QJsonArray() << SceneBase::serialize(begin) << SceneBase::serialize(end);
  json["color"]= SceneBase::serialize(color);
  return std::move(json);
}
SceneBase::DrawingPath SceneBase::DrawingPath::deserialize(const QJsonObject &json)
{
  DrawingPath ret;
  ret.color      = SceneBase::deserializeColor(json["color"].toString());
  auto const path= json["pos"].toArray();
  ret.begin      = SceneBase::deserializePointF(path[0].toArray());
  ret.end        = SceneBase::deserializePointF(path[1].toArray());
  return std::move(ret);
}

void SceneBase::GraphicsItemDeleter::operator()(QGraphicsItem *item) const
{
  scene->removeItem(item);
  delete item;
}
SceneBase::GraphicsItemDeleter::GraphicsItemDeleter(QGraphicsScene *sceneIn)
    : scene(sceneIn)
{}

SceneBase::CollidingIgnore::CollidingIgnore(QGraphicsScene *sceneIn)
    : scene(sceneIn)
{}
SceneBase::CollidingIgnore::~CollidingIgnore()
{
  for (auto ptr : items)
    scene->addItem(ptr);
}
SceneBase::CollidingIgnore &SceneBase::CollidingIgnore::add(QGraphicsItem *in)
{
  assert(in);
  items.push_back(in);
  scene->removeItem(in);
  return *this;
}
SceneBase::CollidingIgnore &SceneBase::CollidingIgnore::remove(QGraphicsItem *in)
{
  assert(in);
  auto const erCount= std::erase(items, in);
  scene->addItem(in);
  assert(erCount == 1);
  return *this;
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
  return QJsonArray() << rectF.x() << rectF.y() << rectF.width() << rectF.height();
}
QString SceneBase::serialize(QColor const &color)
{
  return color.operator QVariant().toByteArray();
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
  return QRectF(array[0].toDouble(), array[1].toDouble(), array[2].toDouble(), array[3].toDouble());
}
QColor SceneBase::deserializeColor(const QString &str)
{
  return QVariant(str.toUtf8()).value<QColor>();
}

SceneBase::~SceneBase()= default;
SceneBase::SceneBase(QObject *parent)
    : QGraphicsScene(parent)
{}
SceneBase::SceneBase(QPolygonF polygonFIn, QObject *parent)
    : SceneBase(parent)
{
  _borderPolygon= std::move(polygonFIn);
  constructBorderLines();
}
SceneBase::SceneBase(QJsonObject const &jsonObject, QObject *parent)
    : SceneBase(parent)
{
  SceneBase::deserialize(jsonObject);
}
QJsonObject SceneBase::serialize() const
{
  assert(!_borderPolygon.empty());
  assert(!_borderLines.empty());
  QJsonObject serial;
  serial["name"]                           = objectName();
  serial[TO_LITERAL_STRING(_borderPolygon)]= serialize(_borderPolygon);
  serial["sceneRect"]                      = serialize(sceneRect());

  return std::move(serial);
}
void SceneBase::deserialize(QJsonObject const &json)
{
  assert(_borderPolygon.empty());
  assert(_borderLines.empty());
  setObjectName(json["name"].toString());
  setSceneRect(deserializeRectF(json["sceneRect"].toArray()));
  _borderPolygon= deserializePolygonF(json[TO_LITERAL_STRING(_borderPolygon)].toArray());
  constructBorderLines();
}
SceneBase::CollidingIgnore SceneBase::collidingIgnore()
{
  return CollidingIgnore(this);
}
void SceneBase::constructBorderLines()
{
  assert(_borderLines.empty());
  assert(!_borderPolygon.empty());
  for (auto prevIt= _borderPolygon.cbegin(), currIt= std::next(prevIt); currIt != _borderPolygon.cend();
       prevIt= std::exchange(currIt, std::next(currIt))) {
    _borderLines.emplace_back(addLine(prevIt->x(), prevIt->y(), currIt->x(), currIt->y(), QPen(Qt::black)), this);
  }
}
