//
// Created by dym on 10.04.2021.
//

#ifndef INC_2SHOOT_SCENEBASE_HPP
#define INC_2SHOOT_SCENEBASE_HPP
#include <QGraphicsScene>
#include <memory>

class SceneBase: public QGraphicsScene {
  Q_OBJECT

protected:
  class GraphicsItemDeleter {
    QGraphicsScene *scene;

  public:
    GraphicsItemDeleter(QGraphicsScene *sceneIn);
    void operator()(QGraphicsItem *item) const;
  };

  static QJsonArray serialize(QPointF);
  static QJsonArray serialize(const QPolygonF &);
  static QJsonArray serialize(QRectF);
  static QString serialize(QColor);

  static QPointF deserializePointF(QJsonArray const &);
  static QPolygonF deserializePolygonF(QJsonArray const &);
  static QRectF deserializeRectF(QJsonArray const &);
  static QColor deserializeColor(const QString &);

private:
  QPolygonF _borderPolygon;
  std::vector<std::unique_ptr<QGraphicsLineItem, GraphicsItemDeleter>> _borderLines;

protected:
  [[nodiscard]] bool contains(QPointF const &pointF, Qt ::FillRule fillRule= Qt::OddEvenFill) const
  {
    return _borderPolygon.containsPoint(pointF, fillRule);
  }

  [[nodiscard]] QPolygonF const &borderPolygon() const
  {
    return _borderPolygon;
  }

  [[nodiscard]] std::remove_cvref_t<decltype(_borderLines)> const &borderLines() const
  {
    return _borderLines;
  }

public:
  ~SceneBase() override;
  SceneBase(QPolygonF polygonFIn, QObject *parent= {});
  SceneBase(QJsonObject const & jsonObject, QObject *parent= {});

  [[nodiscard]] virtual QJsonObject serialize() const;
};

#endif // INC_2SHOOT_SCENEBASE_HPP