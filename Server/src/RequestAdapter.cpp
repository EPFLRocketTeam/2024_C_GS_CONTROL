#include "RequestAdapter.h"
#include "FieldUtil.h"
#include "Log.h"
#include "Protocol.h"
#include "ServerSetup.h"
#include "packet_helper.h"
#include <cstdint>
#include <iostream>
#include <optional>
#include <ostream>
#include <qjsonobject.h>
#include <stdexcept>

static ModuleLog _logger = ModuleLog("RequestAdapter");

/*#define RF_PROTOCOL_ICARUS 1*/
/*#define RF_PROTOCOL_FIREHORN 0*/


int createUplinkPacketFromRequest(GUI_FIELD field, uint8_t order_value,
                                  av_uplink_t *p) {
  TranmissionsIDs tIDs = getOrderIdFromGui(field);
  p->order_id = tIDs.order_id;
  p->order_value = order_value;
  return tIDs.capsule_id;
}

float extractDoubleFromFixedPoint(uint16_t fixed) {
  // Interpret the bits as a signed 16-bit integer
  int16_t signed_val = (int16_t)fixed;

  // Divide by 2^6 (64) to restore fractional scaling
  return (float)signed_val / (1 << 6);
}

std::optional<QJsonObject> process_packet(uint8_t packetId, uint8_t *data,
                                          uint32_t len, SqliteDB *db) {
  // Create a JSON object
  QJsonObject jsonObj;
  switch (packetId) {
#if RF_PROTOCOL_FIREHORN
  case CAPSULE_ID::AV_TELEMETRY: {
    av_downlink_t *packedData = new av_downlink_t;
    // Assuming 'packedData' is a pointer to an av_downlink_t struct
    // e.g., av_downlink_t* packedData;

    // Copy the incoming raw data into our packet structure.
    memcpy(packedData, data, av_downlink_size);
    _logger.info("PROCESSING UNPACKED", QString(R"(
    	packet_nbr: %1,
    )")
    .arg(packedData->packet_nbr)
    .toStdString()); 
    /**/

    av_downlink_unpacked_t dataAv = decode_downlink(*packedData);
    db->write_pkt(db->process_pkt(NULL, &dataAv, NULL));
    _logger.info("PROCESSING UNPACKED", QString(R"(
    packet_nbr: %1,
    gnss_lon: %2,
    gnss_lat: %3,
    gnss_alt: %4,
    gnss_vertical_speed: %5,
    N2_pressure: %6,
    N2_temp: %7,
    fuel_pressure: %8,
    LOX_pressure: %9,
    LOX_temp: %10,
    LOX_inj_pressure: %11,
    LOX_inj_temp: %12,
    fuel_inj_pressure: %13,
    chamber_pressure: %14,
    engine_state: %15,
    lpb_voltage: %16,
    lpb_current: %17,
    hpb_voltage: %18,
    hpb_current: %19,
    av_fc_temp: %20,
    ambient_temp: %21,
    av_state: %22,
    cam_rec: %23,
    )")
    .arg(dataAv.packet_nbr)
    .arg(dataAv.gnss_lon)
    .arg(dataAv.gnss_lat)
    .arg(dataAv.gnss_alt)
    .arg(dataAv.gnss_vertical_speed)
    .arg(dataAv.N2_pressure)
    .arg(dataAv.N2_temp)
    .arg(dataAv.fuel_pressure)
    .arg(dataAv.LOX_pressure)
    .arg(dataAv.LOX_temp)
    .arg(dataAv.LOX_inj_pressure)
    .arg(dataAv.LOX_inj_temp)
    .arg(dataAv.fuel_inj_pressure)
    .arg(dataAv.chamber_pressure)
    .arg(dataAv.engine_state)
    .arg(dataAv.lpb_voltage)
    .arg(dataAv.lpb_current)
    .arg(dataAv.hpb_voltage)
    .arg(dataAv.hpb_current)
    .arg(dataAv.av_fc_temp)
    .arg(dataAv.ambient_temp)
    .arg(dataAv.av_state)
    .arg(dataAv.cam_rec)
    .toStdString());
    delete packedData;

    jsonObj[QString::number(GUI_FIELD::PACKET_NBR)] =
        QString::number(static_cast<int>(dataAv.packet_nbr));
    jsonObj[QString::number(GUI_FIELD::AV_TIMER)] = QString("1");
    jsonObj[QString::number(GUI_FIELD::GNSS_LON)] =
        QString::number(static_cast<double>(dataAv.gnss_lon)) + " [°]";
    jsonObj[QString::number(GUI_FIELD::GNSS_LAT)] =
        QString::number(static_cast<double>(dataAv.gnss_lat)) + " [°]";
    jsonObj[QString::number(GUI_FIELD::GNSS_ALT)] =
        QString::number(static_cast<double>(dataAv.gnss_alt)) + " [m]";
    jsonObj[QString::number(GUI_FIELD::GNSS_VERTICAL_SPEED)] =
        QString::number(static_cast<int>(dataAv.gnss_vertical_speed)) + " [m/s]";
    jsonObj[QString::number(GUI_FIELD::N2_PRESSURE)] =
        QString::number(static_cast<double>(dataAv.N2_pressure)) + " [bar]";
    jsonObj[QString::number(GUI_FIELD::FUEL_PRESSURE)] =
        QString::number(static_cast<double>(dataAv.fuel_pressure)) + " [bar]";
    jsonObj[QString::number(GUI_FIELD::LOX_PRESSURE)] =
        QString::number(static_cast<double>(dataAv.LOX_pressure)) + " [bar]";
    jsonObj[QString::number(GUI_FIELD::LOX_INJ_PRESSURE)] =
        QString::number(static_cast<double>(dataAv.LOX_inj_pressure)) + " [bar]";
    jsonObj[QString::number(GUI_FIELD::FUEL_INJ_PRESSURE)] =
        QString::number(static_cast<double>(dataAv.fuel_inj_pressure)) + " [bar]";
    jsonObj[QString::number(GUI_FIELD::CHAMBER_PRESSURE)] =
        QString::number(static_cast<double>(dataAv.chamber_pressure)) + " [bar]";
    jsonObj[QString::number(GUI_FIELD::N2_TEMP)] =
        QString::number(static_cast<int>(dataAv.N2_temp)) + " [°C]";
    jsonObj[QString::number(GUI_FIELD::LOX_TEMP)] =
        QString::number(static_cast<int>(dataAv.LOX_temp)) + " [°C]";
    jsonObj[QString::number(GUI_FIELD::LOX_INJ_TEMP)] =
        QString::number(static_cast<int>(dataAv.LOX_inj_temp)) + " [°C]";
    jsonObj[QString::number(GUI_FIELD::LPB_VOLTAGE)] =
        QString::number(static_cast<double>(dataAv.lpb_voltage)) + " [V]";
    jsonObj[QString::number(GUI_FIELD::HPB_VOLTAGE)] =
        QString::number(static_cast<double>(dataAv.hpb_voltage)) + " [V]";
    jsonObj[QString::number(GUI_FIELD::LPB_CURRENT)] =
        QString::number(static_cast<double>(dataAv.lpb_current)) + " [A]";
    jsonObj[QString::number(GUI_FIELD::HPB_CURRENT)] =
        QString::number(static_cast<double>(dataAv.hpb_current)) + " [A]";

    jsonObj[QString::number(GUI_FIELD::AV_FC_TEMP)] =
        QString::number(static_cast<int>(dataAv.av_fc_temp)) + " [°C]";
    jsonObj[QString::number(GUI_FIELD::AMBIENT_TEMP)] =
        QString::number(static_cast<int>(dataAv.ambient_temp)) + " [°C]";


    int engine_states = static_cast<int>(dataAv.engine_state);
    jsonObj[QString::number(GUI_FIELD::VENT_N2)] =
        QString::number((engine_states & ENGINE_STATE_VENT_N2) > 0 ? 1 : 0);
    jsonObj[QString::number(GUI_FIELD::PRESSURE_VALVE_LOX)] =
        QString::number((engine_states & ENGINE_STATE_P_LOX) > 0 ? 1 : 0);
    jsonObj[QString::number(GUI_FIELD::PRESSURE_VALVE_FUEL)] =
        QString::number((engine_states & ENGINE_STATE_P_FUEL) > 0 ? 1 : 0);
    jsonObj[QString::number(GUI_FIELD::VENT_LOX)] =
        QString::number((engine_states & ENGINE_STATE_VENT_LOX) > 0 ? 1 : 0);
    jsonObj[QString::number(GUI_FIELD::VENT_FUEL)] =
        QString::number((engine_states & ENGINE_STATE_VENT_FUEL) > 0 ? 1 : 0);
    jsonObj[QString::number(GUI_FIELD::MAIN_LOX)] =
        QString::number((engine_states & ENGINE_STATE_MAIN_LOX) > 0 ? 1 : 0);
    jsonObj[QString::number(GUI_FIELD::MAIN_FUEL)] =
        QString::number((engine_states & ENGINE_STATE_MAIN_FUEL) > 0 ? 1 : 0);

    jsonObj[QString::number(GUI_FIELD::AV_STATE)] =
        fieldUtil::avStateToName(dataAv.av_state);
    jsonObj[QString::number(GUI_FIELD::CAM_REC)] =
        QString::number(static_cast<int>(dataAv.cam_rec));
    QJsonObject engineStateObj;
    // Add the sub-object to the main JSON object
    /*jsonObj[QString::number(GUI_FIELD::ENGINE_STATE)] = engineStateObj;*/
    break;
  }
#endif
  case CAPSULE_ID::GSE_TELEMETRY: {
    
    #ifdef RF_PROTOCOL_FIREHORN
    gse_downlink_t *dataGse = new gse_downlink_t;

    // Copy the incoming raw data into our packet structure.
    memcpy(dataGse, data, gse_downlink_size);
    db->write_pkt(db->process_pkt(NULL, NULL, dataGse));

    populatePFSJson(jsonObj, dataGse);
    delete dataGse;
    #endif

    #if RF_PROTOCOL_ICARUS
    populateGseJson(jsonObj, data, len);
    #endif

    break;
  }

#if RF_PROTOCOL_ICARUS
  case CAPSULE_ID::HOPPER_TELEMETRY: {

    // Make sure the incoming data is at least the size of the Hopper packet.
    if (len < av_downlink_size) {
      break;
    }
    av_downlink_t dataHopper;
    // Copy the incoming raw data into our Hopper packet structure.
    memcpy(&dataHopper, data, av_downlink_size);
    db->write_pkt(db->process_pkt(NULL, &dataHopper, NULL));

    // Create a JSON object and fill it with the Hopper packet's fields.
    std::cout << dataHopper.packet_nbr << std::endl;
    jsonObj[QString::number(GUI_FIELD::PACKET_NBR)] =
        QString::number(static_cast<uint32_t>(dataHopper.packet_nbr));
    jsonObj[QString::number(GUI_FIELD::AV_TIMER)] = QString("1");
    jsonObj[QString::number(GUI_FIELD::HOPPER_N2O_PRESSURE)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.N2O_pressure));
    jsonObj[QString::number(GUI_FIELD::HOPPER_CHAMBER_PRESSURE)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.chamber_pressure));

    jsonObj[QString::number(GUI_FIELD::HOPPER_ETH_PRESSURE)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.ETH_pressure));
    _logger.error("EVENT",
                  QString(R"(%1)")
                      .arg(extractDoubleFromFixedPoint(dataHopper.N2O_temp))
                      .toStdString());
    jsonObj[QString::number(GUI_FIELD::HOPPER_N2O_TEMP)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.N2O_temp));
    // Vents are defined as nside a nested struct.
    jsonObj[QString::number(GUI_FIELD::HOPPER_N2O_VENT)] =
        QString::number(static_cast<int>(dataHopper.N2O_vent));
    jsonObj[QString::number(GUI_FIELD::HOPPER_ETH_VENT)] =
        QString::number(static_cast<int>(dataHopper.ETH_vent));
    jsonObj[QString::number(GUI_FIELD::HOPPER_N2_SOL)] =
        QString::number(static_cast<int>(dataHopper.N2_solenoid));
    jsonObj[QString::number(GUI_FIELD::HOPPER_N2O_MAIN)] =
        QString::number(static_cast<int>(dataHopper.N2O_main));
    jsonObj[QString::number(GUI_FIELD::HOPPER_ETH_MAIN)] =
        QString::number(static_cast<int>(dataHopper.ETH_main));
    jsonObj[QString::number(GUI_FIELD::HOPPER_ETH_SOL)] =
        QString::number(static_cast<int>(dataHopper.ETH_sol));
    jsonObj[QString::number(GUI_FIELD::HOPPER_N2O_SOL)] =
        QString::number(static_cast<int>(dataHopper.N2O_sol));
    jsonObj[QString::number(GUI_FIELD::HOPPER_IGNITER)] =
        QString::number(static_cast<int>(dataHopper.igniter));
    jsonObj[QString::number(GUI_FIELD::HOPPER_GNSS_LON)] =
        QString::number(static_cast<double>(dataHopper.gnss_lon));
    jsonObj[QString::number(GUI_FIELD::HOPPER_GNSS_LAT)] =
        QString::number(static_cast<double>(dataHopper.gnss_lat));
    jsonObj[QString::number(GUI_FIELD::HOPPER_SAT_NBR)] =
        QString::number(static_cast<int>(dataHopper.sat_nbr));
    jsonObj[QString::number(GUI_FIELD::HOPPER_GYRO_X)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.gyro_x));
    jsonObj[QString::number(GUI_FIELD::HOPPER_GYRO_Y)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.gyro_y));
    jsonObj[QString::number(GUI_FIELD::HOPPER_GYRO_Z)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.gyro_z));
    jsonObj[QString::number(GUI_FIELD::HOPPER_ACC_X)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.acc_x));
    jsonObj[QString::number(GUI_FIELD::HOPPER_ACC_Y)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.acc_y));
    jsonObj[QString::number(GUI_FIELD::HOPPER_ACC_Z)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.acc_z));
    jsonObj[QString::number(GUI_FIELD::HOPPER_BARO)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.baro));
    jsonObj[QString::number(GUI_FIELD::HOPPER_KALMAN_POS_X)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.kalman_pos_x));
    jsonObj[QString::number(GUI_FIELD::HOPPER_KALMAN_POS_Y)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.kalman_pos_y));
    jsonObj[QString::number(GUI_FIELD::HOPPER_KALMAN_POS_Z)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.kalman_pos_z));
    jsonObj[QString::number(GUI_FIELD::HOPPER_KALMAN_YAW)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.kalman_yaw));
    jsonObj[QString::number(GUI_FIELD::HOPPER_KALMAN_PITCH)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.kalman_pitch));
    jsonObj[QString::number(GUI_FIELD::HOPPER_KALMAN_ROLL)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.kalman_roll));
    jsonObj[QString::number(GUI_FIELD::HOPPER_GIMBAL_X)] =
        QString::number(static_cast<int>(dataHopper.gimbal_x));
    jsonObj[QString::number(GUI_FIELD::HOPPER_GIMBAL_Y)] =
        QString::number(static_cast<int>(dataHopper.gimbal_y));
    jsonObj[QString::number(GUI_FIELD::HOPPER_HV_VOLTAGE)] =
        QString::number(static_cast<int>(dataHopper.HV_voltage));
    jsonObj[QString::number(GUI_FIELD::HOPPER_LV_VOLTAGE)] =
        QString::number(static_cast<int>(dataHopper.LV_voltage));
    jsonObj[QString::number(GUI_FIELD::HOPPER_AV_TEMP)] =
        QString::number(static_cast<int>(dataHopper.AV_temp));
    jsonObj[QString::number(GUI_FIELD::HOPPER_ID_CONFIG)] =
        QString::number(static_cast<int>(dataHopper.ID_config));
    jsonObj[QString::number(GUI_FIELD::HOPPER_AV_STATE)] =
        QString::number(static_cast<int>(dataHopper.AV_state));

    break;
  }
