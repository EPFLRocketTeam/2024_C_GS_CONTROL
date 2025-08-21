#ifndef REQUESTADAPTER_H
#define REQUESTADAPTER_H

#include "ERT_RF_Protocol_Interface/Protocol.h"
#include "FieldUtil.h"
#include "QJsonObject"

typedef  struct {
    int order_id;
    int capsule_id;
} TranmissionsIDs;

int createUplinkPacketFromRequest(GUI_FIELD field, uint8_t order_value, av_uplink_t* p);
void populateGseJson(QJsonObject &jsonObj, const gse_downlink_t *dataGse);
TranmissionsIDs getOrderIdFromGui(GUI_FIELD f);
float extractDoubleFromFixedPoint(uint16_t fixed);

#endif // REQUESTADAPTER_H 
