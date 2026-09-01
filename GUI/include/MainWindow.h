/**
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief Main window of the application
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cstddef>
#include <memory>

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "ClientManager.h"
#include "ControlPanelView.h"
#include "RequestBuilder.h"
#include "components/DraggableButton.h"
#include <Log.h>
#include <qwidget.h>

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr,
             QMap<std::string, QList<std::vector<GUI_FIELD>>>
                 *controlPannelMap = nullptr,
             QWidget *leftWidget = nullptr, QWidget *middleWidget = nullptr,
             QWidget *rightWidget = nullptr);
  
  ~MainWindow() override = default;

  inline static std::unique_ptr<ClientManager> clientManager;

  void initiateLaunchTimer();


private slots:
  void updateLaunchTimer();

private:
  void UpdateLaunchTimerState(const QString& av_state);
  void startLaunchTimer();

  QWidget *leftSection;
  QWidget *middleSection;
  QWidget *rightSection;

  ModuleLog _logger = ModuleLog("MainWindow", LOG_FILE_PATH);
  DraggableButton *pannelButton;
  ControlPanelView *panelSection;
  QHBoxLayout *createSectionsLayout();
  QRect segmentSize;
  // Launch timer components
  QLabel *launchTimerLabel;
  QTimer *launchTimer;
  double launchTimerValue;
  int launchInitiated;
  int launchTimerStarted;
};

#endif /* MAINWINDOW_H */
