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
    for (auto shape : userShapesPacked[i].toArray()) {
      auto const path(DrawingPath::deserialize(shape.toObject()));
      _figuresUser[i].emplace_back(path, drawingFigureMethods[i](QRectF(path.begin, path.end)));
    }
}

QJsonObject SceneCreator::serialize() const
{
  constexpr auto methodsCount= sizeof(drawingFigureMethods) / sizeof(drawingFigureMethods[0]);
  QJsonArray userShapes[methodsCount];
  for (size_t i{}; i < methodsCount; ++i)
    for (auto const figure : _figuresUser[i])
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
      drawingProcess= std::make_unique<DrawingProcess>(event->scenePos(), this);
    }
  }
  QGraphicsScene::mousePressEvent(event);
}

void SceneCreator::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if (figureSelector != -1 && drawingProcess) {
    auto oldFigure(std::move(*drawingProcess));

    drawingProcess->set(drawingFigureMethods[figureSelector]({ drawingProcess->begin(), event->scenePos() }), event->scenePos());
    {
      auto raii(collidingIgnore(oldFigure.figure()));
      if (!collidingItems(drawingProcess->figure()).isEmpty()) {
        raii.remove(oldFigure.figure());
        *drawingProcess= std::move(oldFigure);
        qDebug() << "colliding";
      }
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

template<typename... Args>
SceneCreator::CollidingIgnore SceneCreator::collidingIgnore(Args &&...args)
{
  CollidingIgnore raii(this);

  (raii.add(std::forward<Args>(args)), ...);
  return std::move(raii);
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

QJsonObject SceneCreator::DrawingPath::serialize() const
{
  QJsonObject json;
  json["pos"]  = QJsonArray() << SceneBase::serialize(begin) << SceneBase::serialize(end);
  json["color"]= SceneBase::serialize(color);
  return std::move(json);
}
SceneCreator::DrawingPath SceneCreator::DrawingPath::deserialize(const QJsonObject &json)
{
  DrawingPath ret;
  ret.color      = SceneBase::deserializeColor(json["color"].toString());
  auto const path= json["pos"].toArray();
  ret.begin      = SceneBase::deserializePointF(path[0].toArray());
  ret.end        = SceneBase::deserializePointF(path[1].toArray());
  return std::move(ret);
}
