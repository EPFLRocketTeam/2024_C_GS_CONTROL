/**
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief Main window of the application
*/

#include <memory>

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <qabstractscrollarea.h>
#include <QScrollBar>

#include "Setup.h"
#include "MainWindow.h"
#include "ControlPannelView.h"



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle(mws::title);
    setGeometry(mws::x, mws::y, mws::width, mws::height);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setStyleSheet(col::background());
    
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);

    QHBoxLayout *sectionsLayout = createSectionsLayout();
    centralLayout->addLayout(sectionsLayout);
    _logger.debug("MainWindow", "Setup finished");
    
}



QHBoxLayout* MainWindow::createSectionsLayout() {
    QHBoxLayout *sectionsLayout = new QHBoxLayout();

    QFrame *leftSection = new LeftView();
    QFrame *middleSection = new MiddleView();
    QFrame *rightSection = new RightView();
    
    // Instantiate a QMap with std::string keys and std::vector<std::string> values
    QMap<std::string, std::vector<GUI_FIELD>> valvesMap;
    QMap<std::string, std::vector<GUI_FIELD>> pushButtonMap;
    QMap<std::string, QMap<std::string, std::vector<GUI_FIELD>>> controlMap;

    // Populate the QMap with key-value pairs
    valvesMap.insert("Engine Valves", {GUI_FIELD::MAIN_FUEL, GUI_FIELD::MAIN_LOX, GUI_FIELD::VENT_FUEL,
        GUI_FIELD::VENT_LOX, GUI_FIELD::IGNITER_LOX, GUI_FIELD::IGNITER_FUEL});
    valvesMap.insert("GSE Valves", {GUI_FIELD::GSE_VENT});
    
    pushButtonMap.insert("Command", {GUI_FIELD::CMD_1,GUI_FIELD::CMD_2, GUI_FIELD::CMD_3});
    controlMap.insert("ValveControlButton", valvesMap);
    controlMap.insert("QPushButton", pushButtonMap);

    pannelSection = new ControlPannelView(this, &controlMap);
    pannelSection->move(100, 300);

    replacePannelButton();

    
    QScrollArea* scrollArea = new QScrollArea;
    TelemetryView* telemetryView = new TelemetryView;
    scrollArea->setWidget(telemetryView);
    scrollArea->setWidgetResizable(true);
    // Optionally hide the horizontal scroll bar if you only need vertical scrolling:
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->verticalScrollBar()->setAttribute(Qt::WA_TranslucentBackground, true);

    scrollArea->verticalScrollBar()->setStyleSheet(
            "QScrollBar:vertical {"
    "    background: #2e2e2e;"
    "    width: 15px;"
    "    margin: 15px 3px 15px 3px;"
    "}"
    "QScrollBar::handle:vertical {"
    "    background: #b0b0b0;"
    "    min-height: 20px;"
    "    border: 2px solid transparent;"  // transparent border to trigger border-radius
    "    border-radius: 7px;"
    "    background-clip: padding;"        // ensure the background respects the border-radius
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
    "}"    );
    sectionsLayout->addWidget(scrollArea, (100-mws::middleSectionWidth)/2);
    sectionsLayout->addWidget(middleSection, mws::middleSectionWidth);
    sectionsLayout->addWidget(rightSection, (100-mws::middleSectionWidth)/2);
    

    return sectionsLayout;
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    replacePannelButton();    
    pannelSection->resizeWidget();
}

void MainWindow::replacePannelButton() {
    // QPoint p = QWidget::mapTo(this, QPoint(width()/2-pannelButton->width()/2, height()-pannelButton->height()));
    // pannelButton->move(p);
}

