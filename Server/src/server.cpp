#include "Server.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "ClientManager.h"
#include <optional>
#include <qglobal.h>
#include <qjsonobject.h>
#include <string>
#include <QJsonDocument>
#include <QMap>
#include <QString>
#include <QJsonObject>
#include <sqlite3.h>


#include "../Capsule/src/capsule.h"
#include "FieldUtil.h"
#include "RequestAdapter.h"
#include "packet_helper.h"
#include <RequestBuilder.h>

#define DB_ERROR_MESSAGE_PREFIX "The Databse couldn't be open, and produce the following error message"

Server::Server(QObject *parent) : QTcpServer(parent), requestHandler(this), serialPort(new QSerialPort(this)), capsule(&Server::handleSerialPacket, this) {
    
    connect(&requestHandler, &RequestHandler::subscribe, this, &Server::receiveSubscribe);
    connect(&requestHandler, &RequestHandler::unsubscribe, this, &Server::receiveUnsubscribe);
    connect(&requestHandler, &RequestHandler::get, this, &Server::receiveGet);
    connect(&requestHandler, &RequestHandler::post, this, &Server::receivePost);
    connect(serialPort, &QSerialPort::readyRead, this, &Server::receiveSerialData);
    connect(serialPort, &QSerialPort::errorOccurred, this, &Server::serialError);

    setup_db();

    /*openSerialPort();*/
}


int Server::setup_db() {
    QString db_path = (QCoreApplication::applicationDirPath() + "/../data.db");
    int exit = sqlite3_open(db_path.toStdString().c_str(), &database);
    /*sqlite3* DB; */
    /*int exit = 0; */
    /*int res = sqlite3_open("example.db", &DB); */

    if (exit) {
        const char* db_errmsg = sqlite3_errmsg(database);
        size_t m_length = 100 + strnlen(db_errmsg, 2000);
        char* error_message = (char *)calloc(m_length, sizeof(char));
        snprintf(error_message, m_length, "%s: %s", DB_ERROR_MESSAGE_PREFIX, db_errmsg);
        _serverLogger.warn("Databse Opening", error_message);
        free(error_message);
        database = NULL;
        return 0;
    }
    
    QString success_message = QString::fromStdString("The Databse was opened succesfuly in the file ") + db_path;
    _serverLogger.info("Databse Opening", success_message.toStdString());
    return 1;
}


Server::~Server() {
    if (database != NULL) {
        sqlite3_close(database);
    }
}

void Server::openSerialPort() {
    int ctr = 0;
    serialPort->setBaudRate(QSerialPort::Baud115200);
    QString serial_port_name = "";
    if (!serialPort->isOpen()) {
        do {
            serial_port_name = "ttyACM" + QString::number(ctr++);
            serialPort->setPortName(serial_port_name);
        } while (!serialPort->open(QIODevice::ReadWrite) && ctr <= 50);
        if (!serialPort->isOpen()) { // opening on WSL => ttyS
            ctr = 0;
            do {
                serial_port_name = "ttyS" + QString::number(ctr++);
                serialPort->setPortName(serial_port_name);
            } while (!serialPort->open(QIODevice::ReadWrite) && ctr <= 50);
        }
        if (serialPort->isOpen()) {
            std::cout << "Serial port open" << std::endl;
        } else {
            std::cout << "Impossible to find valid serialPort port" << std::endl;
        }
    }

}

void Server::receiveSerialData() {
    QByteArray data = serialPort->readAll();
    for (auto &&item: data) {
        capsule.decode(item);
    }
}

void Server::serialError() {
    std::cout << "Serial port error: " << serialPort->errorString().toStdString() << std::endl;
}

void Server::incomingConnection(qintptr socketDescriptor) {
    // A new connection is incoming
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketDescriptor);

    connect(client, &QTcpSocket::readyRead, this, &Server::readyRead);
    connect(client, &QTcpSocket::disconnected, this, &Server::disconnected);

    clients.append(client);
    
    std::cout << "Client connected: " << socketDescriptor << std::endl;
}

void Server::receiveSubscribe(const QJsonObject &request,  QTcpSocket *senderSocket) {
    // Handle the subscribe request
    int field = request.value("payload").toObject().value("field").toInt();

    std::cout << "socket: " << senderSocket << std::endl;
    if (subscriptionMap[field].contains(senderSocket)) {
        std::cout << "Already subscribed to: " << QString::number(field).toStdString() << std::endl;
        return;
    }
    subscriptionMap[field].append(senderSocket);
    std::cout << "Subscribed to: " << QString::number(field).toStdString() << std::endl; 
}

