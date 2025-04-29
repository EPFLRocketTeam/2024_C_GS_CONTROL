#include "Server.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "ClientManager.h"
#include <optional>
#include <qglobal.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <stdexcept>
#include <string>
#include <QJsonDocument>
#include <QMap>
#include <QString>
#include <QJsonObject>
#include <sqlite3.h>
#include <random>
#include <QSerialPortInfo>

#include "ERT_RF_Protocol_Interface/Protocol.h"
#include "../Capsule/src/capsule.h"
#include "FieldUtil.h"
#include "RequestAdapter.h"
#include "Setup.h"
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

    openSerialPort();
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
    if (serialPort->isOpen()) {
        serialPort->close();
    }
}

void Server::openSerialPort() {

    int ctr = 0;
    serialPort->setBaudRate(QSerialPort::Baud115200);
    QString serial_port_name = "";
    bool foundPort = false;
    #if not MANUALLY_SET_PORT 
    // First try ttyACM ports.
    if (serialPort->isOpen()) {
        return;
    }
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.portName().startsWith("ttyACM")) {
            serial_port_name =  "/dev/" + info.portName();
            _serverLogger.info(serial_port_name.toStdString(), serial_port_name.toStdString());
            serialPort->setPortName(serial_port_name);
            if (serialPort->open(QIODevice::ReadWrite)) {
                foundPort = true;
                break;
            }
        }
    }
    // If not found, try ttyS ports.
    if (!foundPort) {
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
            if (info.portName().startsWith("ttyS")) {
                serial_port_name = "/dev/" + info.portName();
                _serverLogger.debug("Serial Opening", QString(R"(name=%4, manufacturer=%1, pId=%2, vendoreId=%3, isNull=%5)")
                                .arg(info.manufacturer()).arg(info.productIdentifier()).arg(info.vendorIdentifier()).arg(serial_port_name)
                                .arg(info.isNull())
                                .toStdString());

                serialPort->setPortName(serial_port_name);
                if (serialPort->open(QIODevice::ReadWrite)) {
                    foundPort = true;
                    break;
                }
            }
        }
    }
    #else
    serial_port_name = SERIAL_USED;
    serialPort->setPortName(serial_port_name);
    if (serialPort->open(QIODevice::ReadWrite)) {
        foundPort = true;
    } else {
        _serverLogger.info("SerialActions", QString(R"(Could not open serila port %1)").arg(serial_port_name).toStdString());
    }
    #endif

    if (foundPort) {
        _serverLogger.info("SerialActions", QString("Serial port open on %1").arg(serial_port_name).toStdString());
    } else {
        serialPort->setPortName("-");
        _serverLogger.error("SerialActions", "Serial port could not open");
    }
}

void Server::receiveSerialData() {
    QByteArray data = serialPort->readAll();
    for (auto &&item: data) {
        capsule.decode(item);
    }
}

void Server::serialError() {
    _serverLogger.error("Serial Error", serialPort->errorString().toStdString());
}

void Server::incomingConnection(qintptr socketDescriptor) {
    // A new connection is incoming
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketDescriptor);

    connect(client, &QTcpSocket::readyRead, this, &Server::readyRead);
    connect(client, &QTcpSocket::disconnected, this, &Server::disconnected);

    clients.append(client);
    
    _serverLogger.info("Client Connected", QString(R"(Client on socket %1)").arg(socketDescriptor).toStdString());
}

void Server::receiveSubscribe(const QJsonObject &request,  QTcpSocket *senderSocket) {
    // Handle the subscribe request
    int field = request.value("payload").toObject().value("field").toInt();

    _serverLogger.debug("Subscription", QString(R"(The client %1 is trying to subscribe to %2)")
                           .arg((qintptr)senderSocket).arg(fieldUtil::enumToFieldName((GUI_FIELD)field)).toStdString());

    if (subscriptionMap[field].contains(senderSocket)) {
        _serverLogger.warn("Subscription failed", QString(R"(The client %1 is already subscribed to %2)")
                           .arg((qintptr)senderSocket).arg(fieldUtil::enumToFieldName((GUI_FIELD)field)).toStdString());
        return;
    }
    subscriptionMap[field].append(senderSocket);
    _serverLogger.debug("Subscription Successfull", QString(R"(The client %1 subscribed to %2)")
                           .arg((qintptr)senderSocket).arg(fieldUtil::enumToFieldName((GUI_FIELD)field)).toStdString());
}

void Server::receiveUnsubscribe(const QJsonObject &request,  QTcpSocket *senderSocket) {
    // Handle the unsubscribe request
    int field = request.value("payload").toObject().value("field").toInt();
    subscriptionMap[field].removeOne(senderSocket);
    _serverLogger.debug("Unsubscribed Successfully", QString(R"(The client %1 unsubscribed from %2)")
                           .arg((qintptr)senderSocket).arg(fieldUtil::enumToFieldName((GUI_FIELD)field)).toStdString());
}

