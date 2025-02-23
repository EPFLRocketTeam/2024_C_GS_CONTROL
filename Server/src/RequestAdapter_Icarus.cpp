#include "ERT_RF_Protocol_Interface/PacketDefinition_Icarus.h"
#include "FieldUtil.h"
#include "RequestAdapter.h"
#include "ERT_RF_Protocol_Interface/Protocol.h"
#include <iostream>
#include <ostream>
#include <stdexcept>

av_uplink_t* createUplinkPacketFromRequest(GUI_FIELD field, uint8_t order_value) {
    av_uplink_t* p = (av_uplink_t*)calloc(1, av_uplink_size);
    std::cout << "TEAT" << std::endl;
    p->order_id = getOrderIdFromGui(field);
    p->order_value = order_value;
    std::cout << "TEAT2" << std::endl;
    return p;
}

uint8_t getOrderIdFromGui(GUI_FIELD f) {
    switch (f)
    {
    case GUI_FIELD::GUI_CMD_CALIBRATE:
        return CMD_ID::HOPPER_CMD_CALIBRATE;

    case GUI_FIELD::GUI_CMD_LAUNCH:
        return CMD_ID::HOPPER_CMD_LAUNCH;

    case GUI_FIELD::GUI_CMD_ARM:
        return CMD_ID::HOPPER_CMD_ARM;

    case GUI_FIELD::GUI_CMD_ABORT:
        return CMD_ID::HOPPER_CMD_ABORT;

    case GUI_FIELD::HOPPER_N2O_VENT:
        return CMD_ID::HOPPER_CMD_VENT_N2O;

    case GUI_FIELD::HOPPER_ETH_VENT:
        return CMD_ID::HOPPER_CMD_VENT_FUEL;
    
    case GUI_FIELD::GUI_CMD_GIMBALL_X:
        return CMD_ID::HOPPER_CMD_GIMBALL_X;

    case GUI_FIELD::GUI_CMD_GIMBALL_Y:
        return CMD_ID::HOPPER_CMD_GIMBALL_Y;

    case GUI_FIELD::HOPPER_ETH_MAIN:
        return CMD_ID::HOPPER_CMD_SERVO_FUEL;

    case GUI_FIELD::HOPPER_N2O_MAIN:
        return CMD_ID::HOPPER_CMD_SERVO_N2O;

    case GUI_FIELD::GSE_VENT:
        return CMD_ID::GSE_CMD_VENT;
        

    default:
        throw std::invalid_argument("Invalid GUI_FIELD, no command matching");
        break;
    }
}
