//
// Created by dym on 15.04.2021.
//
#include "InputManager.hpp"
#include "PrintMethods.hpp"
#include "SceneItem.hpp"
#include "moc/Scene.hpp"
#include <QColor>
#include <QGraphicsSceneMouseEvent>

InputManagerBase::~InputManagerBase()= default;

struct PainterManager::PainterPath
{
  QPointF begin;
  SceneItem *item;
};

PainterManager::~PainterManager()= default;
PainterManager::PainterManager() = default;
void PainterManager::mousePressEvent(Scene *scene, QGraphicsSceneMouseEvent *event)
{
  if (!method)
    return;
  assert(!path);
  path.reset(new PainterPath{ event->scenePos(), new SceneItem(method, { event->scenePos(), event->scenePos() }) });
  scene->addItem(path->item);
}
void PainterManager::mouseMoveEvent(Scene *scene, QGraphicsSceneMouseEvent *event)
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
void PainterManager::mouseReleaseEvent(Scene *scene, QGraphicsSceneMouseEvent *event)
{
  if (!method)
    return;
  path.reset();
}
