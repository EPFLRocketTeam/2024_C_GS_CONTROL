#ifndef PACKET_HELPER_H
#define PACKET_HELPER_H

#include <cstdint>
#include <optional>
#include <qjsonobject.h>
#include <string>
#include <QJsonObject>
#include <sys/types.h>

#include "Protocol.h"
#include "data_storage.h"


/*std::string to_string(const av_downlink_t &packet);*/
std::optional<QJsonObject> process_packet (uint8_t packetId, uint8_t* data, uint32_t len, SqliteDB* db);

#endif /* SERVER_H */
