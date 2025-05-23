#include "SerialView.h"
#include "MainWindow.h"
#include "RequestBuilder.h"
#include "../Setup.h"
#include <qboxlayout.h>
#include <qthread.h>

SerialView::SerialView(std::unique_ptr<QWidget> parent) : QFrame(parent.get()) {
    setObjectName("SerialView");
    setStyleSheet(col::defaultCardStyle("SerialView"));
    
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    
    isOpen = false;
    QHBoxLayout* outterLayout = new QHBoxLayout(this);
    outterLayout->setContentsMargins(5, 5, 5, 5);
    layout = new QHBoxLayout();
    outterLayout->addLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    setupStatusLed();

    serialNameLabel = new QLabel(QString("Serial port used : %1").arg("-")); 
    serialNameLabel->setObjectName("child");

    openButton = new QPushButton; 
    openButton->setObjectName("child");
    openButton->setText("Open");

    layout->addWidget(serialNameLabel, 1, Qt::AlignHCenter);
    layout->addWidget(openButton, 1);
    layout->addWidget(statusLed, 1, Qt::AlignHCenter);
    updateStatus("unknown");
    setupStyle();
    setupConnections();

}

void SerialView::buttonClicked() {
    RequestBuilder b;
    b.setHeader(RequestType::POST);
    b.addField("cmd", GUI_FIELD::GUI_CMD_SET_SERIAL_STATUS);
 
    if (isOpen) {
        b.addField("cmd_order", "close");
        MainWindow::clientManager->send(b.toString());

        
        return;
    } 
    b.addField("cmd_order", "open");
    MainWindow::clientManager->send(b.toString());
    
    
}

void SerialView::setupStyle() {
    openButton->setFixedHeight(35);
    openButton->setObjectName("openButton");
    QString style = QString(R"(
        QPushButton#%5 {
        color: %4;
        font: bold 14px;
        background: %1;
        border:2px solid %1;
        border-radius: 10px;
        }
        QPushButton#%5:hover {
            background-color: %3;     
            
        }
        QPushButton#%5:pressed {
            background-color: %2!important;     
            border:2px solid %4;
        }
        
    )")
    .arg(col::complementary)
    .arg(col::backgroundColorCode)
    .arg(col::complementaryLighter)
    .arg(col::primary)
    .arg("openButton");
    openButton->setStyleSheet(style);
}


void SerialView::setupConnections() {
    
    connect(openButton, &QPushButton::clicked, this, &SerialView::buttonClicked);

    MainWindow::clientManager->subscribe(GUI_FIELD::SERIAL_STATUS, setSerialStatus);
    MainWindow::clientManager->subscribe(GUI_FIELD::SERIAL_NAME_USE, setSerialName);

    RequestBuilder b;
    b.setHeader(RequestType::POST);
    b.addField("cmd", GUI_FIELD::SERIAL_NAME_USE);
    MainWindow::clientManager->send(b.toString());

    b.clear();
    b.setHeader(RequestType::POST);
    b.addField("cmd", GUI_FIELD::SERIAL_STATUS);
    MainWindow::clientManager->send(b.toString());

    
}

void SerialView::changeButtonStyle(bool targetStatus) {
    if (targetStatus) {
        openButton->setText("Open"); 
        return;
    } 
    openButton->setText("Close");
}

void SerialView::updateStatus(QString newStatus) {
    if (newStatus == "open") {
        isOpen = true;
        statusLed->setStyleSheet(ledGreenStyle);
        statusLed->setToolTip("The serial is open");
    } else if (newStatus == "close") {
        isOpen = false; 
        statusLed->setStyleSheet(ledRedStyle);
        statusLed->setToolTip("The serial is closed");
    } else if (newStatus == "unknown") {
        isOpen = false; 
        statusLed->setStyleSheet(ledYellowStyle);
        statusLed->setToolTip("The serial state is unkown");
    } else {
        throw std::runtime_error("Invalid argument passed in function \"SerialView::updateStatus\", expected \"open\" or \"close\"");
    }
    changeButtonStyle(!isOpen);
}

void SerialView::setupStatusLed() {
    statusLed = new QLabel;
    statusLed->setObjectName("statusLed");
    statusLed->setFixedSize(20, 20);
    
}
