//
// Created by dym on 08.04.2021.
//

#ifndef INC_2SHOOT_FORMMAIN_HPP
#define INC_2SHOOT_FORMMAIN_HPP
#include <QMainWindow>

namespace Ui
{
class FormMain;
}

class FormMain final: public QMainWindow {
  Q_OBJECT
  QScopedPointer<Ui::FormMain> ui;

public:
  ~FormMain() override;
  FormMain(QWidget *parent= {}, Qt::WindowFlags flags= {});
};

#endif // INC_2SHOOT_FORMMAIN_HPP
