//
// Created by dym on 16.04.2021.
//

#include "Scene.hpp"
#include "utils/InputManager.hpp"
#include <ch_utilities.h>

Scene::~Scene()= default;
Scene::Scene(QObject *parent)
    : QGraphicsScene(parent)
{}
void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  const ch::InvokeOnDestruct exEvent([this, event] { QGraphicsScene::mousePressEvent(event); });
  if (managerWeak.expired())
    return;
  auto const manager= managerWeak.lock();
  manager->mousePressEvent(this, event);
}
void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  const ch::InvokeOnDestruct exEvent([this, event] { QGraphicsScene::mousePressEvent(event); });
  if (managerWeak.expired())
    return;
  auto const manager= managerWeak.lock();
  manager->mouseMoveEvent(this, event);
}
void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  const ch::InvokeOnDestruct exEvent([this, event] { QGraphicsScene::mousePressEvent(event); });
  if (managerWeak.expired())
    return;
  auto const manager= managerWeak.lock();
  manager->mouseReleaseEvent(this, event);
}
