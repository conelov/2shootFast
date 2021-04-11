//
// Created by dym on 08.04.2021.
//

#ifndef INC_2SHOOT_SCENECREATOR_HPP
#define INC_2SHOOT_SCENECREATOR_HPP
#include "SceneBase.hpp"

class QGraphicsLineItem;

class SceneCreator: public SceneBase {
  Q_OBJECT

  /// DEFINE
  class DrawingProcess;
  using DrawingFigureMethods= QGraphicsItem *(*)(QGraphicsScene *, QRectF, QColor const &);

  /// MEMBER
public:
  QColor colorFigure{};

private:
  int figureSelector= -1;
  QScopedPointer<DrawingProcess> drawingProcess;
  static const DrawingFigureMethods drawingFigureMethods[3];

  /// METHODS
public:
  ~SceneCreator() override;
  SceneCreator(QPolygonF polygonFIn, QObject *parent= {});
  SceneCreator(QJsonObject const &serial, QObject *parent= {});

  Q_SLOT void setDrawingFigureId(int id)
  {
    figureSelector= id;
  }
  void setColorFigure(QColor const &color)
  {
    colorFigure= color;
  }

  [[nodiscard]] QJsonObject serialize() const override;

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // INC_2SHOOT_SCENECREATOR_HPP
