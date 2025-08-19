#ifndef VALVEBUTTON_H
#define VALVEBUTTON_H

#include <QLabel>
#include <QPushButton>
#include <qsvgrenderer.h>

class ValveButton : public QLabel {
  Q_OBJECT

public:
  enum State { Open = 0, Close, Unknown };

  enum Orientation { Horizontal = 0, Vertical };

  explicit ValveButton(Orientation orientation = Horizontal,
                       QWidget *parent = nullptr);
  void resetStyle();
  State getState();
  void setState(State state);

signals:
  void clicked();

private:
  State currentState;

  void updateButtonIcon();
protected:
  // void paintEvent(QPaintEvent *) override;
  QSize sizeHint() const override { return QSize(52,52); }
  void mousePressEvent(QMouseEvent *event) override;

private:
  Orientation orientation;
  QSize iconSize;

  QSvgRenderer *m_rOpen, *m_rClose, *m_rUnknown;
};

#endif // VALVEBUTTON_H
