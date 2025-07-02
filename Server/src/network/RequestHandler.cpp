#include <iostream>

#include "RequestHandler.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "../ServerSetup.h"

RequestHandler::RequestHandler(QObject *parent) : QObject(parent) {}

void RequestHandler::handleRequest(const QString &request, QTcpSocket* senderSocket)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(request.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
        std::cout << "Error parsing JSON: " << error.errorString().toStdString() << std::endl;
        // std::cout << "Error parsing JSON: " << request.toStdString() << std::endl;
    }
    QJsonObject json = doc.object();

    std::cout << "Error parsing JSON: " << "before" << std::endl;
    int found = 0;
    int readBit = 0;
    int writeBit = 0;
    for (auth_server::AuthKey key : auth_server::validKeys) {
        std::cout << "Error parsing JSON: " << "in" << std::endl;
        if (key.key == json["Authorization"].toString()) {
            found = 1;
            readBit = (key.accessRight >> 1) & 0x1;
            writeBit = key.accessRight & 0x1;
            break;
        }
    }

    if (found != 1) {
        return;
    }
    

    QString header = json["header"].toString();

        std::cout << "Error parsing JSON: " << header.toStdString() << std::endl;
    // Emit the corresponding signal based on the header type
    if (header == "subscribe" && readBit)
        emit subscribe(json, senderSocket);
    else if (header == "unsubscribe" && readBit) 
        emit unsubscribe(json, senderSocket);
    else if (header == "get" && readBit)
        emit get(json, senderSocket);
    else if (header == "post" && readBit && writeBit)
        emit post(json, senderSocket);
    else 
        std::cout << "Unknown header type: " << header.toStdString() << " Or illeage acces right, with R="
            << readBit << " And W=" << writeBit << std::endl;
    // Add more conditions for other types if needed
}
