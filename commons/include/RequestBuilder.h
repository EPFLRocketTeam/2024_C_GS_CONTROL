#ifndef REQUESTBUILDER_H
#define REQUESTBUILDER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <qglobal.h>

enum class RequestType {
    POST,
    GET,
    INTERNAL,
    SUBSCRIBE,
    UNSUBSCRIBE
};

class RequestBuilder : public QObject
{
    Q_OBJECT
public:
    explicit RequestBuilder(QObject *parent = nullptr);
    
    RequestBuilder& setHeader(RequestType type);
    RequestBuilder& addField(const QString& key, const QString& value);
    RequestBuilder& addField(const QString& key, int value);
    RequestBuilder& addField(const QString& key, const QJsonObject& value);
    
    void clear();

    QJsonObject build() const;
    QString toString() const;
  
    static QString authorizationKey;
private:
    RequestType m_headerType;
    QJsonObject m_payload;
};

#endif // REQUESTBUILDER_H
