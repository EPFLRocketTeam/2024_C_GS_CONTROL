#include <memory>

#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <qthread.h>

#include "../Setup.h"
#include "GSManagerView.h"
#include "MainWindow.h"
#include "SerialView.h"
#include "TimerView.h"

GSManagerView::GSManagerView(std::unique_ptr<QWidget> parent)
    : QFrame(parent.get()) {
  // Set up the appearance or behavior as needed

  // setStyleSheet("background-color: lightblue;");
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  setMinimumWidth(mws::sideWidth / 100.0 * mws::width);
  setupUI();

  _logger.debug("SetupUI", "Setup finished");
}

void GSManagerView::setupUI() {
  // QPushButton  *reseButton = new QPushButton();
  std::unique_ptr<QLabel> label_timers =
      std::make_unique<QLabel>("Timers section");

  layout = new QVBoxLayout(this);

  setupTimersSection();
  setupInfoSection();
  setupSerialSection();

  QWidget *wrapper = new QWidget;
  QHBoxLayout *wrapperLayout = new QHBoxLayout(wrapper);
  wrapperLayout->setContentsMargins(0, 0, 0, 0);
  wrapperLayout->addWidget(timersSection, 1);
  wrapperLayout->addWidget(infoSection, 1);
  /*wrapperLayout->setSpacing(15);*/

  layout->addWidget(wrapper, 1);
  layout->addWidget(serialSection, 2);
  layout->setContentsMargins(0, 0, 0, 0);

  setupConnections();
}

void GSManagerView::setupSerialSection() { serialSection = new SerialView; }

void GSManagerView::setupInfoSection() {

  infoSection = new QWidget;
  infoSection->setObjectName("infoSection");
  infoSection->setStyleSheet(col::defaultCardStyle("infoSection"));

  QWidget *line = new QWidget;
  line->setFixedHeight(1);

  line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  line->setStyleSheet(QString("background-color: rgba(130, 130, 130, 100);"));

  downRange = new QLabel("--");
  setInfoChildrenStyle(downRange, 27, false);

  altitude = new QLabel("--");
  setInfoChildrenStyle(altitude, 27, false);

  QLabel *downRangeLabel = new QLabel("Downrange");
  setInfoChildrenStyle(downRangeLabel, 15, true);

  QLabel *altitudeLabel = new QLabel("Altitude");
  setInfoChildrenStyle(altitudeLabel, 15, true);

  setupInfoLayout(altitudeLabel, downRangeLabel, line);
}

void GSManagerView::setupInfoLayout(QLabel *downRangeLabel,
                                    QLabel *altitudeLabel, QWidget *line) {
  QVBoxLayout *infoLayout = new QVBoxLayout(infoSection);
  infoLayout->addWidget(altitudeLabel, 1);
  infoLayout->addWidget(downRange, 2);
  infoLayout->addStretch(1);
  infoLayout->addWidget(line);
  infoLayout->addWidget(downRangeLabel, 1);
  infoLayout->addWidget(altitude, 2);
  infoLayout->addStretch(1);

  QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect;
  shadowEffect->setColor(QColor(0, 0, 0, 255 * 0.3));
  shadowEffect->setOffset(0);
  shadowEffect->setBlurRadius(40);
  infoSection->setGraphicsEffect(shadowEffect);
}

void GSManagerView::setInfoChildrenStyle(QFrame *child, int fontSize,
                                         bool isBold) {
  child->setObjectName("child");
  child->setStyleSheet(childrenStyle);
  QFont font = child->font();
  font.setPointSize(fontSize);
  font.setBold(isBold);
  child->setFont(font);
}

void GSManagerView::setupTimersSection() {

  timerViewAV = new TimerView("AV", GUI_FIELD::AV_TIMER);
  timerViewGSE = new TimerView("GSE", GUI_FIELD::GSE_TIMER);
  // layout->addWidget(label_timers.get(), 1);
  QWidget *internalStucture = new QWidget();
  QHBoxLayout *innerLayout = new QHBoxLayout(internalStucture);
  internalStucture->setMinimumWidth(100);
  // Set size policy to expand horizontally
  internalStucture->setSizePolicy(QSizePolicy::Expanding,
                                  QSizePolicy::Expanding);
  innerLayout->setSpacing(0);
  innerLayout->addWidget(timerViewAV, 1);
  innerLayout->addWidget(timerViewGSE, 1);
  // For the horizontal layout containing the timers:
  innerLayout->setContentsMargins(5, 0, 5, 0);
  innerLayout->setSpacing(0);
  internalStucture->setObjectName("child");

  QLabel *title = new QLabel("Time Since Pckt.");
  title->setObjectName("child");

  timersSection = new QWidget;
  timersSection->setObjectName("timerSection");
  timersSection->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  timersSection->setStyleSheet(col::defaultCardStyle("timerSection"));
  // timersSection->setStyleSheet("background:yellow;");

  QVBoxLayout *timersLayout = new QVBoxLayout(timersSection);
  timersLayout->setContentsMargins(0, 0, 0, 0);
  timersLayout->setSpacing(0);
  timersLayout->addWidget(title, 1, Qt::AlignHCenter);
  timersLayout->addWidget(internalStucture, 3, Qt::AlignHCenter);
}

void GSManagerView::setupConnections() {
  MainWindow::clientManager->subscribe(GUI_FIELD::DOWNRANGE, setDownRange);
  MainWindow::clientManager->subscribe(GUI_FIELD::GNSS_ALT, setAltitude);
}
