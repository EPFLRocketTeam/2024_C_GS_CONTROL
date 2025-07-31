

#ifndef SERVERSETUP_H
#define SERVERSETUP_H

#include <QString>
#include <vector>
#include <stdexcept>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

// To select either the Icarus setup or the Firehorn setup uncomment the line of the Protocol you are using and comment the other
// /!\ if changing the version shown, you also need to make sure that in commons/CMakeLists.txt line 27 the 
// definition RF_PROTOCOL_FIREHORN or RF_PROTOCOL_ICARUS is added
// #define RF_PROTOCOL_ICARUS 1
#define RF_PROTOCOL_FIREHORN 1

// Generate packets every seconds with random data for part of the fields
#define SIMULATE_PACKETS false
// Change the debug level to show the debug messages
#define DEBUG_LOG true

// If MANUALLY_SET_PORT is set to true then please specify the serial port used, otherwise the program will auto open the serial
#define MANUALLY_SET_PORT false
#define SERIAL_USED "/dev/tty0"



// <----------- DO NOT CHANGE ----------->
namespace auth_server {
    typedef struct AuthKey_t {
        QString key;
        uint8_t accessRight; // Access [X X X X X X R W], bit 0 = Write, bit 1 = Read
    } AuthKey;

    inline std::vector<AuthKey> validKeys; // Stores loaded keys

    // Function to load keys from a JSON file
    inline void loadKeysFromFile(const QString& filePath) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            std::string error_msg = "Failed to open the keys file at path: " + filePath.toStdString();
            throw std::runtime_error(error_msg);
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

#endif // END SERVERSETUP_H
