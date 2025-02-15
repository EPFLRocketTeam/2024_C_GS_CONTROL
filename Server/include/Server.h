#ifndef SERVER_H
#define SERVER_H

#include <qcoreapplication.h>
#include <qt6/QtNetwork/QTcpSocket>
#include <qt6/QtNetwork/QTcpServer>

#include <QtSerialPort/QSerialPort>
#include <QMap>
#include <sqlite3.h>

#include "RequestHandler.h"
#include "../Capsule/src/capsule.h"

#include "Log.h"

class Server : public QTcpServer {
    Q_OBJECT

public:
    Server(QObject *parent = nullptr);
    void sendToAllClients(const QByteArray &data);
    void handleSerialPacket(uint8_t packetId, uint8_t *dataIn, uint32_t len);
    void simulateJsonData();
    ~Server();

protected:
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    void readyRead();
    void disconnected();

private:

    int setup_db();
    ModuleLog _packetLogger = ModuleLog("PacketHandler", QCoreApplication::applicationDirPath().toStdString() + "/../Log/packets.logs");
    ModuleLog _serverLogger = ModuleLog("Server", QCoreApplication::applicationDirPath().toStdString() + "/../Log/server.logs");
    void receiveSubscribe(const QJsonObject &request,  QTcpSocket *senderSocket);
    void receiveUnsubscribe(const QJsonObject &request,  QTcpSocket *senderSocket);
    void receivePost(const QJsonObject &request,  QTcpSocket *senderSocket);
    void receiveGet(const QJsonObject &request,  QTcpSocket *senderSocket);
    void receiveSerialData();
    void serialError();
    void openSerialPort();
    void updateSubscriptions(const QJsonObject &newData);
    void sendSerialPacket(uint8_t packetId, uint8_t *packet, uint32_t size);

    void handleCommand(const QJsonObject &command);
    

    QMap<int, QList<QTcpSocket *>> subscriptionMap;
    QList<QTcpSocket *> clients;
    sqlite3 *database;
    
    QSerialPort *serialPort;
    RequestHandler requestHandler;
    Capsule<Server> capsule;
};

#endif /* SERVER_H */
