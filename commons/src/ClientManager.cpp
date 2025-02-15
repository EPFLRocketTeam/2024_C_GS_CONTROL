/**
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief Client side of data stream handling
*/
#include <iostream>

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <ostream>
#include <qt6/QtCore/qtimer.h>
#include <string>
#include <unistd.h>
// #include "../../Setup.h"
#include "ClientManager.h"
#include "QTimer"
#include "RequestBuilder.h"
#include "Log.h"
#include "FieldUtil.h"

ClientManager::ClientManager(QObject *parent, QString host, int port)
    : ClientInterface(parent) {

  socket = new QTcpSocket(this);
  serverHost = host;
  serverPort = port;
  m_reconnectTimer = new QTimer(this);
  m_reconnectTimer->setInterval(1000);    // 1 second
  m_reconnectTimer->setSingleShot(false); // Repeat indefinitely
  connect(m_reconnectTimer, &QTimer::timeout, [this]() {
    if (socket->state() == QAbstractSocket::UnconnectedState) {
      _logger.info("Connection", "Attempting to reconnect");
      socket->abort(); // Reset the socket
      socket->connectToHost(serverHost, serverPort);
    }
  });
  connect(socket, &QTcpSocket::connected, this, &ClientManager::connected);
  connect(socket, &QTcpSocket::readyRead, this, &ClientManager::readyRead);
  connect(socket, &QTcpSocket::disconnected, this,
          &ClientManager::disconnected);

  socket->connectToHost(serverHost, serverPort);
  if (!socket->waitForConnected(3000)) {
    _logger.error("Connection", "Could not connect with the following error:\n" + socket->errorString().toStdString());
  }
}

void ClientManager::connected() {
  _logger.info("Connection", "The client is connected to the server");
  if (m_reconnectTimer->isActive()) {
    m_reconnectTimer->stop();
    for (auto key : subscriptionsJson.keys()) {
      sendSubscribeRequest(key);
    }
    for (auto key : subscriptionsStrings.keys()) {
      sendSubscribeRequest(key);
    }
  }
}

void ClientManager::disconnected() {
  _logger.error("Connection", "You were disconnected from the server, the progrma will try to reconnect as soon as possible");
  if (!m_reconnectTimer->isActive())
    m_reconnectTimer
        ->start(); // Timer fires every 2000 milliseconds (2 seconds)
}

void ClientManager::readyRead() {
  // Handle incoming data from the server

  QByteArray data = socket->readAll();
  handleReceivedData(QString::fromUtf8(data));
}

void ClientManager::subscribe(const GUI_FIELD field, CallbackFunction<QString> callback) {
  _logger.debug("Subscription", QString(R"(An UI element subscribed to %1)").
                arg(fieldUtil::enumToFieldName(field)).toStdString());
  if (subscriptionsStrings[field].size() == 0)
    sendSubscribeRequest(field);
  // sendSubscribeRequest(field);
  subscriptionsStrings[field].append(callback);
}

void ClientManager::sendSubscribeRequest(const QString &field) {

  if (socket->state() == QAbstractSocket::UnconnectedState) {
    if (!m_reconnectTimer->isActive()) {
      m_reconnectTimer->start();
      _logger.error("Connection", "Could not connect with the following error:\n" + socket->errorString().toStdString());
         }
    return;
  }
  RequestBuilder builder;
  builder.setHeader(RequestType::SUBSCRIBE);
  builder.addField("field", field);
  // socket->waitForReadyRead();
  socket->write(builder.toString().toUtf8());
  
  _logger.info("Subscription", "Send subscription to " + field.toStdString());
  socket->waitForBytesWritten();

  socket->flush();
}

void ClientManager::subscribe(const GUI_FIELD field,
                              CallbackFunction<QJsonValue> callback) {
  _logger.debug("Subscription", QString(R"(An UI element subscribed to %1)").
                arg(fieldUtil::enumToFieldName(field)).toStdString());
  if (subscriptionsJson[field].size() == 0)
    sendSubscribeRequest(field);
  subscriptionsJson[field].append(callback);
}

