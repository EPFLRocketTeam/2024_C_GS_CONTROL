
/** 
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief Main function for the GUI
*/
#include <QApplication>
#include <QResource>

#include "MainWindow.h"
#include "ClientManager.h"
#include "RequestBuilder.h"
#include "Setup.h"
#include <QTimer>
#include <QJsonDocument>
#include <QtNetwork/QTcpSocket>
#include <QDir>
#include "FileLocation.h"

#include <Log.h>

void fakeDataHandling();

int start_client(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QString appDir = QCoreApplication::applicationDirPath();
    ModuleLog logger = ModuleLog("App Launcher", LOG_FILE_PATH);

    QString resPath = QDir(appDir).absoluteFilePath("../GUI/res/resources.rcc");
    if (QResource::registerResource(resPath))
        logger.info("Load Resources", "The resources were loaded");
    else 
        logger.error("Load Resources", "Couldn't register the resources");

    auth::loadKeyFromFile(appDir + "/../GUI/src/.key");
    RequestBuilder::authorizationKey = auth::key;
    logger.info("Load Authentication Keys", "A Key was found");

    MainWindow::clientManager = std::make_unique<ClientManager>(nullptr, network::serverIP, network::serverPort);    
    MainWindow mainWindow;

    mainWindow.show();
    return app.exec();
}


int main(int argc, char *argv[]) {
    return start_client(argc, argv);
}
