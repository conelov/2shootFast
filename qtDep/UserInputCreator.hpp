//
// Created by dym on 11.04.2021.
//

#ifndef INC_2SHOOT_USERINPUTCREATOR_HPP
#define INC_2SHOOT_USERINPUTCREATOR_HPP
#include "UserInputBase.hpp"
#include <memory>

class QGraphicsItem;
class QGraphicsScene;
class QRectF;
class QColor;

class UserInputCreator : public UserInputBase{

  /// DEFINE
  class DrawingProcess;

  /// MEMBERS
public:
  QColor *colorPainting{};
  int *figureSelector{};

private:
  std::unique_ptr<DrawingProcess> drawingProcess;

  /// METHODS
public:
  ~UserInputCreator() override;
  UserInputCreator(Scene *scene);
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // INC_2SHOOT_USERINPUTCREATOR_HPP
