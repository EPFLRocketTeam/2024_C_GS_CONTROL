#ifndef PACKET_HELPER_H
#define PACKET_HELPER_H

#include <cstdint>
#include <optional>
#include <qjsonobject.h>
#include <string>
#include <QJsonObject>
#include <sys/types.h>

#include "ERT_RF_Protocol_Interface/Protocol.h"


/*std::string to_string(const av_downlink_t &packet);*/
std::optional<const QJsonObject> parse_packet(uint8_t packetId, uint8_t* data, uint32_t len);

#endif /* SERVER_H */
