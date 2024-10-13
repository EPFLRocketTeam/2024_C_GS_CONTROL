
/** 
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief Main function for the GUI
*/
#include <iostream>

#include <QApplication>
#include <QResource>

#include "MainWindow.h"
#include "ClientManager.h"
#include "Setup.h"
#include <QTimer>
#include <QJsonDocument>
#include <QtNetwork/QTcpSocket>

// This variable must specify the path to 
#define RELATIVE_PATH_TO_RES_FROM_BUILD_FOLD "../GUI/res"

void fakeDataHandling();

int start_client(int argc, char *argv[]) {
QApplication app(argc, argv);
    QResource::registerResource(RELATIVE_PATH_TO_RES_FROM_BUILD_FOLD "/resources.rcc");


    MainWindow::clientManager = std::make_unique<ClientManager>(nullptr, network::serverIP, network::serverPort);    
    MainWindow mainWindow;
    
    mainWindow.show();
    return app.exec();
}


int main(int argc, char *argv[]) {
    return start_client(argc, argv);
}
