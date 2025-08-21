#include "components/IcarusCommandsView.h"
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <qboxlayout.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qscrollarea.h>
#include <qscrollbar.h>

#include "FieldUtil.h"
#include "MainWindow.h"
#include "RequestBuilder.h"
#include "Setup.h"

IcarusCommandsView::IcarusCommandsView(QWidget *parent)
    : QFrame(parent)
{
    setObjectName("IcarusCommandsView");
    setStyleSheet(col::defaultCardStyle("IcarusCommandsView"));
    QWidget *line = new QWidget;
    line->setFixedHeight(1);
    
    line->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    line->setStyleSheet(QString("background-color: rgba(130, 130, 130, 100);"));
    
    QVBoxLayout* mainLayout = new QVBoxLayout;
    QLayout* configSection = setupIDConfigSection();
    QLayout *gimbalSection = setupGimbalSection();
    QLayout *valvesSection = setupValvesSection();
    /*QScrollArea *scrollArea = new QScrollArea(this);*/
    /*QWidget* mainWidget = new QWidget();*/
    /*mainWidget->setObjectName("IcarusCommandsViewInternalWidget");*/
    /*mainWidget->setStyleSheet(col::defaultCardStyle("IcarusCommandsViewInternalWidget"));*/
    /*mainWidget->setLayout(mainLayout);*/
    /*scrollArea->setWidget(mainWidget);*/
    /*scrollArea->setWidgetResizable(true);*/
    /*// Optionally hide the horizontal scroll bar if you only need vertical*/
    /*// scrolling:*/
    /*scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);*/
    /*scrollArea->verticalScrollBar()->setAttribute(Qt::WA_TranslucentBackground,*/
    /*                                              true);*/
    /**/
    /*scrollArea->verticalScrollBar()->setStyleSheet(*/
    /*    "QScrollBar {"*/
    /*    "   border: none;"*/
    /*    "}"*/
    /*    "QScrollBar:vertical {"*/
    /*    "    background: #2e2e2e;"*/
    /*    "    width: 15px;"*/
    /*    "    margin: 15px 3px 15px 3px;"*/
    /*    "}"*/
    /*    "QScrollBar::handle:vertical {"*/
    /*    "    background: #b0b0b0;"*/
    /*    "    min-height: 20px;"*/
    /*    "    border: 2px solid transparent;" // transparent border to trigger*/
    /*                                         // border-radius*/
    /*    "    border-radius: 7px;"*/
    /*    "    background-clip: padding;" // ensure the background respects the*/
    /*                                    // border-radius*/
    /*    "}"*/
    /*    "QScrollBar::handle:vertical:hover {"*/
    /*    "    background: #a0a0a0;"*/
    /*    "}"*/
    /*    "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"*/
    /*    "    background: none;"*/
    /*    "    height: 15px;"*/
    /*    "}"*/
    /*    "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"*/
    /*    "    background: none;"*/
    /*    "}");*/

    mainLayout->addLayout(configSection);
    mainLayout->addWidget(line);
    mainLayout->addLayout(gimbalSection);
    mainLayout->addWidget(line);
    mainLayout->addLayout(valvesSection);
    /*QVBoxLayout* outterLayout = new QVBoxLayout();*/
    /*outterLayout->addWidget(scrollArea);*/
    setLayout(mainLayout);
}


QLayout* IcarusCommandsView::setupIDConfigSection() {
    QLabel *idLabel = new QLabel("Current ID Config: ");
    idLabel->setStyleSheet(col::labelStyle);
    QLabel *idValue = new QLabel("-");
    idValue->setStyleSheet(col::labelStyle);
    MainWindow::clientManager->subscribe(HOPPER_ID_CONFIG, [idValue](const QString &value) {
        idValue->setText(value);
    });
    QHBoxLayout *configCurrentLayout = new QHBoxLayout;
    configCurrentLayout->addWidget(idLabel);
    configCurrentLayout->addWidget(idValue);

    QLabel *idLineLabel = new QLabel("ID Config: ");
    idLineLabel->setStyleSheet(col::labelStyle);
    
    m_configLineEdit = new QLineEdit(this);
    m_configLineEdit->setPlaceholderText("Enter config ID");
    m_configLineEdit->setStyleSheet(col::labelStyle);
    
    QHBoxLayout *editLayout = new QHBoxLayout;
    editLayout->addWidget(idLineLabel);
    editLayout->addWidget(m_configLineEdit);

    m_sendConfig = new QPushButton("Send Config", this);
    m_sendConfig->setObjectName("config_command_button");
    m_sendConfig->setStyleSheet(col::getButtonStyle("config_command_button"));
    connect(m_sendConfig, &QPushButton::clicked, this, [this](){
        bool ok;
        int configId = m_configLineEdit->text().toInt(&ok);
        if (!ok) {
            _logger.error("Parse Config Id", QString(R"(Couldn't parse an id from the following string: %1)")
                          .arg(m_configLineEdit->text()).toStdString());
            return;
        }
        RequestBuilder b = RequestBuilder();
        b.setHeader(RequestType::POST);
        b.addField("cmd", HOPPER_ID_CONFIG);
        b.addField("cmd_order", configId);
        MainWindow::clientManager->send(b.toString());
        _logger.info("Sent Id Config", QString(R"(sent config id = %1)").arg(configId).toStdString());
    });

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(configCurrentLayout);
    mainLayout->addLayout(editLayout);
    mainLayout->addWidget(m_sendConfig);
    return mainLayout;
       
}


