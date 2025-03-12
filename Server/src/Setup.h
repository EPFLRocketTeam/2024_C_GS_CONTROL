

#ifndef SETUP_H
#define SETUP_H

#include <QString>
#include <vector>
#include <stdexcept>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

// #define RF_PROTOCOL_ICARUS 1
#define RF_PROTOCOL_FIREHORN 1
#define SIMULATE_PACKETS 0
#define DEBUG_LOG 1

namespace auth {
    typedef struct AuthKey_t {
        QString key;
        uint8_t accessRight; // Access [X X X X X X R W], bit 0 = Write, bit 1 = Read
    } AuthKey;

    inline std::vector<AuthKey> validKeys; // Stores loaded keys

    // Function to load keys from a JSON file
    inline void loadKeysFromFile(const QString& filePath) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw std::runtime_error("Failed to open the keys file.");
        }

        QByteArray fileData = file.readAll();
        file.close();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);
        if (!jsonDoc.isArray()) {
            throw std::runtime_error("Invalid JSON format: Expected an array.");
        }

        QJsonArray jsonArray = jsonDoc.array();
        validKeys.clear(); // Clear existing keys

        for (const QJsonValue& value : jsonArray) {
            if (!value.isObject()) {
                throw std::runtime_error("Invalid JSON format: Each key entry must be an object.");
            }

            QJsonObject obj = value.toObject();
            QString key = obj.value("key").toString();
            int accessRight = obj.value("accessRight").toInt();

            if (key.isEmpty() || accessRight < 0 || accessRight > 255) {
                throw std::runtime_error("Invalid key or accessRight value in the JSON file.");
            }

            validKeys.push_back({key, static_cast<uint8_t>(accessRight)});
        }
    }
}

#endif
