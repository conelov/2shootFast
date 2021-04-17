//
// Created by dym on 15.04.2021.
//
#include "InputHandler.hpp"
#include "PrintMethods.hpp"
#include "SceneItem.hpp"
#include "moc/Scene.hpp"
#include <QColor>
#include <QGraphicsSceneMouseEvent>

InputHandlerBase::~InputHandlerBase()= default;

struct DrawHandler::PainterPath
{
  QPointF begin;
  SceneItem *item;
};

DrawHandler::~DrawHandler()= default;
DrawHandler::DrawHandler() = default;
void DrawHandler::mousePressEvent(Scene *scene, QGraphicsSceneMouseEvent *event)
{
  if (!method)
    return;
  assert(!path);
  path.reset(new PainterPath{ event->scenePos(), new SceneItem(method, { event->scenePos(), event->scenePos() }) });
  scene->addItem(path->item);
  scene->userItems.push_back(path->item);
}
void DrawHandler::mouseMoveEvent(Scene *scene, QGraphicsSceneMouseEvent *event)
{
  if (!method)
    return;
  QRectF const rectItemNew{ path->begin, event->scenePos() }, rectItemPrev= path->item->sceneBoundingRect();
  path->item->resize(rectItemNew);
  /// TODO: описать область обновления во время перерисовки
  scene->update();
  //  scene->update(rectItemNew);
  //  scene->update(rectItemPrev);
}
void DrawHandler::mouseReleaseEvent(Scene *scene, QGraphicsSceneMouseEvent *event)
{
  if (!method)
    return;
  path.reset();
}
