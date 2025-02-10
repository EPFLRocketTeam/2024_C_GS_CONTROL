
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
#include <iostream>
#include <filesystem>
#include <QDir>


void fakeDataHandling();

int start_client(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QString appDir = QCoreApplication::applicationDirPath();
    QString resPath = QDir(appDir).absoluteFilePath("../GUI/res/resources.rcc");
    QResource::registerResource(resPath);
    std::cout << "Current working directory: " << appDir.toStdString() << std::endl;
    auth::loadKeyFromFile(appDir + "/../GUI/src/.key");
    std::cout << "KEY: " <<
        auth::key.toStdString() << std::endl;
    RequestBuilder::authorizationKey = auth::key;

    MainWindow::clientManager = std::make_unique<ClientManager>(nullptr, network::serverIP, network::serverPort);    
    MainWindow mainWindow;
    
    mainWindow.show();
    return app.exec();
}


int main(int argc, char *argv[]) {
    return start_client(argc, argv);
}
