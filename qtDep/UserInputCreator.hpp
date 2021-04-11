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
  using DrawingFigureMethods= QGraphicsItem *(*)(QGraphicsScene *, QRectF const &, QColor const &);
  class DrawingProcess;

  /// MEMBERS
public:
  QColor *colorPainting{};
  int *figureSelector{};

private:
  std::unique_ptr<DrawingProcess> drawingProcess;
  static const DrawingFigureMethods drawingFigureMethods[3];

  /// METHODS
public:
  ~UserInputCreator() override;
  UserInputCreator(SceneBase *scene);
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // INC_2SHOOT_USERINPUTCREATOR_HPP
