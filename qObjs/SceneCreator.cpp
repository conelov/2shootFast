//
// Created by dym on 08.04.2021.
//
#include "SceneCreator.hpp"
#include <QDebug>
#include <QGraphicsLineItem>
#include <QGraphicsSceneMouseEvent>
#include <QJsonArray>
#include <QJsonObject>
#include <memory>
#include <utility>

#ifdef TO_LITERAL_STRING
  #error "TO_LITERAL_STRING is defined"
#endif
#define TO_LITERAL_STRING(in) #in

const SceneCreator::DrawingFigureMethods SceneCreator::drawingFigureMethods[3]= {
  [](QGraphicsScene *const scene, QRectF const rectF, QColor const &color) -> QGraphicsItem * {
    return scene->addEllipse(rectF, color, color);
  },
  [](QGraphicsScene *const scene, QRectF const rectF, QColor const &color) -> QGraphicsItem * {
    return scene->addPolygon(QPolygonF(rectF), color, color);
  },
  [](QGraphicsScene *const scene, QRectF const rect, QColor const &color) -> QGraphicsItem *
  {
    QPen pen(color);
    pen.setWidth(4);
    return scene->addLine(rect.x(), rect.y(), rect.x() + rect.width(), rect.y() + rect.height(), pen);
  }
};

class SceneCreator::DrawingProcess {
  QPointF const _begin;
  QPointF _end;
  std::unique_ptr<QGraphicsItem, SceneBase::GraphicsItemDeleter> _figure;

public:
  DrawingProcess()                      = delete;
  DrawingProcess(DrawingProcess const &)= delete;
  DrawingProcess &operator=(DrawingProcess const &)= delete;
  DrawingProcess(DrawingProcess &&other) noexcept
      : _begin(other._begin)
      , _end(other._end)
      , _figure(std::move(other._figure))
  {}
  DrawingProcess &operator=(DrawingProcess &&other) noexcept
  {
    assert(_begin == other._begin);
    _end   = other._end;
    _figure= std::move(other._figure);
    return *this;
  };
  DrawingProcess(QPointF in, QGraphicsScene *scene)
      : _begin(in)
      , _end(_begin)
      , _figure(nullptr, scene)
  {}

  QGraphicsItem *release()
  {
    return _figure.release();
  }

  void set(QGraphicsItem *const figure, QPointF const end)
  {
    assert(!_figure);
    _figure.reset(figure);
    _end= end;
  }

  [[nodiscard]] QGraphicsItem *figure() const
  {
    return _figure.get();
  }

  void setFigure(QGraphicsItem *figure, QPointF end)
  {
    _figure.reset(figure);
    _end= end;
  }

  [[nodiscard]] QPointF begin() const
  {
    return _begin;
  }

  [[nodiscard]] QPointF end() const
  {
    return _end;
  }

  [[nodiscard]] operator bool() const
  {
    return _figure.operator bool();
  }
};

SceneCreator::~SceneCreator()= default;

SceneCreator::SceneCreator(QPolygonF polygonFIn, QObject *parent)
    : SceneBase(std::move(polygonFIn), parent)
{}

SceneCreator::SceneCreator(const QJsonObject &serial, QObject *parent)
    : SceneBase(serial, parent)
{
  auto userShapesPacked(serial[TO_LITERAL_STRING(_figuresUser)].toArray());
  assert(userShapesPacked.size() == sizeof(drawingFigureMethods) / sizeof(drawingFigureMethods[0]));

  for (size_t i{}; i < userShapesPacked.size(); ++i)
    for (auto shape : userShapesPacked[i].toArray()) {
      auto const path(DrawingPath::deserialize(shape.toObject()));
      assert(path.color != colorFigure);
      _figuresUser[i].emplace_back(path, drawingFigureMethods[i](this, QRectF(path.begin, path.end), path.color));
    }
}

QJsonObject SceneCreator::serialize() const
{
  constexpr auto methodsCount= sizeof(drawingFigureMethods) / sizeof(drawingFigureMethods[0]);
  QJsonArray userShapes[methodsCount];
  for (size_t i{}; i < methodsCount; ++i)
    for (auto const &figure : _figuresUser[i])
      userShapes[i].push_back(figure.first.serialize());

  QJsonArray userShapesPacked;
  for (auto &shapes : userShapes)
    userShapesPacked.push_back(std::move(shapes));

  auto serial(SceneBase::serialize());
  serial[TO_LITERAL_STRING(_figuresUser)]= std::move(userShapesPacked);

  return std::move(serial);
}

void SceneCreator::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  if (figureSelector != -1) {
    assert(!drawingProcess);
    if (SceneBase::contains(event->scenePos())) {
      drawingProcess.reset(new DrawingProcess(event->scenePos(), this));
    }
  }
  QGraphicsScene::mousePressEvent(event);
}

void SceneCreator::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if (figureSelector != -1 && drawingProcess) {
    auto oldFigure(std::move(*drawingProcess));

    drawingProcess->set(
        drawingFigureMethods[figureSelector](this, { drawingProcess->begin(), event->scenePos() }, colorFigure),
        event->scenePos());
    auto raii(collidingIgnore());
    if (oldFigure)
      raii.add(oldFigure.figure());
    if (!collidingItems(drawingProcess->figure()).isEmpty()) {
      if (oldFigure)
        raii.remove(oldFigure.figure());
      *drawingProcess= std::move(oldFigure);

      static size_t counter{};
      qDebug() << "colliding" << ++counter;
    }
  }
  QGraphicsScene::mouseMoveEvent(event);
}

void SceneCreator::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  if (figureSelector != -1 && drawingProcess) {
    assert(*drawingProcess);
    _figuresUser[figureSelector].emplace_back(
        DrawingPath{ drawingProcess->begin(), drawingProcess->end(), colorFigure }, drawingProcess->release());
    drawingProcess.reset();
  }
  QGraphicsScene::mouseReleaseEvent(event);
}