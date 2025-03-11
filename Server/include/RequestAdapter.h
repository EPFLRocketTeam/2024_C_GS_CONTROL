#ifndef REQUESTADAPTER_H
#define REQUESTADAPTER_H

#include "ERT_RF_Protocol_Interface/Protocol.h"
#include "FieldUtil.h"

typedef  struct {
    int order_id;
    int capsule_id;
} TranmissionsIDs;

int createUplinkPacketFromRequest(GUI_FIELD field, uint8_t order_value, av_uplink_t* p);
TranmissionsIDs getOrderIdFromGui(GUI_FIELD f);

#endif // REQUESTADAPTER_H 