#endif
  case CAPSULE_ID::ABORT_BOARD: {
    jsonObj["ABORT"] = "ABORT";
    break;
  }
  case CAPSULE_ID::GSC_INTERNAL_UPLINK:
	case CAPSULE_ID::GSC_INTERNAL_VEHICLE_DOWNLINK:
	case CAPSULE_ID::GSC_INTERNAL_GSE_DOWNLINK: {
    gsc_internal_t internalData;
    memcpy(&internalData, data, gsc_internal_size);
    _logger.info("PROCESSING INTERNAL", QString(R"(
      Capsule ID: %1,
      RSSI: %2,
      SNR: %3,
    )")
    .arg(packetId)
    .arg(internalData.rssi)
    .arg(internalData.snr)
    .toStdString());
    return std::nullopt;
  }
  case CAPSULE_ID::GSC_INTERNAL_ERR_UPLINK:
	case CAPSULE_ID::GSC_INTERNAL_ERR_VEHICLE_DOWNLINK:
	case CAPSULE_ID::GSC_INTERNAL_ERR_GSE_DOWNLINK: {
    gsc_internal_error_t errorData;
    memcpy(&errorData, data, gsc_internal_error_size);
    _logger.info("PROCESSING INTERNAL ERROR", QString(R"(
      Capsule ID: %1,
      [ERROR] Tx Failed: %2,
      [ERROR] Rx Not a downlink packet: %3,
      [ERROR] Rx Buffer overflow: %4,
    )")
    .arg(packetId)
    .arg((errorData.error & (1 << 0)) > 0)
    .arg((errorData.error & (1 << 1)) > 0)
    .arg((errorData.error & (1 << 2)) > 0)

    .toStdString());
    return std::nullopt;
  }

  default:
    return std::nullopt;
  }

  return jsonObj;
}