QLayout* IcarusCommandsView::setupGimbalSection() {
    QLabel* gimbalXLabel = new QLabel("Gimbal X:", this);
    gimbalXLabel->setStyleSheet(QString("font-size: 14pt; color: %1;font-weight: 400;background: transparent;").arg(col::primary));
    QLabel* gimbalYLabel = new QLabel("Gimbal Y:", this);
    gimbalYLabel->setStyleSheet(QString("font-size: 14pt; color: %1;font-weight: 400;background: transparent;").arg(col::primary));

    // Create the line edits and set placeholder text
    m_gimbalXLineEdit = new QLineEdit(this);
    m_gimbalXLineEdit->setPlaceholderText("Enter number for Gimbal X");
    m_gimbalXLineEdit->setStyleSheet(col::labelStyle);
    m_gimbalYLineEdit = new QLineEdit(this);
    m_gimbalYLineEdit->setPlaceholderText("Enter number for Gimbal Y");
    m_gimbalYLineEdit->setStyleSheet(col::labelStyle);

    // Create the button and connect its clicked signal to the slot
    m_commandButton = new QPushButton("Send Command", this);
    m_commandButton->setObjectName("gimbal_command_button");
    m_commandButton->setStyleSheet(col::getButtonStyle("gimbal_command_button"));
    connect(m_commandButton, &QPushButton::clicked, this, &IcarusCommandsView::onButtonClicked);
    
    // Layout for the Gimbal X row
    QHBoxLayout* layoutX = new QHBoxLayout;
    layoutX->addWidget(gimbalXLabel);
    layoutX->addWidget(m_gimbalXLineEdit);

    // Layout for the Gimbal Y row
    QHBoxLayout* layoutY = new QHBoxLayout;
    layoutY->addWidget(gimbalYLabel);
    layoutY->addWidget(m_gimbalYLineEdit);

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(setupCurrentValueLabels());
    mainLayout->addLayout(layoutX);
    mainLayout->addLayout(layoutY);
    mainLayout->addWidget(m_commandButton);

    return mainLayout;
}


QLayout* IcarusCommandsView::setupCurrentValueLabels() {
    QLabel *xLabel = new QLabel("Gimbal X: ");
    xLabel->setStyleSheet(col::labelStyle);
    QLabel *xValue = new QLabel("-");
    xValue->setStyleSheet(col::labelStyle);
    QLabel *yLabel = new QLabel("Gimbal Y: ");
    yLabel->setStyleSheet(col::labelStyle);
    QLabel *yValue = new QLabel("-");
    yValue->setStyleSheet(col::labelStyle);
    QHBoxLayout *currentValLayout = new QHBoxLayout;
    currentValLayout->addWidget(xLabel);
    currentValLayout->addWidget(xValue);
    currentValLayout->addWidget(yLabel);
    currentValLayout->addWidget(yValue);
    MainWindow::clientManager->subscribe(HOPPER_GIMBAL_X, [xValue](const QString &value){
        xValue->setText(value);
    });
    MainWindow::clientManager->subscribe(HOPPER_GIMBAL_Y, [yValue](const QString &value){
        yValue->setText(value);
    });
    return currentValLayout;
}


QLayout* IcarusCommandsView::setupValvesSection() {
    QLabel* mainOLabel = new QLabel("Main N2O", this);
    mainOLabel->setStyleSheet(QString("font-size: 14pt; color: %1;font-weight: 400;background: transparent;").arg(col::primary));
    QLabel* MainELabel = new QLabel("Main Fuel", this);
    MainELabel->setStyleSheet(QString("font-size: 14pt; color: %1;font-weight: 400;background: transparent;").arg(col::primary));

    // Create the line edits and set placeholder text
    m_mainOLineEdit = new QLineEdit(this);
    m_mainOLineEdit->setPlaceholderText("Enter number for Main N2O");
    m_mainOLineEdit->setStyleSheet(col::labelStyle);
    m_mainELineEdit = new QLineEdit(this);
    m_mainELineEdit->setPlaceholderText("Enter number for Main Fuel");
    m_mainELineEdit->setStyleSheet(col::labelStyle);

    // Create the button and connect its clicked signal to the slot
    m_valvesCommandButton = new QPushButton("Send Command", this);
    m_valvesCommandButton->setObjectName("valves_command_button");
    m_valvesCommandButton->setStyleSheet(col::getButtonStyle("valves_command_button"));
    connect(m_valvesCommandButton, &QPushButton::clicked, this, &IcarusCommandsView::onValvesCommandButtonClicked);
    
    // Layout for the Gimbal X row
    QHBoxLayout* layoutX = new QHBoxLayout;
    layoutX->addWidget(mainOLabel);
    layoutX->addWidget(m_mainOLineEdit);

    // Layout for the Gimbal Y row
    QHBoxLayout* layoutY = new QHBoxLayout;
    layoutY->addWidget(MainELabel);
    layoutY->addWidget(m_mainELineEdit);

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(setupCurrentValvesLabels());
    mainLayout->addLayout(layoutX);
    mainLayout->addLayout(layoutY);
    mainLayout->addWidget(m_valvesCommandButton);

    return mainLayout;
}


