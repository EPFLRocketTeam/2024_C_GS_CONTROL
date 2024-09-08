
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
#include "RequestBuilder.h"

// This variable must specify the relative path of the resources folder from the build one
#define RELATIVE_PATH_TO_RES_FROM_BUILD_FOLD "../GUI/res"

void fakeDataHandling();

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QResource::registerResource(RELATIVE_PATH_TO_RES_FROM_BUILD_FOLD "/resources.rcc");


    MainWindow::clientManager = std::make_unique<ClientManager>();    
    MainWindow mainWindow;
    
    mainWindow.show();
    return app.exec();
}
