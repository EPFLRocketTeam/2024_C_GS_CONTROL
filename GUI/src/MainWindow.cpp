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

#include "ControlPannelView.h"
#include "GSEWindow.h"
#include "Log.h"
#include "MainWindow.h"
#include "Setup.h"

MainWindow::MainWindow(
    QWidget *parent,
    QMap<std::string, QList<std::vector<GUI_FIELD>>>
        *controlPannelMap,
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
  launchTimerLabel->setStyleSheet("QLabel { font-size: 24px; font-weight: bold; color: red; background: transparent; }");
  launchTimerLabel->setVisible(false); // Initially hidden
  launchTimer = new QTimer(this);
  launchTimerValue = 0.0;
  connect(launchTimer, &QTimer::timeout, this, &MainWindow::updateLaunchTimer);

  // Add launch timer to top of layout
  centralLayout->addWidget(launchTimerLabel, 0, Qt::AlignCenter);

  pannelSection = new ControlPannelView(this, controlPannelMap);
  leftSection = leftWidget;
  middleSection = middleWidget;
  rightSection = rightWidget;
  QHBoxLayout *sectionsLayout = createSectionsLayout();
  centralLayout->addLayout(sectionsLayout);
  _logger.debug("MainWindow", "Setup finished");
}

QHBoxLayout *MainWindow::createSectionsLayout() {
  QHBoxLayout *sectionsLayout = new QHBoxLayout();

  pannelSection->move(100, 300);
  replacePannelButton();

  if (leftSection) {
    leftSection->setParent(this);
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidget(ui_elements::leftPlaceholder);
    scrollArea->setWidgetResizable(true);
    // Optionally hide the horizontal scroll bar if you only need vertical
    // scrolling:
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->verticalScrollBar()->setAttribute(Qt::WA_TranslucentBackground,
                                                  true);

    scrollArea->verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical {"
        "    background: #2e2e2e;"
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
    sectionsLayout->addWidget(scrollArea, (100 - mws::middleSectionWidth) / 2);
  }
  if (middleSection) {
    /*middleSection->setParent(this);*/
    sectionsLayout->addWidget(middleSection, mws::middleSectionWidth);
  }
  if (rightSection) {
    /*rightSection->setParent(this);*/
    /*sectionsLayout->addWidget(rightSection,*/
    /*                          (100 - mws::middleSectionWidth) / 2);*/

    rightSection->setParent(this);
    QScrollArea *rightScrollArea = new QScrollArea(this);
    rightScrollArea->setWidget(rightSection);
    rightScrollArea->setWidgetResizable(true);
    // Optionally hide the horizontal scroll bar if you only need vertical
    // scrolling:
    rightScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    rightScrollArea->verticalScrollBar()->setAttribute(Qt::WA_TranslucentBackground,
                                                  true);

    rightScrollArea->verticalScrollBar()->setStyleSheet(
        "QScrollBar:vertical {"
        "    background: #2e2e2e;"
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
    sectionsLayout->addWidget(rightScrollArea, (100 - mws::middleSectionWidth) / 2);
  
  }

  return sectionsLayout;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
  replacePannelButton();
  pannelSection->resizeWidget();
}

void MainWindow::replacePannelButton() {
  // QPoint p = QWidget::mapTo(this, QPoint(width()/2-pannelButton->width()/2,
  // height()-pannelButton->height())); pannelButton->move(p);
}

void MainWindow::startLaunchTimer() {
  launchTimerValue = LAUNCH_DELAY; // Start at -20.0
  launchTimerLabel->setVisible(true);
  updateLaunchTimer(); // Update display immediately
  launchTimer->start(100); // Update every 100ms (0.1 seconds)
}

void MainWindow::updateLaunchTimer() {
  // Format the timer value with one decimal place
  QString timeText = QString::number(launchTimerValue, 'f', 1);
  launchTimerLabel->setText(QString("T: %1 %2s").arg(launchTimerValue >= 0 ? "+" : "").arg(timeText));
  
  // Increment by 0.1 seconds
  launchTimerValue += 0.1;
}