void ClientManager::sendSubscribeRequest(const GUI_FIELD field) {
  if (socket->state() == QAbstractSocket::UnconnectedState) {
    if (!m_reconnectTimer->isActive()) {
      m_reconnectTimer->start();
      QString error_msg = QString::fromStdString("The connection with the server failed with the following error: ") + socket->errorString();
      _logger.error("ConnectionFailed", error_msg.toStdString());
    }
    return;
  }
  RequestBuilder builder;
  builder.setHeader(RequestType::SUBSCRIBE);
  builder.addField("field", field);
  socket->write(builder.toString().toUtf8());
  
  _logger.debug("Subscription", "subscribed to " + std::to_string(field));
    /*std::cout << "Try to subscribe " << socket->isValid() << socket->state() << std::endl;*/

  socket->waitForBytesWritten();
  socket->flush();
}

QString removeExtraCurlyBrackets(const QString &jsonString) {
  QString sanitizedString = jsonString.trimmed();

  // Remove leading extra curly brackets
  while (sanitizedString.startsWith('{')) {
    sanitizedString.remove(0, 1);
    sanitizedString = sanitizedString.trimmed();
  }

  // Remove trailing extra curly brackets
  while (sanitizedString.endsWith('}')) {
    sanitizedString.chop(1);
    sanitizedString = sanitizedString.trimmed();
  }

  return sanitizedString;
}

void ClientManager::handleReceivedData(const QString &data) {
  QString jsonString(data);
  _logger.debug("Received Data", data.toStdString());
  // Split the string by '}{'
  QStringList jsonStrings = jsonString.split("}{");
  if (jsonStrings.size() == 1) {
    // If there's only one JSON string, it's the only one to process
    jsonStrings = jsonString.split("}\n{");
  } 

  // Process each individual JSON string
  for (const QString &jsonStr : jsonStrings) {
    // Add '}' to the end of each string if it's not the last one
    QString json = jsonStr;

    json = removeExtraCurlyBrackets(json);
    if (json != jsonStrings.first()) {
      json.prepend("{");
    }

    if (json != jsonStrings.last()) {
      json.append("}");
      json.append("}");
    }

    // Parse JSON string to JSON object
    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json.toUtf8(), &error);
    if (error.error != QJsonParseError::NoError) {
      _logger.error("Handle Data", "An error occured while parsing the json");
      continue;
    }

    // Extract payload from JSON object
    QJsonObject payload = jsonDoc.object()["payload"].toObject();
    notifyChildrenFields(payload);
  }

  // notifyChildrenFields(jsonFromString(data).value("payload").toObject());
}

QJsonObject ClientManager::jsonFromString(const QString &data) {
  QJsonParseError error;
  QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8(), &error);
  if (error.error != QJsonParseError::NoError) {
      _logger.error("Handle Data", "The following error occured while parsing the json:\n" + error.errorString().toStdString());
    return QJsonObject();
  }
  return doc.object();
}

void ClientManager::notifyChildrenFields(const QJsonObject &localObject) {
  for (auto it = localObject.constBegin(); it != localObject.constEnd(); ++it) {
    const QVector<CallbackFunction<QString>> &callbacksStrings =
        subscriptionsStrings.value((GUI_FIELD)it.key().toInt());
    const QVector<CallbackFunction<QJsonValue>> &callbacksJson =
        subscriptionsJson.value((GUI_FIELD)it.key().toInt());

    const QJsonValue &value = it.value();
    for (const auto &callback : callbacksJson) {
      callback(value);
    }

    if (value.isObject()) {
      notifyChildrenFields(value.toObject());
    } else if (value.isArray()) {
      const QJsonArray array = value.toArray();
      for (const QJsonValue &element : array) {
        if (element.isObject()) {
          notifyChildrenFields(element.toObject());
        }
      }
    } else {

      for (const auto &callback : callbacksStrings) {
        callback(value.toVariant().toString());
      }
    }
  }
}

void ClientManager::send(const QString &data) {
  // send command "serialNameUsed", "serialStatus"
  if (socket->state() == QAbstractSocket::UnconnectedState) {
    if (!m_reconnectTimer->isActive()) {
      m_reconnectTimer->start();
      _logger.error("Connection", "Could not connect with the following error:\n" + socket->errorString().toStdString());
      std::cout << "Error: " << socket->errorString().toStdString()
                << std::endl;
    }
    return;
  }
  QJsonObject json = jsonFromString(data);
  if (json.value("header").toString() == "internal") {
    handleReceivedData(data);
  } else {
    socket->write(data.toUtf8());
    socket->waitForBytesWritten();
  }
}
