//
// Created by dym on 10.04.2021.
//

#ifndef INC_2SHOOT_SCENEBASE_HPP
#define INC_2SHOOT_SCENEBASE_HPP
#include <QGraphicsScene>
#include <memory>

class UserInputBase;

class SceneBase: public QGraphicsScene {
  Q_OBJECT

  /// STATIC DEFINE
public:
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

private:
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
  std::unique_ptr<UserInputBase> _inputHandler;
  QPolygonF _borderPolygon;
  std::vector<std::unique_ptr<QGraphicsLineItem, GraphicsItemDeleter>> _borderLines;

  std::vector<std::pair<DrawingPath, QGraphicsItem *>> _figuresUser[3];

  /// METHODS
  ~SceneBase() override;
  SceneBase(QPolygonF polygonFIn, QObject *parent= {});
  SceneBase(QJsonObject const &jsonObject, QObject *parent= {});

  [[nodiscard]] virtual QJsonObject serialize() const;
  virtual void deserialize(QJsonObject const &json);

  [[nodiscard]] bool contains(QPointF const &pointF, Qt ::FillRule fillRule= Qt::OddEvenFill) const
  {
    return _borderPolygon.containsPoint(pointF, fillRule);
  }

private:
  SceneBase(QObject *parent= {});

  void constructBorderLines();

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // INC_2SHOOT_SCENEBASE_HPP