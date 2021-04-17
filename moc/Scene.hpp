//
// Created by dym on 16.04.2021.
//

#ifndef INC_2SHOOT_SCENE_HPP
#define INC_2SHOOT_SCENE_HPP
#include <QGraphicsScene>
#include <memory>
class InputHandlerBase;
class SceneItem;

class Scene: public QGraphicsScene {
  Q_OBJECT

  QPolygon borderPolygon;
  std::vector<QGraphicsLineItem *> borderLine;

public:
  std::vector<SceneItem *> userItems;
  std::weak_ptr<InputHandlerBase> handlerWeak{};

  static QPolygon defaultBorders();

  ~Scene() override;
  Scene();
  Scene(QPolygon borders, QObject *parent= {});

  friend QDataStream &operator<<(QDataStream &stream, Scene const &scene);
  friend QDataStream &operator>>(QDataStream &stream, Scene &scene);

private:
  void constructBorderLine();

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};
#endif // INC_2SHOOT_SCENE_HPP
