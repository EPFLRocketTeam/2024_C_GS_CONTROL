#include "ERT_RF_Protocol_Interface/PacketDefinition_Firehorn.h"
#include "ERT_RF_Protocol_Interface/Protocol.h"
#include "FieldUtil.h"
#include "RequestAdapter.h"
#include "packet_helper.h"
#include <cstdint>
#include <iostream>
#include <optional>
#include <ostream>
#include <qjsonobject.h>
#include <stdexcept>
#include "Setup.h"


int createUplinkPacketFromRequest(GUI_FIELD field, uint8_t order_value, av_uplink_t* p) {
    TranmissionsIDs tIDs = getOrderIdFromGui(field);
    p->order_id = tIDs.order_id;
    p->order_value = order_value;
    return tIDs.capsule_id;
}

std::optional<QJsonObject> parse_packet(uint8_t packetId, uint8_t *data, uint32_t len) {
    // Create a JSON object
    QJsonObject jsonObj;
    switch (packetId) {
        #if RF_PROTOCOL_FIREHORN
        case CAPSULE_ID::AV_TELEMETRY: {
            av_downlink_t packedData;

            // Copy the incoming raw data into our packet structure.
            memcpy(&packedData, data, sizeof(packedData));
            
            av_downlink_unpacked dataAv = decode_downlink(packedData);
            
            jsonObj[QString::number(GUI_FIELD::PACKET_NBR)] = QString::number(static_cast<int>(dataAv.packet_nbr));
            jsonObj[QString::number(GUI_FIELD::AV_TIMER)] = QString("1");
            jsonObj[QString::number(GUI_FIELD::GNSS_LON)] = QString::number(static_cast<double>(dataAv.gnss_lon));
            jsonObj[QString::number(GUI_FIELD::GNSS_LAT)] = QString::number(static_cast<double>(dataAv.gnss_lat));
            jsonObj[QString::number(GUI_FIELD::GNSS_ALT)] = QString::number(static_cast<double>(dataAv.gnss_alt));
            jsonObj[QString::number(GUI_FIELD::GNSS_VERTICAL_SPEED)] = QString::number(static_cast<int>(dataAv.gnss_vertical_speed));
            jsonObj[QString::number(GUI_FIELD::N2_PRESSURE)] = QString::number(static_cast<double>(dataAv.N2_pressure));
            jsonObj[QString::number(GUI_FIELD::FUEL_PRESSURE)] = QString::number(static_cast<double>(dataAv.fuel_pressure));
            jsonObj[QString::number(GUI_FIELD::LOX_PRESSURE)] = QString::number(static_cast<double>(dataAv.LOX_pressure));
            jsonObj[QString::number(GUI_FIELD::FUEL_LEVEL)] = QString::number(static_cast<double>(dataAv.fuel_level));
            jsonObj[QString::number(GUI_FIELD::LOX_LEVEL)] = QString::number(static_cast<double>(dataAv.LOX_level));
            jsonObj[QString::number(GUI_FIELD::N2_TEMP)] = QString::number(static_cast<int>(dataAv.N2_temp));
            jsonObj[QString::number(GUI_FIELD::LOX_TEMP)] = QString::number(static_cast<int>(dataAv.LOX_temp));
            jsonObj[QString::number(GUI_FIELD::LOX_INJ_TEMP)] = QString::number(static_cast<int>(dataAv.LOX_inj_temp));
            jsonObj[QString::number(GUI_FIELD::LPB_VOLTAGE)] = QString::number(static_cast<double>(dataAv.lpb_voltage));
            jsonObj[QString::number(GUI_FIELD::HPB_VOLTAGE)] = QString::number(static_cast<double>(dataAv.hpb_voltage));
            jsonObj[QString::number(GUI_FIELD::AV_FC_TEMP)] = QString::number(static_cast<int>(dataAv.av_fc_temp));
            jsonObj[QString::number(GUI_FIELD::AMBIENT_TEMP)] = QString::number(static_cast<int>(dataAv.ambient_temp));
            
            int engine_states = static_cast<int>(dataAv.engine_state);
            jsonObj[QString::number(GUI_FIELD::IGNITER_LOX)] = QString::number((engine_states & ENGINE_STATE_IGN_LOX) > 0 ? 1 : 0);
            jsonObj[QString::number(GUI_FIELD::IGNITER_FUEL)] = QString::number((engine_states & ENGINE_STATE_IGN_FUEL) > 0 ? 1 : 0);
            jsonObj[QString::number(GUI_FIELD::VENT_LOX)] = QString::number((engine_states & ENGINE_STATE_VENT_LOX) > 0 ? 1 : 0);
            jsonObj[QString::number(GUI_FIELD::VENT_FUEL)] = QString::number((engine_states & ENGINE_STATE_VENT_FUEL) > 0 ? 1 : 0);
            jsonObj[QString::number(GUI_FIELD::MAIN_LOX)] = QString::number((engine_states & ENGINE_STATE_MAIN_LOX) > 0 ? 1 : 0);
            jsonObj[QString::number(GUI_FIELD::MAIN_FUEL)] = QString::number((engine_states & ENGINE_STATE_MAIN_FUEL) > 0 ? 1 : 0);
            
            jsonObj[QString::number(GUI_FIELD::AV_STATE)] = QString::number(static_cast<int>(dataAv.av_state));
            jsonObj[QString::number(GUI_FIELD::CAM_REC)] = QString::number(static_cast<int>(dataAv.cam_rec));            
            QJsonObject engineStateObj;
            // Add the sub-object to the main JSON object
            /*jsonObj[QString::number(GUI_FIELD::ENGINE_STATE)] = engineStateObj;*/
            break;
        }
        #endif
        case CAPSULE_ID::GSE_TELEMETRY: {
            PacketGSE_downlink dataGse;
            
            // Copy the incoming raw data into our packet structure.
            memcpy(&dataGse, data, sizeof(PacketGSE_downlink));

            // Add primitive data members to JSON object
            jsonObj[QString::number(GUI_FIELD::GSE_TANK_PRESSURE)] = QString::number(static_cast<double>(dataGse.tankPressure));
            jsonObj[QString::number(GUI_FIELD::GSE_TIMER)] = QString("1");
            jsonObj[QString::number(GUI_FIELD::GSE_TANK_TEMPERATURE)] = QString::number(static_cast<double>(dataGse.tankTemperature));
            jsonObj[QString::number(GUI_FIELD::GSE_FILLING_PRESSURE)] = QString::number(static_cast<double>(dataGse.fillingPressure));
            jsonObj[QString::number(GUI_FIELD::GSE_DISCONNECT_ACTIVE)] = QString::number(dataGse.disconnectActive);
            #if RF_PROTOCOL_FIREHORN
            jsonObj[QString::number(GUI_FIELD::GSE_LOADCELL_1)] = QString::number(static_cast<int>(dataGse.loadcell_raw));
            #else 
            jsonObj[QString::number(GUI_FIELD::GSE_LOADCELL_1)] = QString::number(static_cast<int>(dataGse.loadcell1));
            jsonObj[QString::number(GUI_FIELD::GSE_LOADCELL_2)] = QString::number(static_cast<int>(dataGse.loadcell2));
            jsonObj[QString::number(GUI_FIELD::GSE_LOADCELL_3)] = QString::number(static_cast<int>(dataGse.loadcell3));
            jsonObj[QString::number(GUI_FIELD::GSE_LOADCELL_4)] = QString::number(static_cast<int>(dataGse.loadcell4));
            #endif
            // Create a sub-object for status
            QJsonObject statusObj;
            statusObj[QString::number(GUI_FIELD::GSE_FILLING_N2O)] = QString::number(static_cast<int>(dataGse.status.fillingN2O));
            statusObj[QString::number(GUI_FIELD::GSE_VENT)] = QString::number(static_cast<int>(dataGse.status.vent));

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
            jsonObj[QString::number(GUI_FIELD::AV_TIMER)] = QString("1");
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


#if RF_PROTOCOL_ICARUS
TranmissionsIDs getOrderIdFromGui(GUI_FIELD f) {
    switch (f)
    {
    case GUI_CMD_DISCONNECT:
        return {CMD_ID::GSE_CMD_DISCONNECT, GSE_TELEMETRY};

    case GUI_FIELD::GUI_CMD_CALIBRATE:
        return {CMD_ID::HOPPER_CMD_CALIBRATE, HOPPER_DOWNLINK};
    
    case GUI_FIELD::GUI_CMD_PRESSURIZE:
        return {CMD_ID::HOPPER_CMD_PRESSURIZE, HOPPER_DOWNLINK};

    case GUI_FIELD::GUI_CMD_LAUNCH:
        return {CMD_ID::HOPPER_CMD_LAUNCH, HOPPER_DOWNLINK};

    case GUI_FIELD::GUI_CMD_ARM:
        return {CMD_ID::HOPPER_CMD_ARM, HOPPER_DOWNLINK};

    case GUI_FIELD::GUI_CMD_ABORT:
        return {CMD_ID::HOPPER_CMD_ABORT, HOPPER_DOWNLINK};

    case GUI_FIELD::HOPPER_N2O_VENT:
        return {CMD_ID::HOPPER_CMD_VENT_N2O, HOPPER_DOWNLINK};

    case GUI_FIELD::HOPPER_ETH_VENT:
        return {CMD_ID::HOPPER_CMD_VENT_FUEL, HOPPER_DOWNLINK};
    
    case GUI_FIELD::GUI_CMD_GIMBALL_X:
        return {CMD_ID::HOPPER_CMD_GIMBALL_X, HOPPER_DOWNLINK};

    case GUI_FIELD::GUI_CMD_GIMBALL_Y:
        return {CMD_ID::HOPPER_CMD_GIMBALL_Y, HOPPER_DOWNLINK};

    case GUI_FIELD::HOPPER_ETH_MAIN:
        return {CMD_ID::HOPPER_CMD_SERVO_FUEL, HOPPER_DOWNLINK};

    case GUI_FIELD::HOPPER_N2O_MAIN:
        return {CMD_ID::HOPPER_CMD_SERVO_N2O, HOPPER_DOWNLINK};

    case GUI_FIELD::HOPPER_N2_SOL:
        return {CMD_ID::HOPPER_CMD_N2_SOL, HOPPER_DOWNLINK};

    case GUI_FIELD::GSE_VENT:
        return {CMD_ID::GSE_CMD_VENT, GSE_TELEMETRY};

    case GUI_FIELD::GSE_FILLING_N2O:
        return {CMD_ID::GSE_CMD_FILLING_N2O, GSE_TELEMETRY};

    case GUI_FIELD::HOPPER_ID_CONFIG:
        return {CMD_ID::HOPPER_CMD_ID_CONFIG, HOPPER_DOWNLINK};
        

    default:
        throw std::invalid_argument("Invalid GUI_FIELD, no command matching");
        break;
    }
}
#endif

#if RF_PROTOCOL_FIREHORN
TranmissionsIDs getOrderIdFromGui(GUI_FIELD f) {
    switch (f)
    {
    case GUI_CMD_DISCONNECT:
        return { GSE_CMD_DISCONNECT, GSE_TELEMETRY } ;

    case GUI_FIELD::GUI_CMD_CALIBRATE:
        return { AV_CMD_CALIBRATE, AV_TELEMETRY};
    
    case GUI_FIELD::GUI_CMD_PRESSURIZE:
        return { AV_CMD_PRESSURIZE, AV_TELEMETRY };

    case GUI_FIELD::GUI_CMD_RECOVER:
        return { AV_CMD_RECOVER, AV_TELEMETRY };

    case GUI_FIELD::GUI_CMD_MANUAL_DEPLOY:
        return { AV_CMD_MANUAL_DEPLOY, AV_TELEMETRY };

    case GUI_FIELD::GUI_CMD_IGNITION:
        return { AV_CMD_IGNITION, AV_TELEMETRY };

    case GUI_FIELD::GUI_CMD_ARM:
        return { AV_CMD_ARM, AV_TELEMETRY };

    case GUI_FIELD::GUI_CMD_ABORT:
        return { AV_CMD_ABORT, AV_TELEMETRY };

    case GUI_FIELD::VENT_LOX:
        return { AV_CMD_VENT_LOX, AV_TELEMETRY };

    case GUI_FIELD::VENT_FUEL:
        return { AV_CMD_VENT_FUEL, AV_TELEMETRY };
    
    case GUI_FIELD::MAIN_LOX:
        return { AV_CMD_MAIN_LOX, AV_TELEMETRY };

    case GUI_FIELD::MAIN_FUEL:
        return { AV_CMD_MAIN_FUEL, AV_TELEMETRY };

    case GUI_FIELD::IGNITER_FUEL:
        return { AV_CMD_IGNITER_FUEL, AV_TELEMETRY };

    case GUI_FIELD::IGNITER_LOX:
        return { AV_CMD_IGNITER_LOX, AV_TELEMETRY };

    case GUI_FIELD::GSE_FILLING_N2O:
        return { GSE_CMD_FILLING_LOX, GSE_TELEMETRY };

    case GUI_FIELD::GSE_VENT:
        return { CMD_ID::GSE_CMD_VENT, GSE_TELEMETRY };

    default:
        throw std::invalid_argument("Invalid GUI_FIELD, no command matching");
        break;
    }
}
#endif
