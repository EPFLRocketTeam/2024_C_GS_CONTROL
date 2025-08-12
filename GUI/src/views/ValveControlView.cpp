/**
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief QFrame displaying global data
*/
#include <iostream>
#include <memory>

#include <QPainter>
#include <QPushButton>
#include <QtSvg/QSvgRenderer>
#include <qglobal.h>
#include <qnamespace.h>
#include <qsvgrenderer.h>
#include <qwidget.h>

#include "../Setup.h"
#include "FieldUtil.h"
#include "MainWindow.h"
#include "RequestBuilder.h"
#include "ValveControlView.h"
#include "components/DataLabel.h"
#include "components/ValveButton.h"

ValveControlView::ValveControlView(std::vector<ValveInfo> valves,
                                   std::vector<LabelInfo> labels,
                                   QString connectedBg, QString disconnectedBg,
                                   QWidget *parent)
    : QFrame(parent), svgRenderer(nullptr) {
  setContentsMargins(25, 25, 25, 25);
  setMinimumWidth(mws::middleSectionWidth / 100.0 * mws::width);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  _valves = valves;
  _labels = labels;
  connectedBgPath = connectedBg;
  disconnectedBgPath = disconnectedBg;
  MainWindow::clientManager->subscribe(GUI_FIELD::GSE_DISCONNECT_ACTIVE,
                                       [this](const QString &message) {
                                         if (message == "1") {
                                           setSvgBackground(disconnectedBgPath);
                                         } else {
                                           setSvgBackground(connectedBgPath);
                                         }
                                       });
  setSvgBackground(connectedBgPath);
  placeValves();
  /*placeCommandButtons();*/
  placeDataLabels();
  /*DraggableButton *dButton = new DraggableButton(this);*/
}

void ValveControlView::placeValves() {
  for (auto valveInfo : _valves) {
    addButtonIcon(valveInfo.f, valveInfo.p.x, valveInfo.p.y, valveInfo.o);
  }
}

void ValveControlView::addDataLabel(const GUI_FIELD field, float x, float y) {
  DataLabel *label = new DataLabel(field, this);
  addComponent(label, x, y);
}

void ValveControlView::placeDataLabels() {
  for (auto labelInfo : _labels) {
    addDataLabel(labelInfo.f, labelInfo.p.x, labelInfo.p.y);
  }
  /*//GSE top*/
  /*addDataLabel(GUI_FIELD::GSE_TANK_PRESSURE, 0.09, 0.25);*/
  /*addDataLabel(GUI_FIELD::GSE_TANK_TEMPERATURE, 0.09, 0.315);*/
  /*//GSE bottom*/
  /*addDataLabel(GUI_FIELD::GSE_FILLING_PRESSURE, 0.138, 0.576);*/
  /**/
  /*//N2O top pressure   */
  /*addDataLabel(GUI_FIELD::CHAMBER_PRESSURE, 0.609595, 0.178905);*/
  /**/
  /*//Engine tank pressure*/
  /*addDataLabel(GUI_FIELD::HOPPER_N2O_PRESSURE, 0.447, 0.439);*/
  /*addDataLabel(GUI_FIELD::HOPPER_N2O_TEMP, 0.447, 0.502);*/
  /*addDataLabel(GUI_FIELD::HOPPER_ETH_PRESSURE, 0.893, 0.438);*/
  /**/
  /*// Engine left pressure */
  /*addDataLabel(GUI_FIELD::CHAMBER_PRESSURE,  0.540116, 0.785047);*/
  /*addDataLabel(GUI_FIELD::CHAMBER_PRESSURE, 0.540116, 0.867824);*/
  /*addDataLabel(GUI_FIELD::CHAMBER_PRESSURE, 0.540116, 0.94526);*/
  /**/
  /*// Engine right pressure*/
  /*addDataLabel(GUI_FIELD::CHAMBER_PRESSURE, 0.924731, 0.783712);*/
  /*addDataLabel(GUI_FIELD::CHAMBER_PRESSURE,0.924731, 0.87984);*/
}

void ValveControlView::placeCommandButtons() {
  addCommandButton("Pressurize", 0.757651, -0.0440587);
  addCommandButton("Ignition", 0.68, 0.9);
  addCommandButton("Disconnect", 0.409429, 0.863818);
}

