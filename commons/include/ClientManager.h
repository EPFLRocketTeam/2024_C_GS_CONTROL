/**
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief Client side of data stream handling
*/

#ifndef CLIENTIMPLEMENTATION_H
#define CLIENTIMPLEMENTATION_H

#include <iostream>
#include <qt6/QtCore/qtimer.h>
#include <string>

#include <QJsonObject>
#include <QMap>
#include <QVector>
#include <QtNetwork/QTcpSocket>

#include "ClientInterface.h"
#include "FieldUtil.h"
#include "Log.h"

class ClientManager : public ClientInterface {
  Q_OBJECT
public:
  ClientManager(QObject *parent = nullptr, QString host = QString("127.0.0.1"),
                int port = 8080);

  void subscribe(const GUI_FIELD field,
                 CallbackFunction<QString> callback) override;
  void subscribe(const GUI_FIELD field,
                 CallbackFunction<QJsonValue> callback) override;
  void handleReceivedData(const QString &data) override;
  void send(const QString &data) override;
  void sendSubscribeRequest(const QString &field);
  QString serverHost;
  int serverPort;

  void unsubscribeAll(const GUI_FIELD field);

private slots:
  void connected();

  void readyRead();

  void disconnected();

  void sendSubscribeRequest(const GUI_FIELD field);
  void sendUnsubscribeRequest(const GUI_FIELD field);

private:
  QMap<GUI_FIELD, QVector<CallbackFunction<QString>>> subscriptionsStrings;
  QMap<GUI_FIELD, QVector<CallbackFunction<QJsonValue>>> subscriptionsJson;

  ModuleLog _logger = ModuleLog("ClientManager");

  bool p = false;
  QTcpSocket *socket;
  QTimer *m_reconnectTimer;
  QJsonObject jsonFromString(const QString &data);
  // void notifyChildrenFields(Json::Value local_root); const QJsonObject&
  // localObject
  void notifyChildrenFields(const QJsonObject &localObject);
};

#endif // CLIENTIMPLEMENTATION_H
