/**
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief QFrame displaying global data
*/

#include <algorithm>
#include <memory>

#include "QMessageBox"
#include <QFrame>
#include <QGraphicsPixmapItem>
#include <QGridLayout>
#include <QStyleFactory>
#include <QHBoxLayout>
#include <QLabel>
#include <QMatrix2x2>
#include <QPixmap>
#include <QPropertyAnimation>
#include <QVBoxLayout>

#include "ControlPanelView.h"
#include "FieldUtil.h"
#include "MainWindow.h"
#include "RequestBuilder.h"
#include "components/ValveControlButton.h"
#include <Setup.h>

ControlPanelView::ControlPanelView(
    QWidget *parent, QMap<std::string, QList<std::vector<GUI_FIELD>>> *controls)
    : QFrame(parent) {
  setStyleSheet("background:transparent;");
  toggled = false;
  // Add a QLabel to display text
  displayText = std::make_unique<QLabel>("This is DataView");
  displayText->setAlignment(Qt::AlignCenter);
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
  setMinimumWidth(mws::sideWidth / 100.0 * mws::width);
  setMaximumWidth(mws::sideWidth / 3 * mws::width);
  setMaximumHeight(mws::height*10.0/100.0);
  setupContainerWidget();


  QHBoxLayout *containerLayout = new QHBoxLayout(controlContainerWidget);
  QList<std::vector<GUI_FIELD>> pushButtonControls =
      controls->value("QPushButton");
  createPushButtonLayouts(containerLayout, &pushButtonControls);

  _logger.debug("ControlPanelView", "Setup finished");
}


QMessageBox::StandardButton ControlPanelView::showConfirmDialog(QWidget *parent, 
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
    .arg(col::primary)              // Yes button background
    .arg(col::complementary)        // Yes button hover
    .arg(col::backgroundColorCode)  // No button background
    .arg(col::complementaryLighter) // No button hover
    );

    // Give object names so stylesheet can target them
    /*QAbstractButton *yesBtn = msgBox.button(QMessageBox::Yes);*/
    /*if (yesBtn) yesBtn->setObjectName("yesButton");*/
    /**/
    /*QAbstractButton *noBtn = msgBox.button(QMessageBox::No);*/
    /*if (noBtn) noBtn->setObjectName("noButton");*/

    return static_cast<QMessageBox::StandardButton>(msgBox.exec());
}