void ValveControlView::addCommandButton(const QString &label, float x,
                                        float y) {
  QPushButton *button = new QPushButton(label, this);
  button->setObjectName("commandButton");
  QString style = QString(R"(
        #%5 {
        color: %4;
        font: bold 14px;
        background: %1;
        border:2px solid %1;
        border-radius: 10px;
        padding-left: 20px;
        padding-right: 20px;
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
                      .arg("commandButton");
  button->setStyleSheet(style);
  button->setFixedHeight(40);
  button->setContentsMargins(20, 5, 20, 5);
  addComponent(button, x, y);
}

void ValveControlView::setSvgBackground(const QString &filePath) {
  // Load the SVG image
  svgRenderer = std::make_unique<QSvgRenderer>(filePath, this);
  svgRenderer->setAspectRatioMode(Qt::AspectRatioMode::KeepAspectRatio);
  update();
}

void ValveControlView::addButtonIcon(GUI_FIELD field, float x, float y,
                                     ValveButton::Orientation orientation) {
  /*ValveButton *button = new ValveButton(orientation, this);*/

  // Create a container widget
  QWidget *valveWithTitle = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(valveWithTitle);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->setSpacing(2); // small gap between title and button

  // Create the title label
  QLabel *titleLabel = new QLabel(fieldUtil::enumToFieldName(field));
  titleLabel->setAlignment(Qt::AlignCenter);

  // Create the valve button
  ValveButton *button =
      new ValveButton(ValveButton::Horizontal, valveWithTitle);
  button->setAlignment(Qt::AlignCenter);
  button->setStyleSheet(
      QString("background: transparent; color: %1;").arg(col::primary));

  // Add them to the layout
  layout->addWidget(titleLabel, 0, Qt::AlignHCenter);
  layout->addWidget(button, 0, Qt::AlignHCenter);
  layout->setAlignment(Qt::AlignCenter);
  valveWithTitle->setStyleSheet(
      QString("background: transparent; color: %1;").arg(col::primary));

  MainWindow::clientManager->subscribe(field, [button](const QString &message) {
    if (message == "0") {
      button->setState(ValveButton::State::Close);
    } else if (message == "unknown") {
      button->setState(ValveButton::State::Unknown);
    } else {
      button->setState(ValveButton::State::Open);
    }
  });

  connect(button, &ValveButton::clicked, [button, field, this]() {
    RequestBuilder b;

    b.setHeader(RequestType::POST);
    b.addField("cmd", field);
    int value = button->getState() == ValveButton::State::Close ? 1 : 0;
    b.addField("cmd_order", value);
    MainWindow::clientManager->send(b.toString());
    b.clear();
    b.setHeader(RequestType::INTERNAL);
    b.addField(QString::number(field), "unknown");
    MainWindow::clientManager->send(b.toString());
    _logger.info(
        "Sent Valve Update",
        QString(
            R"(The valve of field %1 was clicked and the new %2 value was sent to server)")
            .arg(fieldUtil::enumToFieldName(field))
            .arg(value)
            .toStdString());
  });
  addComponent(valveWithTitle, x, y);

  // update(); // Trigger repaint to draw the new icon
}

void ValveControlView::addComponent(QWidget *component, float x, float y) {
  Position pos;
  pos.x = x;
  pos.y = y;
  componentsMap.insert(component, pos);
}

void ValveControlView::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);
  // painter.setCompositionMode(QPainter::CompositionMode_Source);
  // Ensure the background image is valid
  if (!svgRenderer || !svgRenderer->isValid())
    return;

  // Get content margins
  int marginLeft = contentsMargins().left();
  int marginRight = contentsMargins().right();
  int marginTop = contentsMargins().top();
  int marginBottom = contentsMargins().bottom();

  int availableWidth = width() - marginLeft - marginRight;
  int availableHeight = height() - marginTop - marginBottom;

  /*_logger.debug("ResizeEvent",*/
  /*              QString(R"(%1,
   * %2)").arg(width()).arg(height()).toStdString());*/
  /*_logger.debug("ResizeEvent", QString(R"(mleft=%1, mtop=%2)")*/
  /*                                 .arg(marginLeft)*/
  /*                                 .arg(marginRight)*/
  /*                                 .toStdString());*/
  // Get the intrinsic size of the SVG
  QSize svgSize = svgRenderer->defaultSize();

  // Compute the scaling factor to keep the aspect ratio
  double scaleFactor =
      qMin(static_cast<double>(availableWidth) / svgSize.width(),
           static_cast<double>(availableHeight) / svgSize.height());

  int targetWidth = svgSize.width() * scaleFactor;
  int targetHeight = svgSize.height() * scaleFactor;

  // Center the target rect within the available area
  int x = marginLeft + (availableWidth - targetWidth) / 2;
  int y = marginTop + (availableHeight - targetHeight) / 2;
  QRect targetRect(x, y, targetWidth, targetHeight);
  // Render the SVG into the available space
  svgRenderer->render(&painter, targetRect);

  for (auto it = componentsMap.begin(); it != componentsMap.end(); ++it) {
    QWidget *button = it.key();
    Position position = it.value();
    int xPos =
        targetRect.x() + targetRect.width() * position.x - button->width() / 2;
    int yPos = targetRect.y() + targetRect.height() * position.y -
               button->height() / 2;
    button->move(xPos, yPos);
  }
}
