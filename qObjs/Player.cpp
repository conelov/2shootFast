//
// Created by dym on 11.04.2021.
//

#include "Player.hpp"
#include <QPainter>

Player::~Player()= default;
Player::Player(QObject *parent, QGraphicsItem *parent1)
    : QObject(parent)
    , QGraphicsItem(parent1)
{}
QRectF Player::boundingRect() const
{
  return QRectF(-20, -30, 40, 60);
}
void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->setBrush(brush);
  painter->setPen(pen);
  painter->drawPolygon(polygon);

  Q_UNUSED(option)
  Q_UNUSED(widget)
}
void Player::setMouseTarget(QPointF const mousePos)
{
  assert(false);
}
