//
// Created by dym on 08.04.2021.
//
#include "SceneCreator.hpp"
#include <QDebug>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QJsonArray>
#include <QJsonObject>
#include <utility>

#ifdef TO_LITERAL_STRING
  #error "TO_LITERAL_STRING is defined"
#endif
#define TO_LITERAL_STRING(in) #in

class SceneCreator::CollidingIgnore {
  QGraphicsScene *scene;
  std::vector<QGraphicsItem *> items;

public:
  CollidingIgnore()                       = delete;
  CollidingIgnore(CollidingIgnore const &)= delete;
  CollidingIgnore &operator=(CollidingIgnore const &)= delete;
  CollidingIgnore(CollidingIgnore &&)                = default;
  CollidingIgnore &operator=(CollidingIgnore &&)= default;

  CollidingIgnore(QGraphicsScene *sceneIn)
      : scene(sceneIn)
  {}
  ~CollidingIgnore()
  {
    for (auto ptr : items)
      scene->addItem(ptr);
  }
  void add(QGraphicsItem *in)
  {
    if (!in)
      return;
    items.push_back(in);
    scene->removeItem(in);
  }
  void remove(QGraphicsItem *in)
  {
    auto const erCount= std::erase(items, in);
    scene->addItem(in);
    assert(erCount == 1);
  }
};

class SceneCreator::DrawingProcessControl {
  QPointF _begin;

public:
  std::unique_ptr<QGraphicsItem, SceneBase::GraphicsItemDeleter> figure;

  DrawingProcessControl(QPointF in, QGraphicsScene *scene)
      : _begin(in)
      , figure(nullptr, scene)
  {}
  QPointF begin()
  {
    return _begin;
  }
};

SceneCreator::~SceneCreator()
{}

SceneCreator::SceneCreator(QPolygonF polygonFIn, QObject *parent)
    : SceneBase(std::move(polygonFIn), parent)
    , drawingFigureMethods{ drawingFigureMethodsDefault() }
{}

SceneCreator::SceneCreator(const QJsonObject &serial, QObject *parent)
    : SceneBase(serial, parent)
    , drawingFigureMethods{ drawingFigureMethodsDefault() }
{
  auto userShapesPacked(serial[TO_LITERAL_STRING(_figuresUser)].toArray());
  assert(userShapesPacked.size() == sizeof(drawingFigureMethods) / sizeof(drawingFigureMethods[0]));

  for (size_t i{}; i < userShapesPacked.size(); ++i)
    for (auto shape : userShapesPacked[i].toArray())
      _figuresUser.emplace_back(i, drawingFigureMethods[i](SceneBase::deserializeRectF(shape.toArray())));
}

void SceneCreator::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  if (figureSelector != -1) {
    assert(!drawingProcessControl);
    if (SceneBase::contains(event->scenePos())) {
      drawingProcessControl.reset(new DrawingProcessControl(event->scenePos(), this));
    }
  }
  QGraphicsScene::mousePressEvent(event);
}

void SceneCreator::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if (figureSelector != -1 && drawingProcessControl) {
    auto oldFigure= std::move(drawingProcessControl->figure);
    drawingProcessControl->figure.reset(
        drawingFigureMethods[figureSelector]({ drawingProcessControl->begin(), event->scenePos() }));
    {
      auto raii(collidingIgnore(oldFigure.get()));
      if (!collidingItems(drawingProcessControl->figure.get()).isEmpty()) {
        raii.remove(oldFigure.get());
        drawingProcessControl->figure= std::move(oldFigure);
        qDebug() << "colliding";
      }
    }
  }
  QGraphicsScene::mouseMoveEvent(event);
}

void SceneCreator::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  if (figureSelector != -1 && drawingProcessControl) {
    assert(drawingProcessControl->figure);
    _figuresUser.emplace_back(figureSelector, drawingProcessControl->figure.release());
    drawingProcessControl.reset();
  }
  QGraphicsScene::mouseReleaseEvent(event);
}

template<typename... Args>
SceneCreator::CollidingIgnore SceneCreator::collidingIgnore(Args &&...args)
{
  CollidingIgnore raii(this);

  (raii.add(std::forward<Args>(args)), ...);
  return std::move(raii);
}

QJsonObject SceneCreator::serialize() const
{
  QJsonArray userShapes[sizeof(drawingFigureMethods) / sizeof(drawingFigureMethods[0])];
  for (auto figure : _figuresUser)
    userShapes[figure.first].push_back(SceneBase::serialize(figure.second->sceneBoundingRect()));

  QJsonArray userShapesPacked;
  for (auto &shapes : userShapes)
    userShapesPacked.push_back(std::move(shapes));

  auto serial(SceneBase::serialize());
  serial[TO_LITERAL_STRING(_figuresUser)]= std::move(userShapesPacked);

  return std::move(serial);
}

decltype(SceneCreator::drawingFigureMethods) SceneCreator::drawingFigureMethodsDefault()
{
  return { [this](const QRectF &rect) -> QGraphicsItem * { return addEllipse(rect, colorFigure, colorFigure); },
           [this](const QRectF &rect) -> QGraphicsItem * { return addPolygon(QPolygonF(rect), colorFigure, colorFigure); },
           [this](const QRectF &rect) -> QGraphicsItem *
           {
             QPen pen(colorFigure);
             pen.setWidth(4);
             return addLine(rect.x(), rect.y(), rect.x() + rect.width(), rect.y() + rect.height(), pen);
           } };
}
