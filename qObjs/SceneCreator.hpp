//
// Created by dym on 08.04.2021.
//

#ifndef INC_2SHOOT_SCENECREATOR_HPP
#define INC_2SHOOT_SCENECREATOR_HPP
#include "SceneBase.hpp"

class QGraphicsLineItem;

class SceneCreator: public SceneBase {
  Q_OBJECT

  struct DrawingPath{
    QPointF begin, end;
    QColor color;

    [[nodiscard]] QJsonObject serialize() const;
    static DrawingPath deserialize(const QJsonObject &json);
  };
  class DrawingProcess;
  class CollidingIgnore;
public:
  QColor colorFigure{};

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

private:
  int figureSelector= -1;
  std::unique_ptr<DrawingProcess> drawingProcess;
  std::array<std::function<QGraphicsItem *(const QRectF &rect)>, 3> drawingFigureMethods;
  decltype(drawingFigureMethods) drawingFigureMethodsDefault();

  std::vector<std::pair<DrawingPath, QGraphicsItem *>> _figuresUser[3];

  template<typename... Args>
  CollidingIgnore collidingIgnore(Args &&...args);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

public:
  [[nodiscard]] QJsonObject serialize() const override;
};

#endif // INC_2SHOOT_SCENECREATOR_HPP