void Server::receiveUnsubscribe(const QJsonObject &request,  QTcpSocket *senderSocket) {
    // Handle the unsubscribe request
    int field = request.value("payload").toObject().value("field").toInt();
    subscriptionMap[field].removeOne(senderSocket);
    std::cout << "Unsubscribed from: " << subscriptionMap[field].size() << std::endl;
}

void Server::receiveGet(const QJsonObject &request,  QTcpSocket *senderSocket) {
    // Handle the get request
    std::cout << "Received get request" << std::endl;
    QString field = request["field"].toString();
    QByteArray data = "Data for " + field.toUtf8();
    sendToAllClients(data);
}

void Server::receivePost(const QJsonObject &request,  QTcpSocket *senderSocket) {
    // Handle the post request
    std::cout << "Received post request" << std::endl;
    QJsonDocument doc(request);
    QString strJson(doc.toJson(QJsonDocument::Compact));
    if (request["payload"].toObject().contains("cmd")) {
        handleCommand(request); 
    }
    std::cout << strJson.toStdString() << std::endl;
}

void Server::readyRead() {
    // Handle incoming data from connected clients
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket *>(sender());

    while (senderSocket->bytesAvailable() > 0) {

        QByteArray data = senderSocket->readAll();

        QString dataString = QString::fromUtf8(data);
        std::cout << "\033[32m" << dataString.toStdString() << "\033[0m" << std::endl;
        QString jsonString(dataString);
        // jsonString.remove("\n");
        // Split the string by '}{'
        QString sanitizedStr = jsonString.remove("\n");

        QStringList jsonStrings = sanitizedStr.split("}{");
        int counter = 0;
        for (QString &jsonStr : jsonStrings) {
            if (counter == 0 && jsonStrings.length() > 1) {
                jsonStr.append("}");
            } else if (jsonStrings.length() - 1 == counter && jsonStrings.length() > 1){
                jsonStr.prepend("{");
            } else if (jsonStrings.length() > 1) {
                jsonStr.prepend("{");
                jsonStr.append("}");
            }
            counter++;
            
            std::cout << jsonStr.toStdString() << std::endl;
            std::cout << " " << std::endl;

            requestHandler.handleRequest(jsonStr, senderSocket);
            
        }

    }
}


void Server::disconnected() {
    // Remove disconnected clients
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket *>(sender());
    clients.removeOne(senderSocket);
    foreach(const int &key, subscriptionMap.keys()) {
        QList<QTcpSocket *> &socketList = subscriptionMap[key];
        socketList.removeOne(senderSocket);
    }
    qDebug() << "Client disconnected: " << senderSocket->socketDescriptor();
    senderSocket->deleteLater();
}

void Server::sendToAllClients(const QByteArray &data) {
    for (QTcpSocket *client : clients) {
        client->write(data);
        client->flush();
    }
}


void Server::updateSubscriptions(const QJsonObject &newData) {
    

    for (auto it = newData.constBegin(); it != newData.constEnd(); ++it) {
        QRegularExpression regex("^-?\\d+$"); // Regular expression to match integers, including negative ones
        QRegularExpressionMatch match = regex.match(it.key());
        const QList<QTcpSocket *> sockets = subscriptionMap.value(it.key().toInt());  

        const QJsonValue& value = it.value();
        RequestBuilder rBuilder;
        rBuilder.setHeader(RequestType::POST); 

        if (value.isObject()) {
            updateSubscriptions(value.toObject());
            for (QTcpSocket *socket: sockets) {
                std::cout << "Sending data to client" << std::endl;
                rBuilder.addField(it.key(), QString(QJsonDocument(value.toObject()).toJson()));
                QByteArray data = rBuilder.toString().toUtf8();
                socket->write(data, data.size());
                socket->waitForBytesWritten();
                socket->flush();
                
            }
        } else if (value.isArray()) {
            const QJsonArray array = value.toArray();
            for (const QJsonValue& element : array) {
                if (element.isObject()) {
                    updateSubscriptions(element.toObject());
                } 
            }
        } else {
            for (QTcpSocket *socket: sockets) {
                std::cout << "Sending data to client" << std::endl;
                rBuilder.addField(it.key(), value.toString());
                QByteArray data = rBuilder.toString().toUtf8();
                socket->write(data, data.size());
                socket->waitForBytesWritten();
                socket->flush();
                
                
                
            }
        }
    }


}

