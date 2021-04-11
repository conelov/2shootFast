//
// Created by dym on 11.04.2021.
//
#include "UserInputBase.hpp"
#include <SceneBase.hpp>

UserInputBase::CollidingIgnore::CollidingIgnore(QGraphicsScene *sceneIn)
    : scene(sceneIn)
{}
UserInputBase::CollidingIgnore::~CollidingIgnore()
{
  for (auto ptr : items)
    scene->addItem(ptr);
}
UserInputBase::CollidingIgnore &UserInputBase::CollidingIgnore::add(QGraphicsItem *in)
{
  assert(in);
  items.push_back(in);
  scene->removeItem(in);
  return *this;
}
UserInputBase::CollidingIgnore &UserInputBase::CollidingIgnore::remove(QGraphicsItem *in)
{
  assert(in);
  auto const erCount= std::erase(items, in);
  scene->addItem(in);
  assert(erCount == 1);
  return *this;
}
UserInputBase::UserInputBase(SceneBase *scene)
    : scene(scene)
{}
UserInputBase::CollidingIgnore UserInputBase::collidingIgnore()
{
  return UserInputBase::CollidingIgnore(scene);
}
