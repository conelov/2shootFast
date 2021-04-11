//
// Created by dym on 11.04.2021.
//

#include "UserInputCreator.hpp"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <SceneBase.hpp>
#include <memory>

#include <QDebug>

class UserInputCreator::DrawingProcess {
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

const UserInputCreator::DrawingFigureMethods UserInputCreator::drawingFigureMethods[3]= {
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

UserInputCreator::~UserInputCreator()= default;
UserInputCreator::UserInputCreator(SceneBase *scene)
    : UserInputBase(scene)
{}
void UserInputCreator::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  if (*figureSelector != -1) {
    assert(!drawingProcess);
    if (scene->contains(event->scenePos())) {
      drawingProcess= std::make_unique<DrawingProcess>(event->scenePos(), scene);
    }
  }
}
void UserInputCreator::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if (*figureSelector != -1 && drawingProcess) {
    auto oldFigure(std::move(*drawingProcess));

    drawingProcess->set(
        drawingFigureMethods[*figureSelector](scene, { drawingProcess->begin(), event->scenePos() }, *colorPainting),
        event->scenePos());
    auto raii(collidingIgnore());
    if (oldFigure)
      raii.add(oldFigure.figure());
    if (!scene->collidingItems(drawingProcess->figure()).isEmpty()) {
      if (oldFigure)
        raii.remove(oldFigure.figure());
      *drawingProcess= std::move(oldFigure);

      static size_t counter{};
      qDebug() << "colliding" << ++counter;
    }
  }
}
void UserInputCreator::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  if (*figureSelector != -1 && drawingProcess) {
    scene->_figuresUser[*figureSelector].emplace_back(
        SceneBase::DrawingPath{ drawingProcess->begin(), drawingProcess->end(), *colorPainting }, drawingProcess->release());
    drawingProcess.reset();
  }
}