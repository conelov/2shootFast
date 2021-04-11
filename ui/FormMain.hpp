//
// Created by dym on 08.04.2021.
//

#ifndef INC_2SHOOT_FORMMAIN_HPP
#define INC_2SHOOT_FORMMAIN_HPP
#include <QMainWindow>
#include <QSettings>

namespace Ui
{
class FormMain;
}

class FormMain final: public QMainWindow {
  Q_OBJECT

  /// MEMBER
  QScopedPointer<Ui::FormMain> ui;

public:
  /// METHODS
  static QSettings getGlobalQSetting();
  ~FormMain() override;
  FormMain(QWidget *parent= {}, Qt::WindowFlags flags= {});
};

#endif // INC_2SHOOT_FORMMAIN_HPP
