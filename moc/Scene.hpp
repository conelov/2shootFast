//
// Created by dym on 16.04.2021.
//

#ifndef INC_2SHOOT_SCENE_HPP
#define INC_2SHOOT_SCENE_HPP
#include <QGraphicsScene>
#include <memory>
class InputManagerBase;
class SceneItem;

class Scene : public QGraphicsScene{
  Q_OBJECT

  std::vector<SceneItem *> userItems;
public:
  std::weak_ptr<InputManagerBase> managerWeak{};

  ~Scene() override;
  Scene(QObject *parent = {});

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};
#endif // INC_2SHOOT_SCENE_HPP
