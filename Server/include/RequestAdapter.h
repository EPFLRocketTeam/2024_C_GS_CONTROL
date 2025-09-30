#ifndef REQUESTADAPTER_H
#define REQUESTADAPTER_H

#include "Protocol.h"
#include "FieldUtil.h"
#include "QJsonObject"

typedef  struct {
    int order_id;
    int capsule_id;
} TranmissionsIDs;

int createUplinkPacketFromRequest(GUI_FIELD field, uint8_t order_value, av_uplink_t* p);
void populatePFSJson(QJsonObject &jsonObj, const gse_downlink_t *dataGse);
#if RF_PROTOCOL_ICARUS
void populateFSJson(QJsonObject &jsonObj, fs_downlink_t* dataGse);
void populateGseJson(QJsonObject &jsonObj, uint8_t *dataGse, int len);
#endif
TranmissionsIDs getOrderIdFromGui(GUI_FIELD f);
float extractDoubleFromFixedPoint(uint16_t fixed);

#endif // REQUESTADAPTER_H 