void Server::receiveGet(const QJsonObject &request,  QTcpSocket *senderSocket) {
    // Handle the get request
    QJsonDocument doc(request);
    _serverLogger.debug("Received Get Request", doc.toJson(QJsonDocument::Indented).toStdString());
    QString field = request["field"].toString();
    QByteArray data = "Data for " + field.toUtf8();
    throw std::logic_error("Not Implemented");
    sendToAllClients(data);
}

void Server::receivePost(const QJsonObject &request,  QTcpSocket *senderSocket) {
    // Handle the post request
    QJsonDocument doc(request);
    _serverLogger.debug("Received Post Request", doc.toJson(QJsonDocument::Indented).toStdString());
    QString strJson(doc.toJson(QJsonDocument::Compact));
    if (request["payload"].toObject().contains("cmd")) {
        handleCommand(request); 
    }
}

void Server::readyRead() {
    // Handle incoming data from connected clients
    QTcpSocket *senderSocket = qobject_cast<QTcpSocket *>(sender());

    while (senderSocket->bytesAvailable() > 0) {

        QByteArray data = senderSocket->readAll();

        QString dataString = QString::fromUtf8(data);
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


int Server::sendDataToSocket(QTcpSocket * socket, RequestBuilder request) {
    QByteArray data = request.toString().toUtf8();
    _packetLogger.debug("Sent packets to client", request.toString().toStdString());
    socket->write(data, data.size());
    socket->waitForBytesWritten();
    socket->flush();
    return 0;
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
                rBuilder.addField(it.key(), QString(QJsonDocument(value.toObject()).toJson()));
                QByteArray data = rBuilder.toString().toUtf8();
                _packetLogger.debug("Sent packets to client", rBuilder.toString().toStdString());
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
                rBuilder.addField(it.key(), value.toString());
                QByteArray data = rBuilder.toString().toUtf8();
                _packetLogger.debug("Sent packets to client", rBuilder.toString().toStdString());
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
    switch (f) {
    case GUI_FIELD::GUI_CMD_SET_SERIAL_STATUS:
        if (command["payload"].toObject()["cmd_order"].toString() == "close") {
            if (serialPort->isOpen()) {
                serialPort->close();
            }
        } else {
            openSerialPort();
        }
        // Since there is no break the serial status is automatically updated

    case GUI_FIELD::SERIAL_STATUS: {
        _serverLogger.info("Received Command", "Get Serial Status Command");
        QJsonObject m_payload;
        const QString & serialStatus = QString(serialPort->isOpen() ? "open" : "close");
        m_payload[QString::number(GUI_FIELD::SERIAL_STATUS)] = serialStatus;
        _serverLogger.info("Send Response", QString(R"(The response to Serial Status is %1)").arg(QString(QJsonDocument(m_payload).toJson())).toStdString());
        updateSubscriptions(m_payload);
        }
        // Since there is no break the serial name is automatically updated
    case GUI_FIELD::SERIAL_NAME_USE: {
        QJsonObject m_payload;
        m_payload[QString::number(GUI_FIELD::SERIAL_NAME_USE)] = serialPort->portName();
        break;
        }

    default:
        int order = command["payload"].toObject()["cmd_order"].toInt();
        _serverLogger.info("Received Command", QString(R"(Send command for %1 with value %2)")
                           .arg(fieldUtil::enumToFieldName((GUI_FIELD)f)).arg(order).toStdString());
        av_uplink_t p;
        int capsule_id = createUplinkPacketFromRequest((GUI_FIELD)f, order, &p);
        sendSerialPacket(capsule_id, (uint8_t*) &p, av_uplink_size);
        break;
    }
}

void Server::sendSerialPacket(uint8_t packetId, uint8_t *packet, uint32_t size) {
    uint8_t *packetToSend = capsule.encode(packetId, packet, size);
    if (serialPort->isOpen()) {
        serialPort->write((char *) packetToSend,capsule.getCodedLen(size));
        _packetLogger.info("Serial Send", QString(R"(A packet with packet id %1 was sent to radio board)").arg(packetId).toStdString());
    } else {
        _serverLogger.error("Serial Send", "The serial port is not opened, packet couldn't be send");
    }
}



void Server::handleSerialPacket(uint8_t packetId, uint8_t *dataIn, uint32_t len) {
    std::optional<QJsonObject> result = parse_packet(packetId, dataIn, len);
    if (result) {
        QJsonDocument doc(result.value());
        QByteArray jsonData = doc.toJson(QJsonDocument::Indented);
        _packetLogger.info("Received a packet", "");
        _packetLogger.debug("Packet transformed to json", jsonData.toStdString());
        updateSubscriptions(result.value());
    } else {
        _packetLogger.error("Parsing Error", QString(R"(The message with ID=%1 could not be parsed)").arg(packetId).toStdString());
    }
}




void Server::simulateJsonData() {
    // Create a JSON object
    /*QJsonObject jsonObj;*/
    /**/
    /*_packetLogger.info("Received a packet", "A Packet was received");*/
    /*// Add primitive data members to JSON object*/
    /*jsonObj[QString::number(GUI_FIELD::AV_STATE)] = "1000";*/
    /*jsonObj[QString::number(GUI_FIELD::AV_TIMER)] = "1";*/
    /*jsonObj[QString::number(GUI_FIELD::PACKET_NBR)] = "25";*/
    /*jsonObj[QString::number(GUI_FIELD::DOWNRANGE)] = "1013";*/
    /*jsonObj[QString::number(GUI_FIELD::MAIN_FUEL)] = "close";*/
    /*jsonObj[QString::number(GUI_FIELD::HOPPER_N2O_PRESSURE)] = "1";*/
    /*jsonObj[QString::number(GUI_FIELD::HOPPER_ETH_MAIN)] = "45";*/
    /*jsonObj[QString::number(GUI_FIELD::GSE_FILLING_PRESSURE)] = "3";*/
    /*jsonObj[QString::number(GUI_FIELD::GSE_DISCONNECT_ACTIVE)] = "1";*/
    /*jsonObj[QString::number(GUI_FIELD::HOPPER_GIMBAL_X)] = "15";*/
    /*jsonObj[QString::number(GUI_FIELD::HOPPER_GIMBAL_Y)] = "-15";*/
    /*jsonObj[QString::number(GUI_FIELD::HOPPER_ID_CONFIG)] = "1234";*/
    /**/
    /*jsonObj[QString::number(GUI_FIELD::SERIAL_STATUS)] = QString(serialPort->isOpen() ? "open" : "close");*/
    /*jsonObj["1234"] = 50;*/
    /**/
    /*// Create a sub-object for location*/
    /*QJsonObject locationObj;*/
    /*locationObj["4532"] = 45.5;*/
    /*locationObj["1123"] = 9.2;*/
    /**/
    /*// Add the sub-object to the main JSON object*/
    /*jsonObj["53252"] = locationObj;*/
    /**/
    /**/
    /*// Convert the JSON object to a string*/
    /*QString jsonString = QString(QJsonDocument(jsonObj).toJson());*/
    /**/
    /*// Send the JSON string to all connected clients*/
    /*std::cout << "Fake data sent" << std::endl;*/
    /*updateSubscriptions(jsonObj);*/
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<uint32_t> distPacketNbr(0, 1000000);
    std::uniform_real_distribution<float> distCoord(-180.0f, 180.0f); // for lon/lat
    std::uniform_real_distribution<float> distAlt(0.0f, 10000.0f);
    std::uniform_int_distribution<int> distSpeed(-20, 20);
    std::uniform_real_distribution<float> distPressure(0.0f, 100.0f);
    std::uniform_real_distribution<float> distLevel(0.0f, 100.0f);
    std::uniform_int_distribution<int> distTemp(-50, 150);
    std::uniform_real_distribution<float> distVoltage(0.0f, 50.0f);
    std::uniform_int_distribution<int> distState(0, 255);
    
    #ifdef RF_PROTOCOL_FIREHORN
    av_downlink_unpacked packet;
    packet.packet_nbr = distPacketNbr(gen);
    packet.gnss_lon = distCoord(gen);
    packet.gnss_lat = distCoord(gen);
    packet.gnss_alt = distAlt(gen);
    packet.gnss_vertical_speed = static_cast<int8_t>(distSpeed(gen));
    packet.N2_pressure = distPressure(gen);
    packet.fuel_pressure = distPressure(gen);
    packet.LOX_pressure = distPressure(gen);
    packet.fuel_level = distLevel(gen);
    packet.LOX_level = distLevel(gen);
    packet.N2_temp = static_cast<int16_t>(distTemp(gen));
    packet.LOX_temp = static_cast<int16_t>(distTemp(gen));
    packet.LOX_inj_temp = static_cast<int16_t>(distTemp(gen));
    packet.lpb_voltage = distVoltage(gen);
    packet.hpb_voltage = distVoltage(gen);
    packet.av_fc_temp = static_cast<int16_t>(distTemp(gen));
    packet.ambient_temp = static_cast<int16_t>(distTemp(gen));
    packet.engine_state = static_cast<uint8_t>(distState(gen));
    packet.av_state = static_cast<uint8_t>(distState(gen));
    packet.cam_rec = static_cast<uint8_t>(distState(gen));
    handleSerialPacket(CAPSULE_ID::AV_TELEMETRY, (uint8_t *)&packet, sizeof(packet));
    #endif

    std::uniform_int_distribution<int> distBool(0, 1);
    PacketGSE_downlink gsePacket;
    gsePacket.tankPressure    = distPressure(gen);
    gsePacket.tankTemperature = distTemp(gen);
    gsePacket.fillingPressure = distTemp(gen);
    gsePacket.status= {(uint8_t)distBool(gen), (uint8_t)distBool(gen)};
    gsePacket.disconnectActive = static_cast<bool>(distBool(gen));
    #ifdef RF_PROTOCOL_FIREHORN
    gsePacket.loadcell_raw = distTemp(gen);
    #endif
    handleSerialPacket(CAPSULE_ID::GSE_TELEMETRY, (uint8_t *)&gsePacket, sizeof(gsePacket));
}
