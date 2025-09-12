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
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "ClientManager.h"
#include "ControlPannelView.h"
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

  void startLaunchTimer();

protected:
  void resizeEvent(QResizeEvent *event) override;

private slots:
  void updateLaunchTimer();

private:
  void replacePannelButton();
  void buttonGrabbed(QEvent *event);

  QWidget *leftSection;
  QWidget *middleSection;
  QWidget *rightSection;

  ModuleLog _logger = ModuleLog("MainWindow", LOG_FILE_PATH);
  DraggableButton *pannelButton;
  ControlPannelView *pannelSection;
  QHBoxLayout *createSectionsLayout();

  // Launch timer components
  QLabel *launchTimerLabel;
  QTimer *launchTimer;
  double launchTimerValue;
};

#endif /* MAINWINDOW_H */
