//
// Created by dym on 11.04.2021.
//

#include "UserInputCreator.hpp"
#include <FigureSelectorAdapter.hpp>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <Scene.hpp>
#include <memory>

#include <QDebug>

class UserInputCreator::DrawingProcess {
  QPointF const _begin;
  QPointF _end;
  std::unique_ptr<QGraphicsItem, Scene::GraphicsItemDeleter> _figure;

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

UserInputCreator::~UserInputCreator()= default;
UserInputCreator::UserInputCreator(Scene *scene)
    : UserInputBase(scene)
{}
void UserInputCreator::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  if (*figureSelector) {
    assert(!drawingProcess);
    if (scene->contains(event->scenePos())) {
      drawingProcess= std::make_unique<DrawingProcess>(event->scenePos(), scene);
    }
  }
}
void UserInputCreator::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  if (*figureSelector && drawingProcess) {
    auto oldFigure(std::move(*drawingProcess));

    {
      QGraphicsItem *newFigure;
      if (figureSelector)

        drawingProcess->set(
            Scene::drawingFigureMethods[figureSelector->i](scene, { drawingProcess->begin(), event->scenePos() }, *colorPainting),
            event->scenePos());
    }

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
  if (*figureSelector && drawingProcess) {
    scene->_figuresUser[figureSelector->i].emplace_back(
        Scene::DrawingPath{ drawingProcess->begin(), drawingProcess->end(), *colorPainting }, drawingProcess->release());
    drawingProcess.reset();
  }
}