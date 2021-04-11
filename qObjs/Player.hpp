//
// Created by dym on 11.04.2021.
//

#ifndef INC_2SHOOT_PLAYER_HPP
#define INC_2SHOOT_PLAYER_HPP
#include <QBrush>
#include <QGraphicsItem>
#include <QObject>
#include <QPen>

class Player
    : public QObject
    , public QGraphicsItem {
  Q_OBJECT
  Q_INTERFACES(QGraphicsItem)

  /// MEMBER
  QPointF _mouseTarget;

public:
  QPen pen        = QPen(Qt::GlobalColor::black);
  QBrush brush    = QBrush(Qt::GlobalColor::red);
  QPolygon polygon= QPolygon() << QPoint(0, -30) << QPoint(20, 30) << QPoint(-20, 30);

  /// METHOD
  ~Player() override;
  Player(QObject *parent= {}, QGraphicsItem *parent1= {});

  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
  [[nodiscard]] QRectF boundingRect() const override;

  Q_SLOT void setMouseTarget(QPointF);
};

#endif // INC_2SHOOT_PLAYER_HPP
