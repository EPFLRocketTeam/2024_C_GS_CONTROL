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
#include <QMainWindow>
#include <QPushButton>
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
             QMap<std::string, QMap<std::string, std::vector<GUI_FIELD>>>
                 *controlPannelMap = nullptr,
             QWidget *leftWidget = nullptr, QWidget *middleWidget = nullptr,
             QWidget *rightWidget = nullptr);
  virtual ~MainWindow() {};
  inline static std::unique_ptr<ClientManager> clientManager;

protected:
  void resizeEvent(QResizeEvent *event) override;

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
};

#endif /* MAINWINDOW_H */