void ControlPanelView::createPushButtonLayouts(
    QHBoxLayout *mainLayout, QList<std::vector<GUI_FIELD>> *buttons) {

  for (auto it : *buttons) {

    /*const QString &title = QString::fromStdString(it.key());*/
    const std::vector<GUI_FIELD> &buttonField = it;

    QGridLayout *gridLayout = new QGridLayout;

    
    gridLayout->setSpacing(10);
    int maxColumns =
        std::max(static_cast<int>(std::ceil(buttonField.size() / 3.0)), 1);

    for (int i = 0; i < buttonField.size(); ++i) {
      std::string trimmedName =
          fieldUtil::enumToFieldName(buttonField[i]).toStdString();
      QPushButton *button =
          new QPushButton(fieldUtil::enumToFieldName(buttonField[i]));
      std::replace(trimmedName.begin(), trimmedName.end(), ' ', '_');
      button->setObjectName(QString::fromStdString(trimmedName));
      QString style = QString(R"(
                #%5 {
                color: %4;
                font: bold 14px;
                background: %1;
                padding:3px;
                border:2px solid %1;
                border-radius: 10px;
                }
                #%5:hover {
                    background-color: %3;     
                    
                }
                #%5:pressed {
                    background-color: %2!important;     
                    border:2px solid %4;
                }   
                )")
                          .arg(col::complementary)
                          .arg(col::backgroundColorCode)
                          .arg(col::complementaryLighter)
                          .arg(col::primary)
                          .arg(QString::fromStdString(trimmedName));
      button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
      button->setMinimumWidth(100);
      button->setMaximumWidth(300);
      button->setFixedHeight(40);
      button->setStyleSheet(style);
      gridLayout->addWidget(button, i / maxColumns, i % maxColumns);

      QObject::connect(button, &QPushButton::clicked, [button, this]() {
        // Show confirmation dialog
/*        QMessageBox msgBox(this);*/
/*        msgBox.setWindowTitle("Confirm Action");*/
/*        msgBox.setText(QString("Are you sure you want to execute '%1'?")*/
/*                           .arg(button->text()));*/
/*        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);*/
/*        msgBox.setDefaultButton(QMessageBox::No);*/
/**/
/*        // Force light theme (Fusion + palette or stylesheet)*/
/*        msgBox.setStyle(QStyleFactory::create("Fusion"));*/
/*        msgBox.setStyleSheet(R"(*/
/*    QMessageBox {*/
/*        background-color: white;*/
/*        color: black;*/
/*    }*/
/*    QLabel {*/
/*        color: black;*/
/*    }*/
/*    QPushButton {*/
/*        background-color: #f0f0f0;*/
/*        color: black;*/
/*        border: 1px solid gray;*/
/*        padding: 5px 10px;*/
/*        border-radius: 4px;*/
/*    }*/
/*    QPushButton:hover {*/
/*        background-color: #e0e0e0;*/
/*    }*/
/*)");*/
/**/
        /*QMessageBox::StandardButton reply =*/
        /*    static_cast<QMessageBox::StandardButton>(msgBox.exec());*/
        if (showConfirmDialog(this, "Confirm Action",
        QString("Are you sure you want to execute '%1'?").arg(button->text())) 
        == QMessageBox::Yes) {
          // Check if this is the LAUNCH command and start the timer
          if (button->text() == "LAUNCH") {
            // Get the main window and start the launch timer
            QWidget *mainWindow = this;
            while (mainWindow->parentWidget()) {
              mainWindow = mainWindow->parentWidget();
            }
            if (MainWindow *mw = qobject_cast<MainWindow*>(mainWindow)) {
              mw->initiateLaunchTimer();
            }
          }
          
          // Proceed with request
          RequestBuilder b;
          b.setHeader(RequestType::POST);
          b.addField("cmd", fieldUtil::fieldNameToEnum(button->text()));
          b.addField("cmd_order", 1);
          _logger.debug(QString(R"(Confirmed %1 Button)")
                            .arg(button->text())
                            .toStdString(),
                        b.toString().toStdString());
          MainWindow::clientManager->send(b.toString());
        } else {
          _logger.debug("ControlPanelView", QString("Cancelled action for %1")
                                                 .arg(button->text())
                                                 .toStdString());
        }
      });

      _logger.debug(
          "ControlPanelView",
          QString(R"(Created Button %1)").arg(button->text()).toStdString());
    }

    mainLayout->addLayout(gridLayout, 2);
    // mainLayout->addWidget(w, Qt::AlignTop);
  }
}


void ControlPanelView::setupContainerWidget() {

  controlContainerWidget = new QWidget(this);


  controlContainerWidget->setObjectName("controlPannel");
  QString controlPannelStyle = QString(R"(
        #controlPannel {
            
            border-top-left-radius: 10%;
            border-top-right-radius: 10%;

        }
    )");
  controlContainerWidget->setStyleSheet(col::defaultCardStyle("controlPannel") +
                                        controlPannelStyle);
}

void ControlPanelView::setupExpandButton() {

  expandButton = new QPushButton(this);
  expandButton->setObjectName("expandButton");
  QString buttonStyle = QString(R"(
        #expandButton {
            background: %1;
            border-top-left-radius: 10%;
            border-top-right-radius: 10%;
        }
    )")
                            .arg(col::complementary);
  expandButton->setStyleSheet(buttonStyle);

  // button icon : https://icones8.fr/icon/7LmItvRdpSjs/up-arrow
  buttonPixMap = QPixmap(":/icons/up-arrow.png");

  QIcon buttonIcon(buttonPixMap);

  expandButton->setFixedHeight(35);
  expandButton->setIcon(buttonIcon);
  expandButton->setIconSize(QSize(64, 64));
}