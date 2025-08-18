#include <gtest/gtest.h>
#include "../include/RequestAdapter.h"
#include "../../commons/include/FieldUtil.h"
#include "../../commons/ERT_RF_Protocol_Interface/Protocol.h"

#if defined(RF_PROTOCOL_FIREHORN)
#include "../../commons/ERT_RF_Protocol_Interface/PacketDefinition_Firehorn.h"
TEST(RequestAdapterTest, FirehornProtocol) {
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GUI_CMD_CALIBRATE).order_id, AV_CMD_CALIBRATE);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GUI_CMD_PRESSURIZE).order_id, AV_CMD_CALIBRATE);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GUI_CMD_RECOVER).order_id, AV_CMD_RECOVER);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GUI_CMD_MANUAL_DEPLOY).order_id, AV_CMD_MANUAL_DEPLOY);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GUI_CMD_IGNITION).order_id, AV_CMD_IGNITION);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GUI_CMD_ARM).order_id, AV_CMD_ARM);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GUI_CMD_ABORT).order_id, AV_CMD_ABORT);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::VENT_LOX).order_id, AV_CMD_VENT_LOX);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::VENT_FUEL).order_id, AV_CMD_VENT_FUEL);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::MAIN_LOX).order_id, AV_CMD_MAIN_LOX);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::MAIN_FUEL).order_id, AV_CMD_MAIN_FUEL);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::PRESSURE_VALVE_FUEL).order_id, AV_CMD_P_FUEL);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::PRESSURE_VALVE_LOX).order_id, AV_CMD_P_LOX);
}
#elif defined(RF_PROTOCOL_ICARUS)
#include "../../commons/ERT_RF_Protocol_Interface/PacketDefinition_Icarus.h"
TEST(RequestAdapterTest, IcarusProtocol) {
    EXPECT_EQ(getOrderIdFromGui(GUI_CMD_DISCONNECT).order_id, CMD_ID::GSE_CMD_DISCONNECT);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GUI_CMD_CALIBRATE).order_id, CMD_ID::HOPPER_CMD_CALIBRATE);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GUI_CMD_PRESSURIZE).order_id, CMD_ID::HOPPER_CMD_PRESSURIZE);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GUI_CMD_LAUNCH).order_id, CMD_ID::HOPPER_CMD_LAUNCH);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GUI_CMD_ARM).order_id, CMD_ID::HOPPER_CMD_ARM);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GUI_CMD_ABORT).order_id, CMD_ID::HOPPER_CMD_ABORT);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::HOPPER_N2O_VENT).order_id, CMD_ID::HOPPER_CMD_VENT_N2O);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::HOPPER_ETH_VENT).order_id, CMD_ID::HOPPER_CMD_VENT_FUEL);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GUI_CMD_GIMBALL_X).order_id, CMD_ID::HOPPER_CMD_GIMBALL_X);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GUI_CMD_GIMBALL_Y).order_id, CMD_ID::HOPPER_CMD_GIMBALL_Y);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::HOPPER_ETH_MAIN).order_id, CMD_ID::HOPPER_CMD_SERVO_FUEL);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::HOPPER_N2O_MAIN).order_id, CMD_ID::HOPPER_CMD_SERVO_N2O);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::HOPPER_N2_SOL).order_id, CMD_ID::HOPPER_CMD_N2_SOL);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GSE_VENT).order_id, CMD_ID::GSE_CMD_VENT);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::GSE_FILLING_N2O).order_id, CMD_ID::GSE_CMD_FILLING_N2O);
    EXPECT_EQ(getOrderIdFromGui(GUI_FIELD::HOPPER_ID_CONFIG).order_id, CMD_ID::HOPPER_CMD_ID_CONFIG);
}
#else
#error "No protocol defined for tests"
#endif
