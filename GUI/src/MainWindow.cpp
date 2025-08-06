/**
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief Main window of the application
*/

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollArea>
#include <qabstractscrollarea.h>
#include <QScrollBar>

/*#include "GSEWindow.h"*/
#include "Setup.h"
#include "MainWindow.h"
#include "ControlPannelView.h"
#include "Log.h"


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
    
    /*GSEWindow* gseWindow = new GSEWindow;*/
    /*gseWindow->show();*/
}



QHBoxLayout* MainWindow::createSectionsLayout() {
    QHBoxLayout *sectionsLayout = new QHBoxLayout();

    ui_elements::init_views();

        
    pannelSection = new ControlPannelView(this, &ui_elements::controlMap);
    pannelSection->move(100, 300);
    replacePannelButton();
    
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidget(ui_elements::leftPlaceholder);
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
    sectionsLayout->addWidget(ui_elements::middlePlaceholder, mws::middleSectionWidth);
    sectionsLayout->addWidget(ui_elements::rightPlaceholder, (100-mws::middleSectionWidth)/2);
    

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

