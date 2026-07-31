#ifndef BALLVALVEBUTTON_H
#define BALLVALVEBUTTON_H

#include "FieldUtil.h"
#include "FileLocation.h"
#include "Log.h"
#include <QLabel>
#include <QPushButton>
#include <qsvgrenderer.h>
#include <QMessageBox>
#include <qtmetamacros.h>

class BallValveButton : public QLabel {
  Q_OBJECT

public:
  typedef uint8_t angle;
  enum State { Angle = 0, Unknown };

  enum Orientation { Horizontal = 0, Vertical };

  explicit BallValveButton(GUI_FIELD field, Orientation orientation = Horizontal,
                       QWidget *parent = nullptr, bool read_only = true);
  ~BallValveButton() override;

  State getState();
  void setState(State newState);

  angle getAngle();
  void setAngle(angle newAngle);


  GUI_FIELD fieldSensivity();

signals:
  void clicked();

private:
  State currentState;
  angle currentAngle;

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

  QSvgRenderer *m_rAngle, *m_rUnknown;
};

#endif // VALVEBUTTON_H
