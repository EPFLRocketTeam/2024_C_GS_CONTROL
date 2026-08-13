#ifndef BALLVALVEBUTTON_H
#define BALLVALVEBUTTON_H

#include "FieldUtil.h"
#include "FileLocation.h"
#include "Log.h"
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <qsvgrenderer.h>
#include <QMessageBox>
#include <qtmetamacros.h>

class ValueSelector : public QWidget {
  Q_OBJECT

  public:
    ValueSelector(QWidget *parent = nullptr, int value = 0);

  signals:
    void valueChanged(int value);

  public slots:
    void setValue(int value);
  
  private:
    QSlider *valueSlider;
    QSpinBox *valueSpinBox;
    QLabel *valueLabel;
    QBoxLayout *layout;
};

class BallValveButton : public QLabel {
  Q_OBJECT

public:
  enum State { Angle = 0, Unknown };

  enum Orientation { Horizontal = 0, Vertical };

  explicit BallValveButton(GUI_FIELD field, Orientation orientation = Horizontal,
                       QWidget *parent = nullptr, bool read_only = true);
  ~BallValveButton() override;

  State getState();
  void setState(State newState);

  int getAngle();
  GUI_FIELD fieldSensivity();

  
signals:
  void clicked();
  
public slots:
  void setAngle(int newAngle);


private:
  State currentState;
  int currentAngle;

  void updateButtonIcon();
  
protected:
  QSize sizeHint() const override { return QSize(52,52); }
  void mousePressEvent(QMouseEvent *event) override;
  

private:
  Orientation orientation;
  QSize iconSize;
  GUI_FIELD m_field;
  bool m_readOnly = false;
  ModuleLog _logger = ModuleLog("ValveButton", LOG_FILE_PATH);
  QMessageBox::StandardButton showConfirmDialog(QWidget *parent, 
                                              const QString &title, 
                                              const QString &text);
  ValueSelector *angleSel;
  QLabel *angleLabel;
};


#endif // VALVEBUTTON_H
