//
// Created by dym on 12.04.2021.
//
#include "FigureSelectorAdapter.hpp"
#include <QDataStream>

QDataStream &operator<<(QDataStream &stream, FigureSelectorAdapter adapter)
{
  stream << adapter.i << static_cast<FigureSelectorAdapter::FigureTypeEnumUnderlying>(adapter.type);
  return stream;
}
QDataStream &operator>>(QDataStream &stream, FigureSelectorAdapter &adapter)
{
  FigureSelectorAdapter::FigureTypeEnumUnderlying enumType;
  stream >> adapter.i >> enumType;
  adapter.type= static_cast<decltype(adapter.type)>(enumType);
  return stream;
}