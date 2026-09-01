/**
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief Main window of the application
*/

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QVBoxLayout>
#include <qabstractscrollarea.h>
#include <qwidget.h>

#include "ControlPanelView.h"
#include "Log.h"
#include "MainWindow.h"
#include "Setup.h"

MainWindow::MainWindow(
    QWidget *parent,
    QMap<std::string, QList<std::vector<GUI_FIELD>>> *controlPanelMap,
    QMap<std::string, QList<std::vector<GUI_FIELD>>> *controlPanelGSEMap,
    QWidget *leftWidget, QWidget *middleWidget, QWidget *rightWidget)
    : QMainWindow(parent) {
  setWindowTitle(mws::title);
  setGeometry(mws::x, mws::y, mws::width, mws::height);

  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  centralWidget->setStyleSheet(col::background());

  QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);

  // Initialize launch timer
  launchTimerLabel = new QLabel("", this);
  launchTimerLabel->setAlignment(Qt::AlignCenter);
  launchTimerLabel->setStyleSheet(
      "QLabel { font-size: 24px; font-weight: bold; color: red; background: "
      "transparent; }");
  launchTimerLabel->setVisible(false); // Initially hidden
  launchTimer = new QTimer(this);
  launchTimerValue = 0.0;
  connect(launchTimer, &QTimer::timeout, this, &MainWindow::updateLaunchTimer);
  clientManager->subscribe(AV_STATE, [this](const QString &message) {
    this->UpdateLaunchTimerState(message);
  });
  if (controlPanelMap) {
    panelSection = new ControlPanelView(this, controlPanelMap);
  }
  if (controlPanelGSEMap) {
    panelSectionGSE = new ControlPanelView(this, controlPanelGSEMap);
  }
  leftSection = leftWidget;
  middleSection = middleWidget;
  rightSection = rightWidget;
  QHBoxLayout *sectionsLayout = createSectionsLayout();
  centralLayout->addLayout(sectionsLayout);
  _logger.debug("MainWindow", "Setup finished");
}

QHBoxLayout *MainWindow::createSectionsLayout() {
  QHBoxLayout *sectionsLayout = new QHBoxLayout();
  if (leftSection) {
    leftSection->setParent(this);
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(ui_elements::leftPlaceholder);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->verticalScrollBar()->setAttribute(Qt::WA_TranslucentBackground,
                                                  true);

    scrollArea->verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical {"
        "    background: #131313;"
        "    width: 15px;"
        "    margin: 15px 3px 15px 3px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #b0b0b0;"
        "    min-height: 20px;"
        "    border: 2px solid transparent;" // transparent border to trigger
                                             // border-radius
        "    border-radius: 7px;"
        "    background-clip: padding;" // ensure the background respects the
                                        // border-radius
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #a0a0a0;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    background: none;"
        "    height: 15px;"
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "    background: none;"
        "}");
    sectionsLayout->addWidget(scrollArea, (100 - mws::middleSectionWidth) / 2, Qt::AlignHCenter);
  }
  if (middleSection) {
    middleSection->setParent(this);
    if (!leftSection && !rightSection) {
      sectionsLayout->addWidget(middleSection);
    } else {
      sectionsLayout->addWidget(middleSection, mws::middleSectionWidth, Qt::AlignHCenter);
    }
  }
  if (rightSection) {
    rightSection->setParent(this);
    QScrollArea *rightScrollArea = new QScrollArea(this);
    rightScrollArea->setWidget(rightSection);
    rightScrollArea->setWidgetResizable(true);
    // Optionally hide the horizontal scroll bar if you only need vertical
    // scrolling:
    rightScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rightScrollArea->verticalScrollBar()->setAttribute(
        Qt::WA_TranslucentBackground, true);

    rightScrollArea->verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical {"
        "    background: #131313;"
        "    width: 15px;"
        "    margin: 15px 3px 15px 3px;"
        "}"
        "QScrollBar::handle:vertical {"
        "    background: #b0b0b0;"
        "    min-height: 20px;"
        "    border: 2px solid transparent;" // transparent border to trigger
                                             // border-radius
        "    border-radius: 7px;"
        "    background-clip: padding;" // ensure the background respects the
                                        // border-radius
        "}"
        "QScrollBar::handle:vertical:hover {"
        "    background: #a0a0a0;"
        "}"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
        "    background: none;"
        "    height: 15px;"
        "}"
        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
        "    background: none;"
        "}");
        QVBoxLayout *finalRightScrollArea = new QVBoxLayout();
        finalRightScrollArea->addWidget(rightScrollArea, 1);
        finalRightScrollArea->addWidget(panelSection, 1);
        finalRightScrollArea->addWidget(panelSectionGSE, 1);
        sectionsLayout->addLayout(finalRightScrollArea,(100 - mws::middleSectionWidth) / 2);
      }

  return sectionsLayout;
}

void MainWindow::UpdateLaunchTimerState(const QString &av_state) {
  if (av_state == "PRESSURIZATION"){
    if (launchInitiated && !launchTimer->isActive()) {
        startLaunchTimer();
      }
  } else if (av_state == "AoG") {
    launchTimer->stop();
    launchInitiated = false;
  } else if (av_state == "INIT") {
    launchTimer->stop();
    launchTimerLabel->setVisible(false);
  }
}

void MainWindow::initiateLaunchTimer() {
  launchInitiated = true;
}

void MainWindow::startLaunchTimer() {
  launchTimerValue = LAUNCH_DELAY; // Start at -20.0
  launchTimerLabel->setVisible(true);
  updateLaunchTimer();     // Update display immediately
  launchTimer->start(100); // Update every 100ms (0.1 seconds)
}

void MainWindow::updateLaunchTimer() {
  // Format the timer value with one decimal place
  QString timeText = QString::number(launchTimerValue, 'f', 1);
  launchTimerLabel->setText(QString("T  %1 %2s")
                                .arg(launchTimerValue >= 0 ? "+" : " ")
                                .arg(timeText));

  // Increment by 0.1 seconds
  launchTimerValue += 0.1;
}