void Server::handleCommand(const QJsonObject &command) {
    // Handle the command
    int f = command["payload"].toObject()["cmd"].toInt();
    RequestBuilder b = RequestBuilder();
    switch (f)
    {

    case GUI_FIELD::GUI_CMD_SET_SERIAL_STATUS:
        if (command["payload"].toObject()["cmd_order"].toString() == "close") {
            if (serialPort->isOpen()) {
                serialPort->close();
            }
        } else {
            openSerialPort();
        }
    case GUI_FIELD::SERIAL_STATUS: {
        std::cout << "Serial status" << std::endl;
        b.setHeader(RequestType::POST);
        const QString & serialStatus = QString(serialPort->isOpen() ? "open" : "close");
        b.addField(QString::number(GUI_FIELD::SERIAL_STATUS), serialStatus);
        std::cout << b.toString().toStdString() << std::endl;   
        updateSubscriptions(b.build());
        break;
        }

    case GUI_FIELD::SERIAL_NAME_USE:
        b.setHeader(RequestType::POST);
        b.addField(QString::number(GUI_FIELD::SERIAL_NAME_USE), serialPort->portName());
        updateSubscriptions(b.build());
        break;

        

    default:
        int order = command["payload"].toObject()["cmd_order"].toInt();
        std::cout << "cmd: " << f << " order: " << order << std::endl;
        av_uplink_t* p = createUplinkPacketFromRequest((GUI_FIELD)f, order);
        sendSerialPacket(CAPSULE_ID::GS_CMD, (uint8_t*) p, av_uplink_size);
        break;
    }
    
}

void Server::sendSerialPacket(uint8_t packetId, uint8_t *packet, uint32_t size) {
    uint8_t *packetToSend = capsule.encode(packetId, packet, size);
    if (serialPort->isOpen()) {
        serialPort->write((char *) packetToSend,capsule.getCodedLen(size));
        std::cout << "Packet sent to radio Board" << std::endl;
        _packetLogger.info("Packet Sent", "A packet was sent");
    } else {
        _serverLogger.error("Serial Send", "The serial port is not opened, packet couldn't be send");
    }
}



void Server::handleSerialPacket(uint8_t packetId, uint8_t *dataIn, uint32_t len) {
    //    packet_ctr++;
    static int altitude_max = 0;
    static int altitude_max_r = 0;
    std::optional<const QJsonObject> result = parse_packet(packetId, dataIn, len);
    _packetLogger.info("Received a packet", "# TODO");
    if (result) {
        updateSubscriptions(result.value());
    } else {
        _packetLogger.error("Parsing Error", QString(R"(The message with ID=%1 could not be parsed)").arg(packetId).toStdString());
    }
}




void Server::simulateJsonData() {
    // Create a JSON object
    QJsonObject jsonObj;

    _packetLogger.info("Received a packet", "A Packet was received");
    // Add primitive data members to JSON object
    jsonObj[QString::number(GUI_FIELD::AV_STATE)] = "1000";
    jsonObj[QString::number(GUI_FIELD::AV_TIMER)] = "1";
    jsonObj[QString::number(GUI_FIELD::PACKET_NBR)] = "25";
    jsonObj[QString::number(GUI_FIELD::DOWNRANGE)] = "1013";
    jsonObj[QString::number(GUI_FIELD::MAIN_FUEL)] = "close";
    jsonObj[QString::number(GUI_FIELD::HOPPER_N2O_PRESSURE)] = "1";
    jsonObj[QString::number(GUI_FIELD::HOPPER_ETH_MAIN)] = "45";
    jsonObj[QString::number(GUI_FIELD::GSE_FILLING_PRESSURE)] = "3";
    jsonObj[QString::number(GUI_FIELD::GSE_DISCONNECT_ACTIVE)] = "1";

    jsonObj[QString::number(GUI_FIELD::SERIAL_STATUS)] = QString(serialPort->isOpen() ? "open" : "close");
    jsonObj["1234"] = 50;

    // Create a sub-object for location
    QJsonObject locationObj;
    locationObj["4532"] = 45.5;
    locationObj["1123"] = 9.2;

    // Add the sub-object to the main JSON object
    jsonObj["53252"] = locationObj;


    // Convert the JSON object to a string
    QString jsonString = QString(QJsonDocument(jsonObj).toJson());

    // Send the JSON string to all connected clients
    std::cout << "Fake data sent" << std::endl;
    updateSubscriptions(jsonObj);
}
