//
// Created by dym on 15.04.2021.
//

#ifndef INC_2SHOOT_FORMMAIN_HPP
#define INC_2SHOOT_FORMMAIN_HPP
#include <QMainWindow>
#include <QSettings>
#include <memory>

class QGraphicsScene;

namespace Ui
{
class FormMain;
}
namespace draw
{
class Methods;
}
class FormMain final: public QMainWindow {
  Q_OBJECT

  const QScopedPointer<Ui::FormMain> ui;
  std::vector<std::unique_ptr<QGraphicsScene>> sceneActive;
public:
  const QScopedPointer<const draw::Methods> methods;

  static QSettings getGlobalQSetting();

  ~FormMain() override;
  FormMain(QWidget *parent= {}, Qt::WindowFlags flags= {});


};

#endif // INC_2SHOOT_FORMMAIN_HPP
