//
// Created by dym on 09.04.2021.
//

#ifndef INC_2SHOOT_FIGURESPANEL_HPP
#define INC_2SHOOT_FIGURESPANEL_HPP
#include <QWidget>

namespace Ui
{
class FiguresPanel;
}
class QPushButton;

class FiguresPanel final: public QWidget {
  Q_OBJECT
  QScopedPointer<Ui::FiguresPanel> ui;
  QPushButton *buttonFigureCurrentPressed{};

public:
  ~FiguresPanel() override;
  FiguresPanel(QWidget *parent= {}, Qt::WindowFlags f= {});
  FiguresPanel(QColor const &colorIcon= {}, QWidget *parent= {}, Qt::WindowFlags f= {});

  [[nodiscard]] int buttonFigurePressedId() const;
  Q_SIGNAL void figureChange(int);
  void setColorFigure(QColor const &color);

private:
  Q_SLOT void guardMultiCheckButton();
};

#endif // INC_2SHOOT_FIGURESPANEL_HPP