#if RF_PROTOCOL_ICARUS

void populateFSJson(QJsonObject &jsonObj, fs_downlink_t* dataGse) {
  // Add primitive data members to JSON object
  jsonObj[QString::number(GUI_FIELD::GSE_TANK_PRESSURE)] =
      QString::number(static_cast<double>(dataGse->tankPressure));
  jsonObj[QString::number(GUI_FIELD::GSE_TIMER)] = QString("1");
  jsonObj[QString::number(GUI_FIELD::GSE_TANK_TEMPERATURE)] =
      QString::number(static_cast<double>(dataGse->tankTemperature));
  jsonObj[QString::number(GUI_FIELD::GSE_FILLING_PRESSURE)] =
      QString::number(static_cast<double>(dataGse->fillingPressure));
  jsonObj[QString::number(GUI_FIELD::GSE_DISCONNECT_ACTIVE)] =
      QString::number(dataGse->disconnectActive);
  jsonObj[QString::number(GUI_FIELD::GSE_LOADCELL_1)] =
      QString::number(static_cast<int>(dataGse->loadcell1));
  jsonObj[QString::number(GUI_FIELD::GSE_LOADCELL_2)] =
      QString::number(static_cast<int>(dataGse->loadcell2));
  jsonObj[QString::number(GUI_FIELD::GSE_LOADCELL_3)] =
      QString::number(static_cast<int>(dataGse->loadcell3));
  jsonObj[QString::number(GUI_FIELD::GSE_LOADCELL_4)] =
      QString::number(static_cast<int>(dataGse->loadcell4));
  // Create a sub-object for status
  QJsonObject statusObj;
  statusObj[QString::number(GUI_FIELD::GSE_FILLING_N2O)] =
      QString::number(static_cast<int>(dataGse->status.fillingN2O));
  statusObj[QString::number(GUI_FIELD::GSE_VENT)] =
      QString::number(static_cast<int>(dataGse->status.vent));
  jsonObj[QString::number(GUI_FIELD::GSE_CMD_STATUS)] = statusObj;
}



