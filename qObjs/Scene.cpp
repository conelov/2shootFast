//
// Created by dym on 10.04.2021.
//

#include "Scene.hpp"
#include <QGraphicsPolygonItem>
#include <QJsonArray>
#include <QJsonObject>
#include <UserInputBase.hpp>

#include <QDebug>
#include <utility>

#ifdef TO_LITERAL_STRING
  #error "TO_LITERAL_STRING is defined"
#endif
#define TO_LITERAL_STRING(in) #in

Scene::DrawingPath Scene::DrawingPath::deserialize(const QJsonObject &json)
{
  DrawingPath ret;
  ret.color      = Scene::deserializeColor(json["color"].toString());
  auto const path= json["pos"].toArray();
  ret.begin      = Scene::deserializePointF(path[0].toArray());
  ret.end        = Scene::deserializePointF(path[1].toArray());
  return std::move(ret);
}

void Scene::GraphicsItemDeleter::operator()(QGraphicsItem *item) const
{
  scene->removeItem(item);
  delete item;
}
Scene::GraphicsItemDeleter::GraphicsItemDeleter(QGraphicsScene *sceneIn)
    : scene(sceneIn)
{}

QJsonObject Scene::DrawingPath::serialize() const
{
  QJsonObject json;
  json["pos"]  = QJsonArray() << Scene::serialize(begin) << Scene::serialize(end);
  json["color"]= Scene::serialize(color);
  return std::move(json);
}

const Scene::DrawingFigureMethods Scene::drawingFigureMethods[3]= {
  [](QGraphicsScene *const scene, QRectF const &rectF, QColor const &color) -> QGraphicsItem * {
    return scene->addEllipse(rectF, color, color);
  },
  [](QGraphicsScene *const scene, QRectF const &rectF, QColor const &color) -> QGraphicsItem * {
    return scene->addPolygon(QPolygonF(rectF), color, color);
  },
  [](QGraphicsScene *const scene, QRectF const &rect, QColor const &color) -> QGraphicsItem *
  {
    QPen pen(color);
    pen.setWidth(4);
    return scene->addLine(rect.x(), rect.y(), rect.x() + rect.width(), rect.y() + rect.height(), pen);
  }
};

QJsonArray Scene::serialize(QPointF const pointF)
{
  return QJsonArray() << pointF.x() << pointF.y();
}
QJsonArray Scene::serialize(const QPolygonF &polygonF)
{
  QJsonArray array;
  for (auto const point : polygonF)
    array.push_back(serialize(point));

  return std::move(array);
}
QJsonArray Scene::serialize(QRectF const rectF)
{
  return QJsonArray() << rectF.x() << rectF.y() << rectF.width() << rectF.height();
}
QString Scene::serialize(QColor const &color)
{
  return color.operator QVariant().toByteArray();
}

QPointF Scene::deserializePointF(const QJsonArray &array)
{
  return QPointF(array[0].toDouble(), array[1].toDouble());
}
QPolygonF Scene::deserializePolygonF(const QJsonArray &array)
{
  QPolygonF polygonF;
  for (auto const &point : array)
    polygonF.push_back(deserializePointF(point.toArray()));
  return std::move(polygonF);
}
QRectF Scene::deserializeRectF(const QJsonArray &array)
{
  return QRectF(array[0].toDouble(), array[1].toDouble(), array[2].toDouble(), array[3].toDouble());
}
QColor Scene::deserializeColor(const QString &str)
{
  return QVariant(str.toUtf8()).value<QColor>();
}

Scene::~Scene()= default;
Scene::Scene(QObject *parent)
    : QGraphicsScene(parent)
{}
Scene::Scene(QPolygonF polygonFIn, QObject *parent)
    : Scene(parent)
{
  _borderPolygon= std::move(polygonFIn);
  constructBorderLines();
}
Scene::Scene(QJsonObject const &jsonObject, QObject *parent)
    : Scene(parent)
{
  Scene::deserialize(jsonObject);
}
QJsonObject Scene::serialize() const
{
  assert(!_borderPolygon.empty());
  assert(!_borderLines.empty());

  QJsonObject serial;
  serial["name"]                           = objectName();
  serial[TO_LITERAL_STRING(_borderPolygon)]= serialize(_borderPolygon);
  serial["sceneRect"]                      = serialize(sceneRect());

  {
    constexpr auto figureCount= sizeof(_figuresUser) / sizeof(_figuresUser[0]);
    QJsonArray userShapes[figureCount];
    for (size_t i{}; i < figureCount; ++i)
      for (auto const &figure : _figuresUser[i])
        userShapes[i].push_back(figure.first.serialize());

    QJsonArray userShapesPacked;
    for (auto &shapes : userShapes)
      userShapesPacked.push_back(std::move(shapes));

    serial[TO_LITERAL_STRING(_figuresUser)]= std::move(userShapesPacked);
  }
  return std::move(serial);
}
void Scene::deserialize(QJsonObject const &json)
{
  assert(_borderPolygon.empty());
  assert(_borderLines.empty());
  assert(std::all_of(std::cbegin(_figuresUser), std::cend(_figuresUser), [](auto const v) { return v.empty(); }));

  setObjectName(json["name"].toString());
  setSceneRect(deserializeRectF(json["sceneRect"].toArray()));
  _borderPolygon= deserializePolygonF(json[TO_LITERAL_STRING(_borderPolygon)].toArray());
  constructBorderLines();
  {
    constexpr auto figureCount= sizeof(_figuresUser) / sizeof(_figuresUser[0]);
    auto const shapesPack     = json[TO_LITERAL_STRING(_figuresUser)].toArray();
    assert(shapesPack.size() == figureCount);
    for (size_t i{}; i < figureCount; ++i) {
      auto const shapes= shapesPack[i].toArray();
      for (auto const shape : shapes) {
        auto const path= DrawingPath::deserialize(shape.toObject());
        _figuresUser[i].emplace_back(path, drawingFigureMethods[i](this, { path.begin, path.end }, path.color));
      }
    }
  }
}
void Scene::constructBorderLines()
{
  assert(_borderLines.empty());
  assert(!_borderPolygon.empty());
  for (auto prevIt= _borderPolygon.cbegin(), currIt= std::next(prevIt); currIt != _borderPolygon.cend();
       prevIt= std::exchange(currIt, std::next(currIt))) {
    _borderLines.emplace_back(addLine(prevIt->x(), prevIt->y(), currIt->x(), currIt->y(), QPen(Qt::black)), this);
  }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  inputHandler->mousePressEvent(event);
  QGraphicsScene::mousePressEvent(event);
}
void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  inputHandler->mouseMoveEvent(event);
  QGraphicsScene::mouseMoveEvent(event);
}
void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  inputHandler->mouseReleaseEvent(event);
  QGraphicsScene::mouseReleaseEvent(event);
}
