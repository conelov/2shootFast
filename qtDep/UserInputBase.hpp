//
// Created by dym on 11.04.2021.
//

#ifndef INC_2SHOOT_USERINPUTBASE_HPP
#define INC_2SHOOT_USERINPUTBASE_HPP
#include <vector>

class QGraphicsSceneMouseEvent;
class Scene;
class QGraphicsScene;
class QGraphicsItem;

class UserInputBase {
  /// DEFINE
protected:
  class CollidingIgnore{
    QGraphicsScene *scene;
    std::vector<QGraphicsItem *> items;

  public:
    CollidingIgnore()                       = delete;
    CollidingIgnore(CollidingIgnore const &)= delete;
    CollidingIgnore &operator=(CollidingIgnore const &)= delete;
    CollidingIgnore(CollidingIgnore &&)                = default;
    CollidingIgnore &operator=(CollidingIgnore &&)= default;

    ~CollidingIgnore();
    CollidingIgnore(QGraphicsScene *);
    CollidingIgnore & add(QGraphicsItem *);
    CollidingIgnore & remove(QGraphicsItem *);
  };

  /// MEMBERS
protected:
  Scene *const scene{};

  /// METHODS
public:
  virtual ~UserInputBase() = default;
  UserInputBase() = delete;
  UserInputBase(Scene *scene);
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event)= 0;
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) =0;
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) =0;
  virtual CollidingIgnore collidingIgnore();
};
#endif // INC_2SHOOT_USERINPUTBASE_HPP
