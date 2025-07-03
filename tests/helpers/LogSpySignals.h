// spy_utils.h
#ifndef SPY_UTILS_H
#define SPY_UTILS_H

#include <QSignalSpy>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>

inline void DumpSpySignal(const QSignalSpy* spy, const QString& spyName = QString()) {
    if (spyName.isEmpty()) {
        qDebug() << "==== DumpSpySignal: ====";
    } else {
        qDebug() << "==== DumpSpySignal for" << spyName << "====";
    }

    int count = spy->count();
    qDebug() << "  captured signals:" << count;
    for (int i = 0; i < count; ++i) {
        QVariantList args = spy->at(i);
        qDebug() << QString("  -- Signal #%1 --").arg(i+1);
        for (int j = 0; j < args.size(); ++j) {
            const QVariant& v = args.at(j);
            qDebug().noquote() << QString("     arg%1: ").arg(j)
                               << (
                                      v.canConvert<QJsonObject>()
                                      ? QJsonDocument(v.toJsonObject())
                                          .toJson(QJsonDocument::Compact)
                                      : v.toString()
                                  );
            // If it is a socket pointer, print its descriptor too:
            if (v.canConvert<QObject*>()) {
                QObject* obj = v.value<QObject*>();
                if (auto* sock = qobject_cast<QTcpSocket*>(obj)) {
                    qDebug() << "        [QTcpSocket descriptor:" << sock->socketDescriptor() << "]";
                }
            }
        }
    }
    qDebug() << "========================\n";
}

#endif // SPY_UTILS_H
