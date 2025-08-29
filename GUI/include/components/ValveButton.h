#ifndef VALVEBUTTON_H
#define VALVEBUTTON_H

#include "FieldUtil.h"
#include "FileLocation.h"
#include "Log.h"
#include <QLabel>
#include <QPushButton>
#include <qsvgrenderer.h>
#include <QMessageBox>

class ValveButton : public QLabel {
  Q_OBJECT

public:
  enum State { Open = 0, Close, Unknown };

  enum Orientation { Horizontal = 0, Vertical };

  explicit ValveButton(GUI_FIELD field, Orientation orientation = Horizontal,
                       QWidget *parent = nullptr);
  ~ValveButton() override;
  void resetStyle();
  State getState();
  void setState(State state);
  GUI_FIELD fieldSensivity();

signals:
  void clicked();

private:
  State currentState;

  void updateButtonIcon();
protected:
  QSize sizeHint() const override { return QSize(52,52); }
  void mousePressEvent(QMouseEvent *event) override;
  

private:
  Orientation orientation;
  QSize iconSize;
  GUI_FIELD m_field;
  ModuleLog _logger = ModuleLog("ValveButton", LOG_FILE_PATH);
  QMessageBox::StandardButton showConfirmDialog(QWidget *parent, 
                                              const QString &title, 
                                              const QString &text);

  QSvgRenderer *m_rOpen, *m_rClose, *m_rUnknown;
};

#endif // VALVEBUTTON_H
