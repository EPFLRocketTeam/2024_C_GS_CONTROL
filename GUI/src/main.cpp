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
    ModuleLog logger = ModuleLog("App Launcher", LOG_FILE_PATH);
    #if DEBUG_LOG
    MainLog::setDebugLevel(DEBUG);
    #endif
    Q_INIT_RESOURCE(resources);

    QString project_root_path = PROJECT_ROOT_PATH;
    auth::loadKeyFromFile(project_root_path + "/GUI/src/.key");
    RequestBuilder::authorizationKey = auth::key;
    logger.info("Load Authentication Keys", "A Key was found");

    MainWindow::clientManager = std::make_unique<ClientManager>(nullptr, network::serverIP, network::serverPort);    
    MainWindow mainWindow;
    mainWindow.setWindowIcon(QIcon(":/icons/firehorn.ico"));

    mainWindow.show();
    return app.exec();
}


int main(int argc, char *argv[]) {
    return start_client(argc, argv);
}
