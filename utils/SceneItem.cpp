//
// Created by dym on 15.04.2021.
//
#include "Paint.hpp"
#include "SceneItem.hpp"

SceneItem::~SceneItem()=default;
SceneItem::SceneItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{}
