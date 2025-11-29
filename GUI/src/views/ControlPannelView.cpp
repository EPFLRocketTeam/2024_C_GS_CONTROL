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

#include "ControlPannelView.h"
#include "FieldUtil.h"
#include "MainWindow.h"
#include "RequestBuilder.h"
#include "components/ValveControlButton.h"
#include <Setup.h>

ControlPannelView::ControlPannelView(
    QWidget *parent, QMap<std::string, QList<std::vector<GUI_FIELD>>> *controls)
    : QFrame(parent) {

  setStyleSheet("background:transparent;");
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  toggled = false;
  // Add a QLabel to display text
  displayText = std::make_unique<QLabel>("This is DataView");
  displayText->setAlignment(Qt::AlignCenter);

  setupExpandButton();
  setupContainerWidget();

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->addWidget(expandButton, 1, Qt::AlignCenter);
  layout->addWidget(controlContainerWidget, 1, Qt::AlignCenter);
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);

  QHBoxLayout *containerLayout = new QHBoxLayout(controlContainerWidget);
  QList<std::vector<GUI_FIELD>> valveControls =
      controls->value("ValveControlButton");
  createValveLayouts(containerLayout, &valveControls);
  QList<std::vector<GUI_FIELD>> pushButtonControls =
      controls->value("QPushButton");
  createPushButtonLayouts(containerLayout, &pushButtonControls);

  containerLayout->setContentsMargins(20, 10, 20, 10);
  containerLayout->setSpacing(15);

  controlContainerWidget->setFixedHeight(210);

  connect(expandButton, &QPushButton::clicked, this,
          &ControlPannelView::expandClicked);

  resizeWidget();

  _logger.debug("ControlPannelView", "Setup finished");
}

void ControlPannelView::createValveLayouts(
    QHBoxLayout *mainLayout, QList<std::vector<GUI_FIELD>> *valves) {

  for (auto it : *valves) {
    /*const QString &title = QString::fromStdString(it.key());*/
    int maxColumns = std::ceil(it.size() / 3.0);
    const std::vector<GUI_FIELD> &valveNames = it;

    QVBoxLayout *controlLayout = new QVBoxLayout;
    /*QLabel *titleLabel = new QLabel(title);*/
    /*QFont font = titleLabel->font();*/
    /*font.setPointSize(12);*/
    /*font.setBold(true);*/
    /*titleLabel->setFont(font);*/
    /*titleLabel->setStyleSheet(QString("color:%1").arg(col::primary));*/
    /*controlLayout->addWidget(titleLabel, 1, Qt::AlignLeft);*/

    QGridLayout *gridLayout = new QGridLayout;
    createValveControlButtons(gridLayout, valveNames, maxColumns);
    controlLayout->addLayout(gridLayout, 4);

    mainLayout->addLayout(controlLayout);
  }
}

QMessageBox::StandardButton ControlPannelView::showConfirmDialog(QWidget *parent, 
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

void ControlPannelView::createPushButtonLayouts(
    QHBoxLayout *mainLayout, QList<std::vector<GUI_FIELD>> *buttons) {

  for (auto it : *buttons) {

    /*const QString &title = QString::fromStdString(it.key());*/
    const std::vector<GUI_FIELD> &buttonField = it;

    QVBoxLayout *controlLayout = new QVBoxLayout;
    /*QLabel *titleLabel = new QLabel(title);*/

    /*titleLabel->setFixedHeight(37);*/
    /*titleLabel->setStyleSheet(QString("color:%1;").arg(col::primary));*/
    /*QFont font = titleLabel->font();*/
    /*font.setPointSize(13);*/
    /*font.setBold(true);*/
    /*titleLabel->setFont(font);*/
    /*controlLayout->addWidget(titleLabel, 1, Qt::AlignLeft);*/
    controlLayout->setSpacing(15);
    QGridLayout *gridLayout = new QGridLayout;

    gridLayout->setSpacing(15);
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
                padding:5px;
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
          _logger.debug("ControlPannelView", QString("Cancelled action for %1")
                                                 .arg(button->text())
                                                 .toStdString());
        }
      });

      _logger.debug(
          "ControlPannelView",
          QString(R"(Created Button %1)").arg(button->text()).toStdString());
    }

    controlLayout->addLayout(gridLayout, 4);
    controlLayout->setAlignment(Qt::AlignTop);

    mainLayout->addLayout(controlLayout, Qt::AlignBottom);
    // mainLayout->addWidget(w, Qt::AlignTop);
  }
}

void ControlPannelView::createValveControlButtons(
    QGridLayout *gridLayout, const std::vector<GUI_FIELD> &fields,
    int maxColumns) {
  // Clear existing items from the grid layout
  QLayoutItem *child;
  while ((child = gridLayout->takeAt(0)) != nullptr) {
    delete child->widget();
    delete child;
  }

  // Set the maximum number of columns based on the desired width
  // Set the desired number of columns
  gridLayout->setColumnStretch(maxColumns, 1);

  int row = 0;
  int column = 0;

  // Add ValveControlButton for each string
  for (const GUI_FIELD &field : fields) {
    ValveControlButton *button = new ValveControlButton(field);
    gridLayout->addWidget(button, row, column);

    // Move to the next row or wrap to the next column
    if (++column == maxColumns) {
      column = 0;
      ++row;
    }
  }
}

void ControlPannelView::setupContainerWidget() {

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

void ControlPannelView::resizeEvent(QResizeEvent *event) { resizeWidget(); }

void ControlPannelView::setupExpandButton() {

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

void ControlPannelView::resizeWidget() {
  int windowWidth = parentWidget()->width();
  int newWidth(windowWidth * mws::middleSectionWidth / 100);
  int newHeight(controlContainerWidget->height() + expandButton->height());
  setFixedWidth(newWidth);

  move(QPoint((windowWidth - newWidth) / 2, getHeightPos()));
  // Set a minimum height to ensure visibility
  setFixedHeight(newHeight);

  QPoint p =
      QWidget::mapTo(this, QPoint(width() / 2 - expandButton->width() / 2, 0));

  controlContainerWidget->setFixedWidth(newWidth);
  // controlContainerWidget->move(0, expandButton->height());
}

void ControlPannelView::expandClicked() {
  toggled = !toggled;
  QPropertyAnimation *anim = new QPropertyAnimation(this, "pos");
  anim->setDuration(1000);
  anim->setEasingCurve(QEasingCurve::Type::OutQuart);
  anim->setStartValue(pos());
  anim->setEndValue(QPoint(pos().x(), getHeightPos()));
  anim->start(QAbstractAnimation::DeleteWhenStopped);
  QTransform transform;
  buttonPixMap = buttonPixMap.transformed(transform.rotate(180));
  QIcon buttonIcon(buttonPixMap);
  expandButton->setFixedHeight(35);
  expandButton->setIcon(buttonIcon);
  expandButton->setIconSize(QSize(64, 64));
}

int ControlPannelView::getHeightPos() {
  int windowWidth = parentWidget()->width();
  int newHeight(controlContainerWidget->height() + expandButton->height());
  int newY;
  if (toggled) {
    newY = parentWidget()->height() - newHeight;
  } else {
    newY = parentWidget()->height() - expandButton->height();
  }
  return newY;
}