void populateGseJson(QJsonObject &jsonObj, uint8_t *data, int len) {
    if (len == fs_downlink_size) {
        fs_downlink_t *dataGse = new fs_downlink_t;

        // Copy the incoming raw data into our packet structure.
        memcpy(dataGse, data, fs_downlink_size);
        /*db->write_pkt(db->process_pkt(NULL, NULL, dataGse));*/

        populateFSJson(jsonObj, dataGse);
        delete dataGse;

        return;
    } 
    gse_downlink_t *dataGse = new gse_downlink_t;

    // Copy the incoming raw data into our packet structure.
    memcpy(dataGse, data, gse_downlink_size);
    /*db->write_pkt(db->process_pkt(NULL, NULL, dataGse));*/

    populatePFSJson(jsonObj, dataGse);
    delete dataGse;
}



TranmissionsIDs getOrderIdFromGui(GUI_FIELD f) {
  switch (f) {
  case GUI_CMD_DISCONNECT:
    return {CMD_ID::GSE_CMD_DISCONNECT, GSE_TELEMETRY};

  case GUI_FIELD::GUI_CMD_HOPPER_TARE_ORIENTATION:
    return {CMD_ID::HOPPER_CMD_CALIBRATE, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_LAUNCH:
    return {CMD_ID::HOPPER_CMD_LAUNCH, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_ARM:
    return {CMD_ID::HOPPER_CMD_ARM, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_ABORT:
    return {CMD_ID::HOPPER_CMD_ABORT, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_HOPPER_IDLE:
    return {CMD_ID::HOPPER_CMD_IDLE, HOPPER_TELEMETRY};

  case GUI_FIELD::HOPPER_N2O_VENT:
    return {CMD_ID::HOPPER_CMD_VENT_N2O, HOPPER_TELEMETRY};

  case GUI_FIELD::HOPPER_ETH_VENT:
    return {CMD_ID::HOPPER_CMD_VENT_FUEL, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GIMBALL_X:
    return {CMD_ID::HOPPER_CMD_GIMBALL_X, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GIMBALL_Y:
    return {CMD_ID::HOPPER_CMD_GIMBALL_Y, HOPPER_TELEMETRY};

  case GUI_FIELD::HOPPER_ETH_MAIN:
    return {CMD_ID::HOPPER_CMD_MAIN_FUEL, HOPPER_TELEMETRY};

  case GUI_FIELD::HOPPER_N2O_MAIN:
    return {CMD_ID::HOPPER_CMD_MAIN_N2O, HOPPER_TELEMETRY};

  case GUI_FIELD::HOPPER_N2_SOL:
    return {CMD_ID::HOPPER_CMD_N2_SOL, HOPPER_TELEMETRY};

  case GUI_FIELD::HOPPER_N2O_SOL:
    return {CMD_ID::HOPPER_CMD_N2O_SOL, HOPPER_TELEMETRY};

  case GUI_FIELD::HOPPER_ETH_SOL:
    return {CMD_ID::HOPPER_CMD_FUEL_SOL, HOPPER_TELEMETRY};

 case GUI_FIELD::HOPPER_IGNITER:
    return {CMD_ID::HOPPER_CMD_IGNITER, HOPPER_TELEMETRY};


  case GUI_FIELD::GUI_CMD_HOPPER_TARE:
    return {CMD_ID::HOPPER_CMD_TARE, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_HOPPER_HOMING_GIMBAL:
    return {CMD_ID::HOPPER_CMD_HOMING_GIMBAL, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_HOPPER_HOMING_MAIN_VALVES:
    return {CMD_ID::HOPPER_CMD_HOMING_MAIN_VALVES, HOPPER_TELEMETRY};

  case GUI_FIELD::GSE_VENT:
    return {CMD_ID::GSE_CMD_VENT, GSE_TELEMETRY};

  case GUI_FIELD::GSE_FILLING_N2O:
    return {CMD_ID::GSE_CMD_FILLING_N2O, GSE_TELEMETRY};

  case GUI_FIELD::HOPPER_ID_CONFIG:
    return {CMD_ID::HOPPER_CMD_ID_CONFIG, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GSE_IDLE:
    return {GSE_CMD_IDLE, GSE_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GSE_CALIBRATE:
    return {GSE_CMD_CALIBRATE, GSE_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GSE_ARM:
    return {GSE_CMD_ARM, GSE_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GSE_PASSIVATE:

    return {GSE_CMD_PASSIVATE, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQN_NC1:
    return {GSE_CMD_TOGGLE_11, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQN_NC2:
    return {GSE_CMD_TOGGLE_12, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQN_NC3:
    return {GSE_CMD_TOGGLE_13, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQN_NC4:
    return {GSE_CMD_TOGGLE_14, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQN_NC5:
    return {GSE_CMD_TOGGLE_15, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GPN_NC1:
    return {GSE_CMD_TOGGLE_16, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GPN_NC2:
    return {GSE_CMD_TOGGLE_21, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GVN_NC:
    return {GSE_CMD_TOGGLE_22, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GFE_NC:
    return {GSE_CMD_TOGGLE_23, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GFO_NCC:
    return {GSE_CMD_TOGGLE_24, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GDO_NCC:
    return {GSE_CMD_TOGGLE_25, GSE_TELEMETRY};

  case GUI_FIELD::GSE_PC_OLC:
    return {GSE_CMD_TOGGLE_27, GSE_TELEMETRY};

  default:
    throw std::invalid_argument("Invalid GUI_FIELD, no command matching");
    break;
  }
}
#endif

void populatePFSJson(QJsonObject &jsonObj, const gse_downlink_t *dataGse) {
  // uint8_t fields - cast to unsigned int
  jsonObj[QString::number(GUI_FIELD::GSE_GQN_NC1)] =
      QString::number(static_cast<unsigned int>(dataGse->GQN_NC1));
  jsonObj[QString::number(GUI_FIELD::GSE_GQN_NC2)] =
      QString::number(static_cast<unsigned int>(dataGse->GQN_NC2));
  jsonObj[QString::number(GUI_FIELD::GSE_GQN_NC3)] =
      QString::number(static_cast<unsigned int>(dataGse->GQN_NC3));
  jsonObj[QString::number(GUI_FIELD::GSE_GQN_NC4)] =
      QString::number(static_cast<unsigned int>(dataGse->GQN_NC4));
  jsonObj[QString::number(GUI_FIELD::GSE_GQN_NC5)] =
      QString::number(static_cast<unsigned int>(dataGse->GQN_NC5));
  jsonObj[QString::number(GUI_FIELD::GSE_GPN_NC1)] =
      QString::number(static_cast<unsigned int>(dataGse->GPN_NC1));
  jsonObj[QString::number(GUI_FIELD::GSE_GPN_NC2)] =
      QString::number(static_cast<unsigned int>(dataGse->GPN_NC2));
  jsonObj[QString::number(GUI_FIELD::GSE_GVN_NC)] =
      QString::number(static_cast<unsigned int>(dataGse->GVN_NC));
  jsonObj[QString::number(GUI_FIELD::GSE_GFE_NC)] =
      QString::number(static_cast<unsigned int>(dataGse->GFE_NC));
  jsonObj[QString::number(GUI_FIELD::GSE_GFO_NCC)] =
      QString::number(static_cast<unsigned int>(dataGse->GFO_NCC));
  jsonObj[QString::number(GUI_FIELD::GSE_GDO_NCC)] =
      QString::number(static_cast<unsigned int>(dataGse->GDO_NCC));
  jsonObj[QString::number(GUI_FIELD::GSE_PC_OLC)] =
      QString::number(static_cast<unsigned int>(dataGse->PC_OLC));

  // float fields - no cast needed, QString::number handles float directly
  jsonObj[QString::number(GUI_FIELD::GSE_GP1)] = QString::number(dataGse->GP1);
  jsonObj[QString::number(GUI_FIELD::GSE_GP2)] = QString::number(dataGse->GP2);
  jsonObj[QString::number(GUI_FIELD::GSE_GP3)] = QString::number(dataGse->GP3);
  jsonObj[QString::number(GUI_FIELD::GSE_GP4)] = QString::number(dataGse->GP4);
  jsonObj[QString::number(GUI_FIELD::GSE_GP5)] = QString::number(dataGse->GP5);
}



#if RF_PROTOCOL_FIREHORN
TranmissionsIDs getOrderIdFromGui(GUI_FIELD f) {
  switch (f) {
    /*case GUI_CMD_DISCONNECT:*/
    /*  return {CMD_ID::GSE_CMD_DISCONNECT, CAPSULE_ID::GSE_TELEMETRY};*/

  case GUI_FIELD::GUI_CMD_CALIBRATE:
    return {AV_CMD_CALIBRATE, CAPSULE_ID::GSC_CMD};

  case GUI_FIELD::GUI_CMD_LAUNCH:
    return {AV_CMD_LAUNCH, GSC_CMD};

  case GUI_FIELD::GUI_CMD_RECOVER:
    return {AV_CMD_RECOVER, GSC_CMD};

  case GUI_FIELD::GUI_CMD_ARM:
    return {AV_CMD_ARM, GSC_CMD};

  case GUI_FIELD::GUI_CMD_PRESSURIZE:
    return {AV_CMD_PRESSURIZE, GSC_CMD};

  case GUI_FIELD::GUI_CMD_ABORT:
    return {AV_CMD_ABORT, GSC_CMD};

  case GUI_FIELD::VENT_LOX:
    return {AV_CMD_VENT_LOX, GSC_CMD};

  case GUI_FIELD::VENT_FUEL:
    return {AV_CMD_VENT_FUEL, GSC_CMD};

  case GUI_FIELD::VENT_N2:
    return {AV_CMD_VENT_N2, GSC_CMD};

  case GUI_FIELD::MAIN_LOX:
    return {AV_CMD_MAIN_LOX, GSC_CMD};

  case GUI_FIELD::MAIN_FUEL:
    return {AV_CMD_MAIN_FUEL, GSC_CMD};

  case GUI_FIELD::PRESSURE_VALVE_FUEL:
    return {AV_CMD_P_FUEL, GSC_CMD};

  case GUI_FIELD::PRESSURE_VALVE_LOX:
    return {AV_CMD_P_LOX, GSC_CMD};

  case GUI_FIELD::GUI_CMD_GSE_IDLE:
    return {GSE_CMD_IDLE, GSE_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GSE_CALIBRATE:
    return {GSE_CMD_CALIBRATE, GSE_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GSE_ARM:
    return {GSE_CMD_ARM, GSE_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GSE_PASSIVATE:
    return {GSE_CMD_PASSIVATE, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQN_NC1:
    return {GSE_CMD_TOGGLE_11, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQN_NC2:
    return {GSE_CMD_TOGGLE_12, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQN_NC3:
    return {GSE_CMD_TOGGLE_13, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQN_NC4:
    return {GSE_CMD_TOGGLE_14, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQN_NC5:
    return {GSE_CMD_TOGGLE_15, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GPN_NC1:
    return {GSE_CMD_TOGGLE_16, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GPN_NC2:
    return {GSE_CMD_TOGGLE_21, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GVN_NC:
    return {GSE_CMD_TOGGLE_22, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GFE_NC:
    return {GSE_CMD_TOGGLE_23, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GFO_NCC:
    return {GSE_CMD_TOGGLE_24, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GDO_NCC:
    return {GSE_CMD_TOGGLE_25, GSE_TELEMETRY};

  case GUI_FIELD::GSE_PC_OLC:
    return {GSE_CMD_TOGGLE_27, GSE_TELEMETRY};

  default:
    throw std::invalid_argument("Invalid GUI_FIELD, no command matching");
    break;
  }
}
#endif
