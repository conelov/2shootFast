//
// Created by dym on 10.04.2021.
//

#ifndef INC_2SHOOT_SCENE_HPP
#define INC_2SHOOT_SCENE_HPP
#include <QGraphicsScene>
#include <memory>

class UserInputBase;

class Scene: public QGraphicsScene {
  Q_OBJECT

public:
  /// DEFINE / STATIC MEMBER
  class GraphicsItemDeleter {
    QGraphicsScene *scene;

  public:
    GraphicsItemDeleter(QGraphicsScene *sceneIn);
    void operator()(QGraphicsItem *item) const;
  };
  struct DrawingPath
  {
    QPointF begin, end;
    QColor color;

    [[nodiscard]] QJsonObject serialize() const;
    static DrawingPath deserialize(const QJsonObject &json);
  };
  using DrawingFigureMethods= QGraphicsItem *(*)(QGraphicsScene *, QRectF const &, QColor const &);
  static const DrawingFigureMethods drawingFigureMethods[3];


private:
  /// STATIC METHODS
  static QJsonArray serialize(QPointF);
  static QJsonArray serialize(const QPolygonF &);
  static QJsonArray serialize(QRectF);
  static QString serialize(const QColor &);

  static QPointF deserializePointF(QJsonArray const &);
  static QPolygonF deserializePolygonF(QJsonArray const &);
  static QRectF deserializeRectF(QJsonArray const &);
  static QColor deserializeColor(const QString &);

public:
  /// MEMBER
  std::unique_ptr<UserInputBase> inputHandler;

private:
  QPolygonF _borderPolygon;
  std::vector<std::unique_ptr<QGraphicsLineItem, GraphicsItemDeleter>> _borderLines;

public:
  std::vector<std::pair<DrawingPath, QGraphicsItem *>> _figuresUser[3];

  /// METHODS
  ~Scene() override;
  Scene(QPolygonF polygonFIn, QObject *parent= {});
  Scene(QJsonObject const &jsonObject, QObject *parent= {});

  [[nodiscard]] virtual QJsonObject serialize() const;
  virtual void deserialize(QJsonObject const &json);

  [[nodiscard]] bool contains(QPointF const &pointF, Qt ::FillRule fillRule= Qt::OddEvenFill) const
  {
    return _borderPolygon.containsPoint(pointF, fillRule);
  }

private:
  Scene(QObject *parent= {});

  void constructBorderLines();

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // INC_2SHOOT_SCENE_HPP