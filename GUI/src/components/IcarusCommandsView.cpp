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

    mainLayout->addLayout(configSection);
    mainLayout->addWidget(line);
    mainLayout->addLayout(gimbalSection);
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


IcarusCommandsView::~IcarusCommandsView() {}
