#include <iostream>

#include <QCoreApplication>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QTimer>

#include "Server.h"
#include "Setup.h"

int start_server(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    ModuleLog logger = ModuleLog("Server Launcher",  "../Log/server.logs");
    #if DEBUG_LOG
    MainLog::setDebugLevel(DEBUG);
    #endif
    QString appDir = QCoreApplication::applicationDirPath();
    auth::loadKeysFromFile(appDir + "/../Server/src/auth_keys.json"); 
    Server server = Server();
    int port(12345);
    std::cout << "Server started" << std::endl;
    if (server.listen(QHostAddress::Any, port)) {
        logger.info("Server Start", QString(R"(Server Listening on port %1)").arg(port).toStdString());
    } else {
        logger.error("Server Start Error", "Failed to start server. Check if the port is already in use or permissions issue.");
    }    

    #if SIMULATE_PACKETS
    QTimer *timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, &server, &Server::simulateJsonData);
    timer->start(2000); // Timer fires every 2000 milliseconds (2 seconds)
    #endif

    return a.exec();
}

int main(int argc, char *argv[]) {
    return start_server(argc, argv);
}
