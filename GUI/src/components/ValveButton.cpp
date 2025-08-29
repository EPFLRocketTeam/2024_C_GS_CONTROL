#include "FieldUtil.h"
#include "MainWindow.h"
#include <Setup.h>
#include "QGuiApplication"
#include <QStyleFactory>
#include <QIcon>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QTransform>
#include <QtSvg/QSvgRenderer>
#include <QMessageBox>
#include <iostream>
#include <unistd.h>

#include "components/ValveButton.h"

ValveButton::ValveButton(GUI_FIELD field, Orientation orientation,
                         QWidget *parent)
    : QLabel(parent), currentState(Unknown), iconSize(52, 52), m_field(field) {
  // Set initial state and update button icon
  // Change this to set the initial state as needed
  this->orientation = orientation;
  setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  setObjectName("valveButton");
  resetStyle();

  m_rOpen =
      new QSvgRenderer(QStringLiteral(":/images/GS-valve-open.svg"), this);
  m_rClose =
      new QSvgRenderer(QStringLiteral(":/images/GS-valve-close.svg"), this);
  m_rUnknown =
      new QSvgRenderer(QStringLiteral(":/images/GS-valve-unknown.svg"), this);
  updateButtonIcon();

  MainWindow::clientManager->subscribe(field, [this](const QString &message) {
    if (message == "0") {
      setState(ValveButton::State::Close);
    } else if (message == "unknown") {
      setState(ValveButton::State::Unknown);
    } else {
      setState(ValveButton::State::Open);
    }
  });

  connect(this, &ValveButton::clicked, [this]() {
    
    if (showConfirmDialog(
        this, "Confirm Valve Action",
        QString("Are you sure you want to toggle the valve '%1'?")
            .arg(fieldUtil::enumToFieldName(m_field))) == QMessageBox::Yes) {
      // Proceed with request
      RequestBuilder b;
      b.setHeader(RequestType::POST);
      int value = getState() == ValveButton::State::Close ? 1 : 0;
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
      _logger.debug("ValveButton", QString("Cancelled action for valve %1")
                                       .arg(fieldUtil::enumToFieldName(m_field))
                                       .toStdString());
    }
  });

  setFixedSize(sizeHint());
}

GUI_FIELD ValveButton::fieldSensivity() { return m_field;}

QMessageBox::StandardButton ValveButton::showConfirmDialog(QWidget *parent, 
                                              const QString &title, 
                                              const QString &text) {
    QMessageBox msgBox(parent);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    // Force Fusion style (consistent across platforms)
    msgBox.setStyle(QStyleFactory::create("Fusion"));

    // Apply custom stylesheet
    msgBox.setStyleSheet(QString(R"(
        QMessageBox {
            background-color: white;
            color: black;
        }
        QLabel {
            color: black;
            font-size: 14px;
        }
        QPushButton {
            min-width: 80px;
            padding: 6px 12px;
            border-radius: 6px;
            font-weight: bold;
            background-color: #eeeeee;
            border: 1px solid black;
        }
        QPushButton:hover {
            background-color: grey;
        }
    )")
    );

    // Give object names so stylesheet can target them
    /*QAbstractButton *yesBtn = msgBox.button(QMessageBox::Yes);*/
    /*if (yesBtn) yesBtn->setObjectName("yesButton");*/
    /**/
    /*QAbstractButton *noBtn = msgBox.button(QMessageBox::No);*/
    /*if (noBtn) noBtn->setObjectName("noButton");*/

    return static_cast<QMessageBox::StandardButton>(msgBox.exec());
}

ValveButton::~ValveButton() {
  MainWindow::clientManager->unsubscribeAll(m_field);
}

void ValveButton::setState(State state) {
  currentState = state;
  updateButtonIcon();
}

void ValveButton::updateButtonIcon() {
  QString iconFilePath;
  switch (currentState) {
  case Open:
    iconFilePath = ":/images/GS-valve-open.svg";
    break;
  case Close:
    iconFilePath = ":/images/GS-valve-close.svg";
    break;
  case Unknown:
    iconFilePath = ":/images/GS-valve-unknown.svg";
    break;
  }

  QIcon icon(iconFilePath);
  QTransform transform;
  QPixmap newPixmap;
  switch (currentState) {
  case Open:
    if (orientation == Horizontal) {
      newPixmap = icon.pixmap(iconSize).transformed(transform.rotate(90));
      break;
    }
    newPixmap = icon.pixmap(iconSize).transformed(transform.rotate(0));
    break;
  case Close:
    if (orientation == Horizontal) {
      newPixmap = icon.pixmap(iconSize).transformed(transform.rotate(0));
      break;
    }
    newPixmap = icon.pixmap(iconSize).transformed(transform.rotate(90));
    break;
  case Unknown:
    newPixmap = icon.pixmap(iconSize).transformed(transform.rotate(180));
    break;
  }
  setPixmap(newPixmap);

  update();
}

ValveButton::State ValveButton::getState() { return currentState; }

void ValveButton::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton)
    emit clicked();

  setStyleSheet(QString(R"(
            #valveButton {
                background:transparent;
                border-radius: 10%;
            }
            #valveButton:hover {
                background: rgba(90, 90, 90, 130);
            }
            
        )"));

  QTimer::singleShot(100, [this]() { this->resetStyle(); });
}

void ValveButton::resetStyle() {
  setStyleSheet(QString(R"(
            #valveButton {
                background:transparent;
                border-radius: 10%;
            }
            #valveButton:hover {
                background: rgba(90, 90, 90, 90);
            }
            
        )"));
}
