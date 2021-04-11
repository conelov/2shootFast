//
// Created by dym on 10.04.2021.
//

#ifndef INC_2SHOOT_SCENEBASE_HPP
#define INC_2SHOOT_SCENEBASE_HPP
#include <QGraphicsScene>
#include <memory>

class SceneBase: public QGraphicsScene {
  Q_OBJECT

  /// STATIC DEFINE
protected:
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
  class CollidingIgnore{
    QGraphicsScene *scene;
    std::vector<QGraphicsItem *> items;

  public:
    CollidingIgnore()                       = delete;
    CollidingIgnore(CollidingIgnore const &)= delete;
    CollidingIgnore &operator=(CollidingIgnore const &)= delete;
    CollidingIgnore(CollidingIgnore &&)                = default;
    CollidingIgnore &operator=(CollidingIgnore &&)= default;

    CollidingIgnore(QGraphicsScene *sceneIn);
    ~CollidingIgnore();
    CollidingIgnore & add(QGraphicsItem *in);
    CollidingIgnore & remove(QGraphicsItem *in);
  };

  static QJsonArray serialize(QPointF);
  static QJsonArray serialize(const QPolygonF &);
  static QJsonArray serialize(QRectF);
  static QString serialize(const QColor &);

  static QPointF deserializePointF(QJsonArray const &);
  static QPolygonF deserializePolygonF(QJsonArray const &);
  static QRectF deserializeRectF(QJsonArray const &);
  static QColor deserializeColor(const QString &);

  /// MEMBER
private:
  QPolygonF _borderPolygon;
  std::vector<std::unique_ptr<QGraphicsLineItem, GraphicsItemDeleter>> _borderLines;

protected:
  std::vector<std::pair<DrawingPath, QGraphicsItem *>> _figuresUser[3];

  /// METHODS
public:
  ~SceneBase() override;
  SceneBase(QPolygonF polygonFIn, QObject *parent= {});
  SceneBase(QJsonObject const &jsonObject, QObject *parent= {});

  [[nodiscard]] virtual QJsonObject serialize() const;
  virtual void deserialize(QJsonObject const &json);

protected:
  [[nodiscard]] bool contains(QPointF const &pointF, Qt ::FillRule fillRule= Qt::OddEvenFill) const
  {
    return _borderPolygon.containsPoint(pointF, fillRule);
  }

  virtual CollidingIgnore collidingIgnore();

private:
  SceneBase(QObject *parent= {});

  void constructBorderLines();
};

#endif // INC_2SHOOT_SCENEBASE_HPP