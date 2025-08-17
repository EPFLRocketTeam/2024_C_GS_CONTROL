#include "QGuiApplication"
#include <QIcon>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QTimer>
#include <QTransform>
#include <QtSvg/QSvgRenderer>
#include <iostream>
#include <unistd.h>

#include "components/ValveButton.h"

ValveButton::ValveButton(Orientation orientation, QWidget *parent)
    : QLabel(parent), currentState(Unknown), iconSize(52, 52) {
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

  setFixedSize(sizeHint());
}

void ValveButton::setState(State state) {
  currentState = state;
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

void ValveButton::paintEvent(QPaintEvent *) {
  QPainter p(this);
  // 2) Guard painter activation

  p.setRenderHint(QPainter::Antialiasing);

  // pick the right renderer
  QSvgRenderer *r = nullptr;
  switch (currentState) {
  case Open:
    r = m_rOpen;
    break;
  case Close:
    r = m_rClose;
    break;
  case Unknown:
    r = m_rUnknown;
    break;
  }

  // center + rotate if needed
  p.save();
  if (orientation == Horizontal) {
    // rotate 90° about widget center
    p.translate(width() / 2.0, height() / 2.0);
    p.rotate(90);
    p.translate(-height() / 2.0, -width() / 2.0);
    // note: swapped width/height because of the rotate
    r->render(&p, QRectF(0, 0, height(), width()));
  } else {
    // no rotation
    r->render(&p, QRectF(0, 0, width(), height()));
  }
  p.restore();
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
