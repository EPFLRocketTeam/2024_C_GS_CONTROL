#include "FieldUtil.h"
#include "MainWindow.h"
#include <Setup.h>
#include "QGuiApplication"
#include <QStyleFactory>
#include <QIcon>
#include <QLabel>
#include <QLayout>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QTransform>
#include <QtSvg/QSvgRenderer>
#include <QMessageBox>
#include <iostream>
#include <unistd.h>

#include "components/BallValveButton.h"

BallValveButton::BallValveButton(GUI_FIELD field, Orientation orientation,
                         QWidget *parent, bool read_only)
    : QLabel(parent), currentState(Unknown), iconSize(52, 52), m_field(field), m_readOnly(read_only) {
  // Set initial state and update button icon
  // Change this to set the initial state as needed
  this->orientation = orientation;
  currentAngle = 0;
  setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  setObjectName("BallValveButton");
  
  if (m_readOnly) {
    setStyleSheet("background: transparent;");
    return;
  }
  setStyleSheet(QString(R"(
            #BallValveButton {
                background:transparent;
                border-radius: 10%;
            }
            #BallValveButton:hover {
                background: rgba(90, 90, 90, 90);
            }
            
        )"));

  updateButtonIcon();

  MainWindow::clientManager->subscribe(field, [this](const QString &message) {
    if (message == "unknown")  {
      setState(BallValveButton::State::Unknown);
    } else {      
      int temp(message.toInt());
      if (temp >= 0 && temp <= 90) {
        setAngle(temp);
        setState(BallValveButton::State::Angle);
      } else {
        setState(BallValveButton::State::Unknown);
      }
    }
  });

  connect(this, &BallValveButton::clicked, [this]() {
    
    if (showConfirmDialog(
        this, "Confirm Valve Angle",
        QString("Are you sure you want to change the angle of the valve '%1'?")
            .arg(fieldUtil::enumToFieldName(m_field))) == QMessageBox::Yes) {
      // Proceed with request
      RequestBuilder b;
      b.setHeader(RequestType::POST);
      int value = getAngle();
      b.addField("cmd", m_field);
      b.addField("cmd_order", value);
      MainWindow::clientManager->send(b.toString());

      // Send "unknown" state internally
      b.clear();
      b.setHeader(RequestType::INTERNAL);
      b.addField(QString::number(m_field), "unknown");
      MainWindow::clientManager->send(b.toString());

      _logger.info(
          "Sent Valve Update",
          QString(
              R"(The valve of field %1 was confirmed and the new %2 value was sent to server)")
              .arg(fieldUtil::enumToFieldName(m_field))
              .arg(value)
              .toStdString());
    } else {
      _logger.debug("BallValveButton", QString("Cancelled action for valve %1")
                                       .arg(fieldUtil::enumToFieldName(m_field))
                                       .toStdString());
    }
  });
  if (m_readOnly) {
        setAttribute(Qt::WA_TransparentForMouseEvents, true);
  }

  setFixedSize(sizeHint());
}

GUI_FIELD BallValveButton::fieldSensivity() { return m_field;}

QMessageBox::StandardButton BallValveButton::showConfirmDialog(QWidget *parent, 
                                              const QString &title, 
                                              const QString &text) {
    angleSel = new ValueSelector(this, currentAngle);
    angleLabel = new QLabel(tr("Angle: "),this);

    connect(angleSel, &ValueSelector::valueChanged, this, &BallValveButton::setAngle);

    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);


    // Apply custom stylesheet
    msgBox.setStyleSheet(QString(R"(
        QMessageBox {
            background-color: #535353;
            color: black;
        }
        QLabel {
            color: white;
            font-size: 14px;
        }
        QPushButton {
            min-width: 80px;
            padding: 6px 12px;
            border-radius: 6px;
            font-weight: bold;
            background-color: #868686;
            border: 1px solid black;
        }
        QPushButton:hover {
            background-color: grey;
        }
    )")
    );    
    QLayout *temp(msgBox.layout());
    if (angleSel) {
      temp->addWidget(angleSel);
    }
    
    // Give object names so stylesheet can target them
    /*QAbstractButton *yesBtn = msgBox.button(QMessageBox::Yes);*/
    /*if (yesBtn) yesBtn->setObjectName("yesButton");*/
    /**/
    /*QAbstractButton *noBtn = msgBox.button(QMessageBox::No);*/
    /*if (noBtn) noBtn->setObjectName("noButton");*/

    return static_cast<QMessageBox::StandardButton>(msgBox.exec());
}

BallValveButton::~BallValveButton() {
  MainWindow::clientManager->unsubscribeAll(m_field);
}

void BallValveButton::setState(State state) {
  currentState = state;
  updateButtonIcon();
}
void BallValveButton::setAngle(int newAngle) {
  currentAngle = newAngle;
}


void BallValveButton::updateButtonIcon() {
  QString iconFilePath;

  switch (currentState) {
  case Angle:
    iconFilePath = ":/images/GS_ball_valve_angle.svg";
    break;
  case Unknown:
    iconFilePath = ":/images/GS_ball_valve_unknown.svg";
    break;
  }

  QIcon icon(iconFilePath);
  QTransform transform;
  QPixmap newPixmap;

  switch (currentState) {
  case Angle:
    if (orientation == Horizontal) {
      newPixmap = icon.pixmap(iconSize).transformed(transform.rotate(90+currentAngle));
      break;
    }
    newPixmap = icon.pixmap(iconSize).transformed(transform.rotate(currentAngle));
    break;
  case Unknown:
    newPixmap = icon.pixmap(iconSize).transformed(transform.rotate(180));
    break;
  }
  setPixmap(newPixmap);

  update();
}

BallValveButton::State BallValveButton::getState() { return currentState; }
int BallValveButton::getAngle() { return currentAngle; }

void BallValveButton::mousePressEvent(QMouseEvent *event) {
  if (m_readOnly) return;

  if (event->button() == Qt::LeftButton)
    emit clicked();

}


  ValueSelector::ValueSelector(QWidget *parent, int value) {
    valueSpinBox = new QSpinBox;
    
    valueSpinBox->setRange(0, 90);
    valueSpinBox->setSingleStep(1);
    valueSpinBox->setValue(value);
    
    valueLabel = new QLabel(tr("Angle:"));

    connect(valueSpinBox,  &QSpinBox::valueChanged,
            this,         &ValueSelector::valueChanged);

    layout = new QBoxLayout(QBoxLayout::LeftToRight);
    layout->addWidget(valueLabel);
    layout->addWidget(valueSpinBox);
    setLayout(layout);
  }

  void ValueSelector::setValue(int value) {
    valueSpinBox->setValue(value);
  }
