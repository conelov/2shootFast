//
// Created by dym on 16.04.2021.
//

#include "Scene.hpp"
#include "utils/Define.hpp"
#include "utils/InputHandler.hpp"
#include "utils/SceneItem.hpp"

#include <QPolygon>

#include <QDebug>

QPolygon Scene::defaultBorders()
{
  constexpr auto mod= 260;
  return QPolygon() << QPoint{ -mod, -mod } << QPoint{ mod, -mod } << QPoint{ static_cast<int>(mod * 1.35), 0 }
                    << QPoint{ mod, mod } << QPoint{ -mod, mod } << QPoint{ static_cast<int>(-mod * 1.35), 0 }
                    << QPoint{ -mod, -mod };
}
Scene::~Scene()= default;
Scene::Scene() = default;
Scene::Scene(QPolygon borders, QObject *parent)
    : QGraphicsScene(parent)
    , borderPolygon(std::move(borders))
{
  constructBorderLine();
}
void Scene::constructBorderLine()
{
  assert(borderPolygon.size() > 2 && borderPolygon.front() == borderPolygon.back());
  setSceneRect(borderPolygon.boundingRect());
  QPen pen(Qt::GlobalColor::gray);
  pen.setWidth(2);
  for (auto prev= borderPolygon.cbegin(), curr= std::next(prev); curr != borderPolygon.cend();
       prev= std::exchange(curr, std::next(curr))) {
    borderLine.push_back(addLine(QLine(*prev, *curr), pen));
  }
}
void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  const InvokeOnDestruct exEvent([this, event] { QGraphicsScene::mousePressEvent(event); });
  if (handlerWeak.expired())
    return;
  auto const manager= handlerWeak.lock();
  manager->mousePressEvent(this, event);
}
void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  const InvokeOnDestruct exEvent([this, event] { QGraphicsScene::mousePressEvent(event); });
  if (handlerWeak.expired())
    return;
  auto const manager= handlerWeak.lock();
  manager->mouseMoveEvent(this, event);
}
void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  const InvokeOnDestruct exEvent([this, event] { QGraphicsScene::mousePressEvent(event); });
  if (handlerWeak.expired())
    return;
  auto const manager= handlerWeak.lock();
  manager->mouseReleaseEvent(this, event);
}

QDataStream &operator<<(QDataStream &stream, Scene const &scene)
{
  assert(!scene.borderPolygon.isEmpty());
  stream << scene.borderPolygon << static_cast<qsizetype>(scene.userItems.size());
  for (auto const item : scene.userItems) {
    stream << item;
  }
  return stream;
}
QDataStream &operator>>(QDataStream &stream, Scene &scene)
{
  assert(scene.borderPolygon.isEmpty());
  qsizetype itemsCount{};
  stream >> scene.borderPolygon >> itemsCount;
  for (qsizetype i{}; i < itemsCount; ++i) {
    scene.userItems.push_back({});
    stream >> scene.userItems.back();
    scene.addItem(scene.userItems.back());
  }

  scene.constructBorderLine();
  return stream;
}