//
// Created by dym on 11.04.2021.
//

#ifndef INC_2SHOOT_FIGURESELECTORADAPTER_HPP
#define INC_2SHOOT_FIGURESELECTORADAPTER_HPP
#include <QMetaType>

class FigureSelectorAdapter {
public:
  enum FigureType : quint8 
  {
    none,
    shape,
    player
  } type= none;
  using FigureTypeEnumUnderlying= std::underlying_type_t<decltype(type)>;

  qint16 i = -1;

  operator bool() const
  {
    return type != none;
  }
};
Q_DECLARE_METATYPE(FigureSelectorAdapter)

QDataStream &operator<<(QDataStream &stream, FigureSelectorAdapter adapter);
QDataStream &operator>>(QDataStream &stream, FigureSelectorAdapter &adapter);

#endif // INC_2SHOOT_FIGURESELECTORADAPTER_HPP
