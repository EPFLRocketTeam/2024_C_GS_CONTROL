/**
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief Main function for the GUI
*/
#include <QApplication>
#include <QResource>

#include "ClientManager.h"
#include "FileLocation.h"
#include "GSEWindow.h"
#include "MainWindow.h"
#include "RequestBuilder.h"
#include "Setup.h"
#include <QDir>
#include <QJsonDocument>
#include <QTimer>
#include <QtNetwork/QTcpSocket>

#include <Log.h>
#include <cstdio>

void fakeDataHandling();

int start_client(int argc, char *argv[]) {
  QApplication app(argc, argv);
  ModuleLog logger = ModuleLog("App Launcher", LOG_FILE_PATH);
#if DEBUG_LOG
  MainLog::setDebugLevel(DEBUG);
#endif
  Q_INIT_RESOURCE(resources);

  QString project_root_path = PROJECT_ROOT_PATH;
  auth::loadKeyFromFile(project_root_path + "/GUI/src/.key");
  RequestBuilder::authorizationKey = auth::key;
  logger.info("Load Authentication Keys", "A Key was found");

  MainWindow::clientManager = std::make_unique<ClientManager>(
      nullptr, network::serverIP, network::serverPort);

  ui_elements::init_views();

  MainWindow mainWindow(
      nullptr, &ui_elements::controlMap, ui_elements::leftPlaceholder,
      ui_elements::middlePlaceholder, ui_elements::rightPlaceholder);

  mainWindow.setWindowIcon(QIcon(":/icons/firehorn.ico"));

  mainWindow.show();
  #if FIREHORN_GUI
  MainWindow gseWindow(nullptr, &ui_elements::gseControlMap, nullptr, ui_elements::gseMiddlePlaceholder, nullptr);
  gseWindow.show();
  #endif

  return app.exec();
}

int main(int argc, char *argv[]) { return start_client(argc, argv); }
