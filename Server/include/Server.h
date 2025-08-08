#ifndef SERVER_H
#define SERVER_H

#include <qcoreapplication.h>
#include <qt6/QtNetwork/QTcpSocket>
#include <qt6/QtNetwork/QTcpServer>

#include <QtSerialPort/QSerialPort>
#include <QMap>
#include <sqlite3.h>

#include "RequestBuilder.h"
#include "RequestHandler.h"
#include "data_storage.h"
#include "../Capsule/src/capsule.h"

#include "Log.h"

class Server : public QTcpServer {
    Q_OBJECT

    friend class BaseIntegrationTest;
    friend class DataLabelTest;
    friend class FullIntegrationTest;
    friend class ToggleButtonTest;
    friend class CommandButtonTest;
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
    ModuleLog _packetLogger = ModuleLog("PacketHandler",  "../Log/packets.logs");
    ModuleLog _serverLogger = ModuleLog("Server",  "../Log/server.logs");
    void receiveSubscribe(const QJsonObject &request,  QTcpSocket *senderSocket);
    void receiveUnsubscribe(const QJsonObject &request,  QTcpSocket *senderSocket);
    void receivePost(const QJsonObject &request,  QTcpSocket *senderSocket);
    void receiveGet(const QJsonObject &request,  QTcpSocket *senderSocket);
    void receiveSerialData();
    void serialError();
    void openSerialPort();
    void updateSubscriptions(const QJsonObject &newData);
    void sendSerialPacket(uint8_t packetId, uint8_t *packet, uint32_t size);

    int sendDataToSocket(QTcpSocket * socket, RequestBuilder request);
    void handleCommand(const QJsonObject &command);
    void abort_loop();
    int abort_initiated = 0;
    

    QMap<int, QList<QTcpSocket *>> subscriptionMap;
    QList<QTcpSocket *> clients;
    sqlite3 *database;
    SqliteDB *sqlDatabase;
    
    
    QSerialPort *serialPort;
    RequestHandler requestHandler;
    Capsule<Server> capsule;
};

#endif /* SERVER_H */