QLayout* IcarusCommandsView::setupCurrentValvesLabels() {
    QLabel *xLabel = new QLabel("Main N20: ");
    xLabel->setStyleSheet(col::labelStyle);
    QLabel *xValue = new QLabel("-");
    xValue->setStyleSheet(col::labelStyle);
    QLabel *yLabel = new QLabel("Main Fuel: ");
    yLabel->setStyleSheet(col::labelStyle);
    QLabel *yValue = new QLabel("-");
    yValue->setStyleSheet(col::labelStyle);
    QHBoxLayout *currentValLayout = new QHBoxLayout;
    currentValLayout->addWidget(xLabel);
    currentValLayout->addWidget(xValue);
    currentValLayout->addWidget(yLabel);
    currentValLayout->addWidget(yValue);
    MainWindow::clientManager->subscribe(HOPPER_N2O_MAIN, [xValue](const QString &value){
        xValue->setText(value);
    });
    MainWindow::clientManager->subscribe(HOPPER_ETH_MAIN, [yValue](const QString &value){
        yValue->setText(value);
    });
    return currentValLayout;
}


void IcarusCommandsView::onButtonClicked()
{
    // Retrieve values from the text fields and convert to double
    bool okX, okY;
    int xValue = m_gimbalXLineEdit->text().toInt(&okX);
    int yValue = m_gimbalYLineEdit->text().toInt(&okY);
    if (xValue < -15 || xValue > 15 || yValue < -15 || yValue > 15) {
        _logger.error("Invalid Command Value", QString(R"(The value gimbal_x=%1, gimbal_y=%2, contain an invalid value, they should be between -15 and 15)").arg(xValue).arg(yValue).toStdString());
        return;
    }

    if (okX && okY) {
        RequestBuilder b = RequestBuilder();
        b.setHeader(RequestType::POST);
        b.addField("cmd", GUI_CMD_GIMBALL_X);
        b.addField("cmd_order", xValue);
        MainWindow::clientManager->send(b.toString());

        b.clear();
        b.setHeader(RequestType::POST);
        b.addField("cmd", GUI_CMD_GIMBALL_Y);
        b.addField("cmd_order", yValue);
        MainWindow::clientManager->send(b.toString());
        _logger.info("Sent Gimbal Values", QString(R"(Sent gimbal_x=%1, gimbal_y=%2)").arg(xValue).arg(yValue).toStdString());

    } else {
        _logger.error("Couldn't Send Gimbal Values", QString(R"(One of the input was not valid. X input was "%1", Y input was "%2")")
                      .arg(m_gimbalXLineEdit->text()).arg(m_gimbalYLineEdit->text()).toStdString());
    }
}


void IcarusCommandsView::onValvesCommandButtonClicked()
{
    // Retrieve values from the text fields and convert to double
    bool okX, okY;
    int n2oValue = m_mainOLineEdit->text().toInt(&okX);
    int fuelValue = m_mainELineEdit->text().toInt(&okY);
    if (n2oValue < 0 || n2oValue > 100 || fuelValue < 0 || fuelValue > 100) {
        _logger.error("Invalid Command Value", QString(R"(The value valve_MAIN_N2O=%1, valve_MAIN_FUEL=%2, contain an invalid value, they should be between 0 and 100)").arg(n2oValue).arg(fuelValue).toStdString());
        return;
    }

    if (okX && okY) {
        RequestBuilder b = RequestBuilder();
        b.setHeader(RequestType::POST);
        b.addField("cmd", HOPPER_N2O_MAIN);
        b.addField("cmd_order", n2oValue);
        MainWindow::clientManager->send(b.toString());

        b.clear();
        b.setHeader(RequestType::POST);
        b.addField("cmd", HOPPER_ETH_MAIN);
        b.addField("cmd_order", fuelValue);
        MainWindow::clientManager->send(b.toString());
        _logger.info("Sent Gimbal Values", QString(R"(Sent gimbal_x=%1, gimbal_y=%2)").arg(n2oValue).arg(fuelValue).toStdString());

    } else {
        _logger.error("Couldn't Send Gimbal Values", QString(R"(One of the input was not valid. X input was "%1", Y input was "%2")")
                      .arg(m_gimbalXLineEdit->text()).arg(m_gimbalYLineEdit->text()).toStdString());
    }
}

IcarusCommandsView::~IcarusCommandsView() {}
