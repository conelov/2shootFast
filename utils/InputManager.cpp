//
// Created by dym on 15.04.2021.
//
#include "InputManager.hpp"
#include "Paint.hpp"
#include <QColor>

InputManagerBase::~InputManagerBase()= default;

PainterManager::~PainterManager()= default;

PainterManager::PainterManager()
    : color(new QColor)
    ,selector(new draw::Selector)
{}
void PainterManager::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  assert(false);
}
void PainterManager::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  assert(false);
}
void PainterManager::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  assert(false);
}
