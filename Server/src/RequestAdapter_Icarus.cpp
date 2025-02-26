#include "ERT_RF_Protocol_Interface/PacketDefinition_Icarus.h"
#include "FieldUtil.h"
#include "RequestAdapter.h"
#include "ERT_RF_Protocol_Interface/Protocol.h"
#include "packet_helper.h"
#include <iostream>
#include <optional>
#include <ostream>
#include <qjsonobject.h>
#include <stdexcept>
#include "Setup.h"

av_uplink_t* createUplinkPacketFromRequest(GUI_FIELD field, uint8_t order_value) {
    av_uplink_t* p = (av_uplink_t*)calloc(1, av_uplink_size);
    p->order_id = getOrderIdFromGui(field);
    p->order_value = order_value;
    return p;
}

std::optional<const QJsonObject> parse_packet(uint8_t packetId, uint8_t *data, uint32_t len) {
    // Create a JSON object
    const QJsonObject jsonObj;
    switch (packetId) {
        case 0x00: 
            std::cout << "Packet with ID 00 received : " << +packetId << std::endl;
            break;
        #if RF_PROTOCOL_FIREHORN
        case CAPSULE_ID::AV_TELEMETRY: {
            av_downlink_t dataAv;

            // Copy the incoming raw data into our packet structure.
            memcpy(&dataAv, data, sizeof(av_downlink_t));

            // Iterate over struct members and add them to the JSON object
            jsonObj[QString::number(GUI_FIELD::PACKET_NBR)] = static_cast<int>(dataAv.packet_nbr);
            jsonObj[QString::number(GUI_FIELD::TIMESTAMP)] = static_cast<int>(dataAv.timestamp);
            jsonObj[QString::number(GUI_FIELD::GNSS_LON)] = static_cast<double>(dataAv.gnss_lon);
            jsonObj[QString::number(GUI_FIELD::GNSS_LAT)] = static_cast<double>(dataAv.gnss_lat);
            jsonObj[QString::number(GUI_FIELD::GNSS_ALT)] = static_cast<double>(dataAv.gnss_alt);
            jsonObj[QString::number(GUI_FIELD::GNSS_LON_R)] = static_cast<double>(dataAv.gnss_lon_r);
            jsonObj[QString::number(GUI_FIELD::GNSS_LAT_R)] = static_cast<double>(dataAv.gnss_lat_r);
            jsonObj[QString::number(GUI_FIELD::GNSS_ALT_R)] = static_cast<double>(dataAv.gnss_alt_r);
            jsonObj[QString::number(GUI_FIELD::GNSS_VERTICAL_SPEED)] = static_cast<double>(dataAv.gnss_vertical_speed);
            /*jsonObj[QString::number(GUI_FIELD::N2_PRESSURE)] = static_cast<double>(dataAv.N2_pressure);*/
            /*jsonObj[QString::number(GUI_FIELD::FUEL_PRESSURE)] = static_cast<double>(dataAv.fuel_pressure);*/
            /*jsonObj[QString::number(GUI_FIELD::LOX_PRESSURE)] = static_cast<double>(dataAv.LOX_pressure);*/
            /*jsonObj[QString::number(GUI_FIELD::FUEL_LEVEL)] = static_cast<double>(dataAv.fuel_level);*/
            /*jsonObj[QString::number(GUI_FIELD::LOX_LEVEL)] = static_cast<double>(dataAv.LOX_level);*/
            /*jsonObj[QString::number(GUI_FIELD::ENGINE_TEMP)] = static_cast<double>(dataAv.engine_temp);*/
            /*jsonObj[QString::number(GUI_FIELD::IGNITER_PRESSURE)] = static_cast<double>(dataAv.igniter_pressure);*/
            /*jsonObj[QString::number(GUI_FIELD::LOX_INJ_PRESSURE)] = static_cast<double>(dataAv.LOX_inj_pressure);*/
            /*jsonObj[QString::number(GUI_FIELD::FUEL_INJ_PRESSURE)] = static_cast<double>(dataAv.fuel_inj_pressure);*/
            /*jsonObj[QString::number(GUI_FIELD::CHAMBER_PRESSURE)] = static_cast<double>(dataAv.chamber_pressure);*/

            // Create a sub-object for engine_state_t
            QJsonObject engineStateObj;
            /*engineStateObj[QString::number(GUI_FIELD::IGNITER_LOX)] = static_cast<int>(dataAv.engine_state.igniter_LOX);*/
            /*engineStateObj[QString::number(GUI_FIELD::IGNITER_FUEL)] = static_cast<int>(dataAv.engine_state.igniter_fuel);*/
            /*engineStateObj[QString::number(GUI_FIELD::MAIN_LOX)] = static_cast<int>(dataAv.engine_state.main_LOX);*/
            /*engineStateObj[QString::number(GUI_FIELD::MAIN_FUEL)] = static_cast<int>(dataAv.engine_state.main_fuel);*/
            /*engineStateObj[QString::number(GUI_FIELD::VENT_LOX)] = static_cast<int>(dataAv.engine_state.vent_LOX);*/
            engineStateObj[QString::number(GUI_FIELD::VENT_FUEL)] = static_cast<int>(dataAv.engine_state.vent_fuel);

            // Add the sub-object to the main JSON object
            jsonObj[QString::number(GUI_FIELD::ENGINE_STATE)] = engineStateObj;
            break;
        }
        #endif
        case CAPSULE_ID::GSE_TELEMETRY: {
            PacketGSE_downlink dataGse;
            
            // Copy the incoming raw data into our packet structure.
            memcpy(&dataGse, data, sizeof(PacketGSE_downlink));

            // Add primitive data members to JSON object
            jsonObj[QString::number(GUI_FIELD::GSE_TANK_PRESSURE)] = static_cast<double>(dataGse.tankPressure);
            jsonObj[QString::number(GUI_FIELD::GSE_TANK_TEMPERATURE)] = static_cast<double>(dataGse.tankTemperature);
            jsonObj[QString::number(GUI_FIELD::GSE_FILLING_PRESSURE)] = static_cast<double>(dataGse.fillingPressure);
            jsonObj[QString::number(GUI_FIELD::GSE_DISCONNECT_ACTIVE)] = dataGse.disconnectActive;
            jsonObj[QString::number(GUI_FIELD::GSE_LOADCELL_1)] = static_cast<int>(dataGse.loadcell1);
            jsonObj[QString::number(GUI_FIELD::GSE_LOADCELL_2)] = static_cast<int>(dataGse.loadcell2);
            jsonObj[QString::number(GUI_FIELD::GSE_LOADCELL_3)] = static_cast<int>(dataGse.loadcell3);
            jsonObj[QString::number(GUI_FIELD::GSE_LOADCELL_4)] = static_cast<int>(dataGse.loadcell4);

            // Create a sub-object for status
            QJsonObject statusObj;
            statusObj[QString::number(GUI_FIELD::FILLINGN2O)] = static_cast<int>(dataGse.status.fillingN2O);
            statusObj[QString::number(GUI_FIELD::GSE_VENT)] = static_cast<int>(dataGse.status.vent);

            // Add the sub-object to the main JSON object
            jsonObj[QString::number(GUI_FIELD::GSE_CMD_STATUS)] = statusObj;

            break;
        }
        #if RF_PROTOCOL_ICARUS
        case CAPSULE_ID::HOPPER_DOWNLINK: {
            // Make sure the incoming data is at least the size of the Hopper packet.
            if (len < sizeof(PacketHopper_downlink)) {
                break;
            }
            PacketHopper_downlink dataHopper;
            // Copy the incoming raw data into our Hopper packet structure.
            memcpy(&dataHopper, data, sizeof(PacketHopper_downlink));
            
            // Create a JSON object and fill it with the Hopper packet's fields.
            QJsonObject jsonObj;
            jsonObj[QString::number(GUI_FIELD::HOPPER_PACKET_NBR)]   = static_cast<int>(dataHopper.packet_nbr);
            jsonObj[QString::number(GUI_FIELD::HOPPER_N2O_PRESSURE)]   = static_cast<int>(dataHopper.N2O_pressure);
            jsonObj[QString::number(GUI_FIELD::HOPPER_ETH_PRESSURE)]   = static_cast<int>(dataHopper.ETH_pressure);
            jsonObj[QString::number(GUI_FIELD::HOPPER_N2O_TEMP)]       = static_cast<int>(dataHopper.N2O_temp);
            // Vents are defined as nside a nested struct.
            jsonObj[QString::number(GUI_FIELD::HOPPER_N2O_VENT)]       = static_cast<int>(dataHopper.vents.N2O_vent);
            jsonObj[QString::number(GUI_FIELD::HOPPER_ETH_VENT)]       = static_cast<int>(dataHopper.vents.ETH_vent);
            jsonObj[QString::number(GUI_FIELD::HOPPER_N2_SOL)]       = static_cast<int>(dataHopper.vents.N2_sol);
            jsonObj[QString::number(GUI_FIELD::HOPPER_N2O_MAIN)]       = static_cast<int>(dataHopper.N2O_main);
            jsonObj[QString::number(GUI_FIELD::HOPPER_ETH_MAIN)]       = static_cast<int>(dataHopper.ETH_main);
            jsonObj[QString::number(GUI_FIELD::HOPPER_GNSS_LON)]       = static_cast<double>(dataHopper.gnss_lon);
            jsonObj[QString::number(GUI_FIELD::HOPPER_GNSS_LAT)]       = static_cast<double>(dataHopper.gnss_lat);
            jsonObj[QString::number(GUI_FIELD::HOPPER_SAT_NBR)]        = static_cast<int>(dataHopper.sat_nbr);
            jsonObj[QString::number(GUI_FIELD::HOPPER_GYRO_X)]         = static_cast<int>(dataHopper.gyro_x);
            jsonObj[QString::number(GUI_FIELD::HOPPER_GYRO_Y)]         = static_cast<int>(dataHopper.gyro_y);
            jsonObj[QString::number(GUI_FIELD::HOPPER_GYRO_Z)]         = static_cast<int>(dataHopper.gyro_z);
            jsonObj[QString::number(GUI_FIELD::HOPPER_ACC_X)]          = static_cast<int>(dataHopper.acc_x);
            jsonObj[QString::number(GUI_FIELD::HOPPER_ACC_Y)]          = static_cast<int>(dataHopper.acc_y);
            jsonObj[QString::number(GUI_FIELD::HOPPER_ACC_Z)]          = static_cast<int>(dataHopper.acc_z);
            jsonObj[QString::number(GUI_FIELD::HOPPER_BARO)]           = static_cast<int>(dataHopper.baro);
            jsonObj[QString::number(GUI_FIELD::HOPPER_KALMAN_POS_X)]   = static_cast<int>(dataHopper.kalman_pos_x);
            jsonObj[QString::number(GUI_FIELD::HOPPER_KALMAN_POS_Y)]   = static_cast<int>(dataHopper.kalman_pos_y);
            jsonObj[QString::number(GUI_FIELD::HOPPER_KALMAN_POS_Z)]   = static_cast<int>(dataHopper.kalman_pos_z);
            jsonObj[QString::number(GUI_FIELD::HOPPER_KALMAN_YAW)]     = static_cast<int>(dataHopper.kalman_yaw);
            jsonObj[QString::number(GUI_FIELD::HOPPER_KALMAN_PITCH)]   = static_cast<int>(dataHopper.kalman_pitch);
            jsonObj[QString::number(GUI_FIELD::HOPPER_KALMAN_ROLL)]    = static_cast<int>(dataHopper.kalman_roll);
            jsonObj[QString::number(GUI_FIELD::HOPPER_GIMBAL_X)]       = static_cast<int>(dataHopper.gimbal_x);
            jsonObj[QString::number(GUI_FIELD::HOPPER_GIMBAL_Y)]       = static_cast<int>(dataHopper.gimbal_y);
            jsonObj[QString::number(GUI_FIELD::HOPPER_HV_VOLTAGE)]     = static_cast<int>(dataHopper.HV_voltage);
            jsonObj[QString::number(GUI_FIELD::HOPPER_LV_VOLTAGE)]     = static_cast<int>(dataHopper.LV_voltage);
            jsonObj[QString::number(GUI_FIELD::HOPPER_AV_TEMP)]        = static_cast<int>(dataHopper.AV_temp);
            jsonObj[QString::number(GUI_FIELD::HOPPER_ID_CONFIG)]        = static_cast<int>(dataHopper.ID_config);
            jsonObj[QString::number(GUI_FIELD::HOPPER_AV_STATE)]        = static_cast<int>(dataHopper.AV_state);
            
            break;
        }
        #endif
        default:
            return std::nullopt;
    }
    return jsonObj;
}

uint8_t getOrderIdFromGui(GUI_FIELD f) {
    switch (f)
    {
    case GUI_CMD_DISCONNECT:
        return CMD_ID::GSE_CMD_DISCONNECT;

    case GUI_FIELD::GUI_CMD_CALIBRATE:
        return CMD_ID::HOPPER_CMD_CALIBRATE;
    
    case GUI_FIELD::GUI_CMD_PRESSURIZE:
        return CMD_ID::HOPPER_CMD_PRESSURIZE;

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

    case GUI_FIELD::HOPPER_N2_SOL:
        return CMD_ID::HOPPER_CMD_N2_SOL;

    case GUI_FIELD::GSE_VENT:
        return CMD_ID::GSE_CMD_VENT;
        

    default:
        throw std::invalid_argument("Invalid GUI_FIELD, no command matching");
        break;
    }
}
