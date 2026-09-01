#include "RequestAdapter.h"
#include "FieldUtil.h"
#include "Log.h"
#include "ServerSetup.h"
#include "packet_helper.h"
#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>
#include <optional>
#include <ostream>
#include <qjsonobject.h>
#include <stdexcept>

#ifdef RF_PROTOCOL_FIREHORN
#include "DownlinkCompression_Firehorn2.h"
#include "PacketDefinition_Firehorn2.h"
#include "ParameterDefinition_Firehorn2.h"
#endif /* RF_PROTOCOL_FIREHORN */


#ifdef RF_PROTOCOL_ICARUS
#include "PacketDefinition_Icarus.h"
#include "ParameterDefinition_Icarus.h"
#endif /* RF_PROTOCOL_ICARUS */


static ModuleLog _logger = ModuleLog("RequestAdapter");

namespace {

constexpr double kGroundStationLatDeg = 39.3949167;
constexpr double kGroundStationLonDeg = -8.2928830;
constexpr double kEarthRadiusMeters = 6'371'000.0;
constexpr double kPi = 3.14159265358979323846;

double degToRad(double deg) { return deg * kPi / 180.0; }

double computeDownrangeMeters(double vehicleLatDeg, double vehicleLonDeg) {
  if (!std::isfinite(vehicleLatDeg) || !std::isfinite(vehicleLonDeg)) {
    return std::numeric_limits<double>::quiet_NaN();
  }

  const double lat1 = degToRad(kGroundStationLatDeg);
  const double lon1 = degToRad(kGroundStationLonDeg);
  const double lat2 = degToRad(vehicleLatDeg);
  const double lon2 = degToRad(vehicleLonDeg);

  const double dLat = lat2 - lat1;
  const double dLon = lon2 - lon1;

  const double sinHalfDLat = std::sin(dLat / 2.0);
  const double sinHalfDLon = std::sin(dLon / 2.0);

  const double a = sinHalfDLat * sinHalfDLat +
                   std::cos(lat1) * std::cos(lat2) *
                       sinHalfDLon * sinHalfDLon;
  const double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));

  return kEarthRadiusMeters * c;
}

} // namespace

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
  av_timestamp: %2,
  gnss_lon: %3,
  gnss_lat: %4,
  gnss_alt: %5,
  vertical_speed: %6,
  absolute_speed; %7,
  agl_altitude: %8,
  N2_pressure_1: %9,
  N2_temp_1: %10,
  N2_pressure_2: %11,
  N2_temp_2: %12,
  fuel_pressure: %13,
  fuel_temp: %14,
  LOX_pressure: %15,
  LOX_temp: %16,
  LOX_fls_temp_1: %17,
  LOX_fls_temp_2: %18,
  LOX_fls_temp_3: %19,
  LOX_fls_temp_4: %20,
  LOX_fls_temp_5: %21,
  LOX_fls_temp_6: %22,
  fuel_inj_pressure: %25,
  LOX_inj_pressure: %26,
  chamber_pressure: %27,
  chamber_temp: %28,
  valves_state: %29,
  valve_dpr_fuel: %30,
  valve_dpr_LOX: %31,
  lpb_voltage: %32,
  lpb_current: %33,
  vout_5v_voltage: %34,
  vout_5v_current: %35, 
  hpb_main_voltage: %36,
  hpb_main_current: %37,
  hpb_backup_voltage: %38,
  hpb_backup_current: %39,
  vout_24v_voltage: %40,
  vout_24v_current: %41, 
  av_fc_temp: %42,
  ambient_temp: %43,
  av_state: %44,
  cam_rec: %45,
  rail_cable_status: %47
  av_pyros: %46
  )")
                      .arg(dataAv.packet_nbr)
                      .arg(dataAv.av_timestamp)
                      .arg(dataAv.gnss_lon)
                      .arg(dataAv.gnss_lat)
                      .arg(dataAv.gnss_alt)
                      .arg(dataAv.vertical_speed)
                      .arg(dataAv.absolute_speed)
                      .arg(dataAv.agl_altitude)
                      .arg(dataAv.N2_pressure_1)
                      .arg(dataAv.N2_temp_1)
                      .arg(dataAv.N2_pressure_2)
                      .arg(dataAv.N2_temp_2)
                      .arg(dataAv.fuel_pressure)
                      .arg(dataAv.fuel_temp)
                      .arg(dataAv.LOX_pressure)
                      .arg(dataAv.LOX_temp)
                      .arg(dataAv.LOX_fls_temp_1)
                      .arg(dataAv.LOX_fls_temp_2)
                      .arg(dataAv.LOX_fls_temp_3)
                      .arg(dataAv.LOX_fls_temp_4)
                      .arg(dataAv.LOX_fls_temp_5)
                      .arg(dataAv.LOX_fls_temp_6)
                      .arg(dataAv.fuel_inj_pressure)
                      .arg(dataAv.LOX_inj_pressure)
                      .arg(dataAv.chamber_pressure)
                      .arg(dataAv.chamber_temp)
                      .arg(dataAv.valves_state)
                      .arg(dataAv.valve_dpr_fuel)
                      .arg(dataAv.valve_dpr_LOX)
                      .arg(dataAv.lpb_voltage)
                      .arg(dataAv.lpb_current)
                      .arg(dataAv.vout_5v_voltage)
                      .arg(dataAv.vout_5v_current)
                      .arg(dataAv.hpb_main_voltage)
                      .arg(dataAv.hpb_main_current)
                      .arg(dataAv.hpb_backup_voltage)
                      .arg(dataAv.hpb_backup_current)
                      .arg(dataAv.vout_24v_voltage)
                      .arg(dataAv.vout_24v_current)
                      .arg(dataAv.av_fc_temp)
                      .arg(dataAv.ambient_temp)
                      .arg(dataAv.av_state)
                      .arg(dataAv.cam_rec)
                      .arg(dataAv.rail_cable_status)
                      .arg(dataAv.pyro_status)
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
    jsonObj[QString::number(GUI_FIELD::VERTICAL_SPEED)] =
        QString::number(static_cast<int>(dataAv.vertical_speed)) +
        " [m/s]";
    jsonObj[QString::number(GUI_FIELD::ABSOLUTE_SPEED)] =
        QString::number(static_cast<int>(dataAv.absolute_speed)) +
        " [m/s]";
    jsonObj[QString::number(GUI_FIELD::N2_PRESSURE_1)] =
        QString::number(static_cast<double>(dataAv.N2_pressure_1)) + " [bar]";
    jsonObj[QString::number(GUI_FIELD::N2_PRESSURE_2)] =
        QString::number(static_cast<double>(dataAv.N2_pressure_2)) + " [bar]";
    jsonObj[QString::number(GUI_FIELD::FUEL_PRESSURE)] =
        QString::number(static_cast<double>(dataAv.fuel_pressure)) + " [bar]";
    jsonObj[QString::number(GUI_FIELD::LOX_PRESSURE)] =
        QString::number(static_cast<double>(dataAv.LOX_pressure)) + " [bar]";
    jsonObj[QString::number(GUI_FIELD::LOX_INJ_PRESSURE)] =
        QString::number(static_cast<double>(dataAv.LOX_inj_pressure)) +
        " [bar]";
    jsonObj[QString::number(GUI_FIELD::FUEL_INJ_PRESSURE)] =
        QString::number(static_cast<double>(dataAv.fuel_inj_pressure)) +
        " [bar]";
    jsonObj[QString::number(GUI_FIELD::CHAMBER_PRESSURE)] =
        QString::number(static_cast<double>(dataAv.chamber_pressure)) +
        " [bar]";
    jsonObj[QString::number(GUI_FIELD::N2_TEMP_1)] =
        QString::number(static_cast<int>(dataAv.N2_temp_1)) + " [°C]";
    jsonObj[QString::number(GUI_FIELD::N2_TEMP_2)] =
        QString::number(static_cast<int>(dataAv.N2_temp_2)) + " [°C]";
    jsonObj[QString::number(GUI_FIELD::LOX_TEMP)] =
        QString::number(static_cast<int>(dataAv.LOX_temp)) + " [°C]";
    jsonObj[QString::number(GUI_FIELD::LOX_FLS_TEMP_1)] =
        QString::number(static_cast<int>(dataAv.LOX_fls_temp_1)) + " [°C]";
    jsonObj[QString::number(GUI_FIELD::LOX_FLS_TEMP_2)] =
        QString::number(static_cast<int>(dataAv.LOX_fls_temp_2)) + " [°C]";
    jsonObj[QString::number(GUI_FIELD::LOX_FLS_TEMP_3)] =
        QString::number(static_cast<int>(dataAv.LOX_fls_temp_3)) + " [°C]";
    jsonObj[QString::number(GUI_FIELD::LOX_FLS_TEMP_4)] =
        QString::number(static_cast<int>(dataAv.LOX_fls_temp_4)) + " [°C]";
    jsonObj[QString::number(GUI_FIELD::LOX_FLS_TEMP_5)] =
        QString::number(static_cast<int>(dataAv.LOX_fls_temp_5)) + " [°C]";
    jsonObj[QString::number(GUI_FIELD::LOX_FLS_TEMP_6)] =
        QString::number(static_cast<int>(dataAv.LOX_fls_temp_6)) + " [°C]";

    jsonObj[QString::number(GUI_FIELD::LPB_VOLTAGE)] =
        QString::number(static_cast<double>(dataAv.lpb_voltage)) + " [V]";
    jsonObj[QString::number(GUI_FIELD::LPB_CURRENT)] =
        QString::number(static_cast<double>(dataAv.lpb_current)) + " [A]";
    jsonObj[QString::number(GUI_FIELD::VOUT_5V_VOLTAGE)] =
        QString::number(static_cast<double>(dataAv.vout_5v_voltage)) + " [V]";
    jsonObj[QString::number(GUI_FIELD::VOUT_5V_CURRENT)] =
        QString::number(static_cast<double>(dataAv.vout_5v_current)) + " [A]";
    jsonObj[QString::number(GUI_FIELD::HPB_MAIN_VOLTAGE)] =
        QString::number(static_cast<double>(dataAv.hpb_main_voltage)) + " [V]";
    jsonObj[QString::number(GUI_FIELD::HPB_MAIN_CURRENT)] =
        QString::number(static_cast<double>(dataAv.hpb_main_current)) + " [A]";
    jsonObj[QString::number(GUI_FIELD::HPB_BACKUP_VOLTAGE)] =
        QString::number(static_cast<double>(dataAv.hpb_backup_voltage)) + " [V]";
    jsonObj[QString::number(GUI_FIELD::HPB_BACKUP_CURRENT)] =
        QString::number(static_cast<double>(dataAv.hpb_backup_current)) + " [A]";
    jsonObj[QString::number(GUI_FIELD::VOUT_24V_VOLTAGE)] =
        QString::number(static_cast<double>(dataAv.vout_24v_voltage)) + " [V]";
    jsonObj[QString::number(GUI_FIELD::VOUT_24V_CURRENT)] =
        QString::number(static_cast<double>(dataAv.vout_24v_current)) + " [A]";

    jsonObj[QString::number(GUI_FIELD::AV_FC_TEMP)] =
        QString::number(static_cast<int>(dataAv.av_fc_temp)) + " [°C]";
    jsonObj[QString::number(GUI_FIELD::AMBIENT_TEMP)] =
        QString::number(static_cast<int>(dataAv.ambient_temp)) + " [°C]";

    int engine_states = static_cast<int>(dataAv.valves_state);

    jsonObj[QString::number(GUI_FIELD::PRESSURE_VALVE_FUEL)] =
        QString::number(dataAv.valve_dpr_fuel);
    jsonObj[QString::number(GUI_FIELD::PRESSURE_VALVE_LOX)] =
        QString::number(dataAv.valve_dpr_LOX);
    jsonObj[QString::number(GUI_FIELD::SECURITY_DPR_FUEL)] =
        QString::number((engine_states & AV_VALVE_SDPR_FUEL) > 0 ? 1 : 0);
    jsonObj[QString::number(GUI_FIELD::SECURITY_DPR_LOX)] =
        QString::number((engine_states & AV_VALVE_SDPR_LOX) > 0 ? 1 : 0);
    jsonObj[QString::number(GUI_FIELD::VENT_LOX)] =
        QString::number((engine_states & AV_VALVE_VENT_LOX) > 0 ? 1 : 0);
    jsonObj[QString::number(GUI_FIELD::VENT_FUEL)] =
        QString::number((engine_states & AV_VALVE_VENT_FUEL) > 0 ? 1 : 0);
    jsonObj[QString::number(GUI_FIELD::MAIN_LOX)] =
        QString::number((engine_states & AV_VALVE_MAIN_LOX) > 0 ? 1 : 0);
    jsonObj[QString::number(GUI_FIELD::MAIN_FUEL)] =
        QString::number((engine_states & AV_VALVE_MAIN_FUEL) > 0 ? 1 : 0);

    jsonObj[QString::number(GUI_FIELD::AV_STATE)] =
        fieldUtil::avStateToName(dataAv.av_state);
    jsonObj[QString::number(GUI_FIELD::CAM_REC)] =
        QString::number(static_cast<int>(dataAv.cam_rec));
    jsonObj[QString::number(GUI_FIELD::RAIL_CABLE)] =
        QString::number(static_cast<int>(dataAv.rail_cable_status));
    jsonObj[QString::number(GUI_FIELD::AV_PYROS)] =
        QString::number(static_cast<int>(dataAv.pyro_status));

  const double downrangeMeters =
      computeDownrangeMeters(static_cast<double>(dataAv.gnss_lat),
                             static_cast<double>(dataAv.gnss_lon));
  if (std::isfinite(downrangeMeters)) {
    const double downrangeKilometers = downrangeMeters / 1000.0;
  jsonObj[QString::number(GUI_FIELD::DOWNRANGE)] =
    QString::number(downrangeKilometers, 'f', 2) + " [km]";
  } else {
    jsonObj[QString::number(GUI_FIELD::DOWNRANGE)] = "--";
  }
    // QJsonObject engineStateObj;
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
        QString::number(
            extractDoubleFromFixedPoint(dataHopper.chamber_pressure));

    jsonObj[QString::number(GUI_FIELD::HOPPER_ETH_PRESSURE)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.ETH_pressure));
    jsonObj[QString::number(GUI_FIELD::HOPPER_O_LINE_PRESSURE)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.o_line_pressure));
    jsonObj[QString::number(GUI_FIELD::HOPPER_ETH_LINE_PRESSURE)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.e_line_pressure));
    jsonObj[QString::number(GUI_FIELD::HOPPER_O_INJ_PRESSURE)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.o_inj_pressure));
    jsonObj[QString::number(GUI_FIELD::HOPPER_ETH_INJ_PRESSURE)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.e_inj_pressure));
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
    jsonObj[QString::number(GUI_FIELD::HOPPER_N2O_CURRENT)] =
        QString::number(static_cast<int>(dataHopper.N2O_current));
    jsonObj[QString::number(GUI_FIELD::HOPPER_ETH_CURRENT)] =
        QString::number(static_cast<int>(dataHopper.ETH_current));
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
    jsonObj[QString::number(GUI_FIELD::HOPPER_TEMP1)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.temp1));
    jsonObj[QString::number(GUI_FIELD::HOPPER_TEMP2)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.temp2));
    jsonObj[QString::number(GUI_FIELD::HOPPER_TEMP3)] =
        QString::number(extractDoubleFromFixedPoint(dataHopper.temp3));
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
    jsonObj[QString::number(GUI_FIELD::HOPPER_LOADCELL1)] =
        QString::number(static_cast<int>(dataHopper.loadcell1));
    jsonObj[QString::number(GUI_FIELD::HOPPER_LOADCELL2)] =
        QString::number(static_cast<int>(dataHopper.loadcell2));
    jsonObj[QString::number(GUI_FIELD::HOPPER_LOADCELL3)] =
        QString::number(static_cast<int>(dataHopper.loadcell3));
    jsonObj[QString::number(GUI_FIELD::HOPPER_LOADCELL4)] =
        QString::number(static_cast<int>(dataHopper.loadcell4));
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
    jsonObj["INTERNAL"] = "INTERNAL";
    break;
  }
  case CAPSULE_ID::GSC_INTERNAL_ERR_UPLINK:
  case CAPSULE_ID::GSC_INTERNAL_ERR_VEHICLE_DOWNLINK:
  case CAPSULE_ID::GSC_INTERNAL_ERR_GSE_DOWNLINK: {
    gsc_internal_error_t errorData;
    memcpy(&errorData, data, gsc_internal_error_size);
    _logger.info("PROCESSING INTERNAL ERROR",
                 QString(R"(
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

void populateFSJson(QJsonObject &jsonObj, fs_downlink_t *dataGse) {
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

  case GUI_FIELD::GUI_CMD_HOPPER_HOMING_X:
    return {CMD_ID::HOPPER_CMD_HOMING_X, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_HOPPER_HOMING_Y:
    return {CMD_ID::HOPPER_CMD_HOMING_Y, HOPPER_TELEMETRY};
  
  case GUI_FIELD::GUI_CMD_HOPPER_HOMING_MAIN_N2O:
    return {CMD_ID::HOPPER_CMD_HOMING_MAIN_N2O, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_HOPPER_HOMING_MAIN_FUEL:
    return {CMD_ID::HOPPER_CMD_HOMING_MAIN_FUEL, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_HOPPER_HOMING_MAIN_VALVES:
    return {CMD_ID::HOPPER_CMD_HOMING_MAIN_VALVES, HOPPER_TELEMETRY};

  case GUI_FIELD::GSE_VENT:
    return {CMD_ID::GSE_CMD_VENT, GSE_TELEMETRY};

  case GUI_FIELD::GSE_FILLING_N2O:
    return {CMD_ID::GSE_CMD_FILLING_N2O, GSE_TELEMETRY};

  case GUI_FIELD::HOPPER_ID_CONFIG:
    return {CMD_ID::HOPPER_CMD_ID_CONFIG, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_HOPPER_SPARE_F1:
    return {CMD_ID::HOPPER_CMD_SPARE1, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_HOPPER_SPARE_F2:
    return {CMD_ID::HOPPER_CMD_SPARE2, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_HOPPER_SPARE_F3:
    return {CMD_ID::HOPPER_CMD_SPARE3, HOPPER_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GSE_IDLE:
    return {CMD_ID::GSE_CMD_IDLE, GSE_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GSE_CALIBRATE:
    return {CMD_ID::GSE_CMD_CALIBRATE, GSE_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GSE_ARM:
    return {CMD_ID::GSE_CMD_ARM, GSE_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GSE_PASSIVATE:
    return {GSE_CMD_PASSIVATE, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQN_NC1:
    return {GSE_CMD_TOGGLE_GQN1, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQN_NC2:
    return {GSE_CMD_TOGGLE_GQN2, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQN_NC3:
    return {GSE_CMD_TOGGLE_GQN3, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQN_NC4:
    return {GSE_CMD_TOGGLE_GQN4, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQN_NC5:
    return {GSE_CMD_TOGGLE_GQN5, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GPN_NC:
    return {GSE_CMD_TOGGLE_GPN, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GPA_NC:
    return {GSE_CMD_TOGGLE_GPA, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GVN_NC:
    return {GSE_CMD_TOGGLE_GVN, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GFE_NC:
    return {GSE_CMD_TOGGLE_GFE, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GFO_NCC:
    return {GSE_CMD_TOGGLE_GFO, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GDO_NCC:
    return {GSE_CMD_TOGGLE_GDO, GSE_TELEMETRY};

  case GUI_FIELD::GSE_PC_OLC:
    return {GSE_CMD_TOGGLE_PC, GSE_TELEMETRY};

  case GUI_FIELD::GSE_PUMP:
    return {GSE_CMD_TOGGLE_PC, GSE_TELEMETRY};

  default:
    throw std::invalid_argument("Invalid GUI_FIELD, no command matching");
    break;
  }
}
#endif

void populatePFSJson(QJsonObject &jsonObj, const gse_downlink_t *dataGse) {
  // uint8_t fields - cast to unsigned int

#if RF_PROTOCOL_FIREHORN
  jsonObj[QString::number(GUI_FIELD::GSE_GQD1_NC)] =
      QString::number((dataGse->valves_state & GSE_VALVE_GQD1_NC) > 0 ? 1 : 0);
  jsonObj[QString::number(GUI_FIELD::GSE_GQD2_NC)] =
      QString::number((dataGse->valves_state & GSE_VALVE_GQD2_NC) > 0 ? 1 : 0);
  jsonObj[QString::number(GUI_FIELD::GSE_GQD3_NC)] =
      QString::number((dataGse->valves_state & GSE_VALVE_GQD3_NC) > 0 ? 1 : 0);
  jsonObj[QString::number(GUI_FIELD::GSE_GQD4_NC)] =
      QString::number((dataGse->valves_state & GSE_VALVE_GQD4_NC) > 0 ? 1 : 0);
  jsonObj[QString::number(GUI_FIELD::GSE_GQD5_NC)] =
      QString::number((dataGse->valves_state & GSE_VALVE_GQD5_NC) > 0 ? 1 : 0);
  jsonObj[QString::number(GUI_FIELD::GSE_GQD6_NC)] =
      QString::number((dataGse->valves_state & GSE_VALVE_GQD6_NC) > 0 ? 1 : 0);
  jsonObj[QString::number(GUI_FIELD::GSE_GPA_NC)] =
      QString::number((dataGse->valves_state & GSE_VALVE_GPA_NC) > 0 ? 1 : 0);
  jsonObj[QString::number(GUI_FIELD::GSE_GPN_NC)] =
      QString::number((dataGse->valves_state & GSE_VALVE_GPN_NC) > 0 ? 1 : 0);
  jsonObj[QString::number(GUI_FIELD::GSE_GVN_NC)] =
      QString::number((dataGse->valves_state & GSE_VALVE_GVN_NO) > 0 ? 1 : 0);
  jsonObj[QString::number(GUI_FIELD::GSE_GPA_NC)] =
      QString::number((dataGse->valves_state & GSE_VALVE_GPA_NC) > 0 ? 1 : 0);
  jsonObj[QString::number(GUI_FIELD::GSE_GFO_NCC)] =
      QString::number((dataGse->valves_state & GSE_VALVE_GFO_NCC) > 0 ? 1 : 0);
  jsonObj[QString::number(GUI_FIELD::GSE_GDO_NCC)] =
      QString::number((dataGse->valves_state & GSE_VALVE_GDO_NCC) > 0 ? 1 : 0);
  jsonObj[QString::number(GUI_FIELD::GSE_GFD_NC)] =
      QString::number((dataGse->valves_state & GSE_VALVE_GFD_NC) > 0 ? 1 : 0);
  jsonObj[QString::number(GUI_FIELD::GSE_GDD_NC)] =
      QString::number((dataGse->valves_state & GSE_VALVE_GDD_NC) > 0 ? 1 : 0);
  jsonObj[QString::number(GUI_FIELD::GSE_GFE_NC)] =
      QString::number((dataGse->valves_state & GSE_VALVE_GFE_NC) > 0 ? 1 : 0);
  jsonObj[QString::number(GUI_FIELD::GSE_PUMP)] =
      QString::number(static_cast<unsigned int>(dataGse->PUMP));

  // float fields - no cast needed, QString::number handles float directly
  jsonObj[QString::number(GUI_FIELD::GSE_GP1)] = QString::number(dataGse->GP1);
  jsonObj[QString::number(GUI_FIELD::GSE_GP2)] = QString::number(dataGse->GP2);
  jsonObj[QString::number(GUI_FIELD::GSE_GP3)] = QString::number(dataGse->GP3);
  jsonObj[QString::number(GUI_FIELD::GSE_GP4)] = QString::number(dataGse->GP4);
  jsonObj[QString::number(GUI_FIELD::GSE_GP5)] = QString::number(dataGse->GP5);

#else
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
  jsonObj[QString::number(GUI_FIELD::GSE_GPN_NC)] =
      QString::number(static_cast<unsigned int>(dataGse->GPN_NC2));
  jsonObj[QString::number(GUI_FIELD::GSE_GPA_NC)] =
      QString::number(static_cast<unsigned int>(dataGse->GPN_NC1));
  jsonObj[QString::number(GUI_FIELD::GSE_GVN_NC)] =
      QString::number(static_cast<unsigned int>(dataGse->GVN_NC));
  jsonObj[QString::number(GUI_FIELD::GSE_GFO_NCC)] =
      QString::number(static_cast<unsigned int>(dataGse->GFO_NCC));
  jsonObj[QString::number(GUI_FIELD::GSE_GDO_NCC)] =
      QString::number(static_cast<unsigned int>(dataGse->GDO_NCC));
  jsonObj[QString::number(GUI_FIELD::GSE_GFE_NC)] =
      QString::number(static_cast<unsigned int>(dataGse->GFE_NC));
  jsonObj[QString::number(GUI_FIELD::GSE_PC_OLC)] =
      QString::number(static_cast<unsigned int>(dataGse->PC_OLC));
  jsonObj[QString::number(GUI_FIELD::GSE_PUMP)] =
      QString::number(static_cast<unsigned int>(dataGse->PUMP));

  // float fields - no cast needed, QString::number handles float directly
  jsonObj[QString::number(GUI_FIELD::GSE_GP1)] = QString::number(dataGse->GP1);
  jsonObj[QString::number(GUI_FIELD::GSE_GP2)] = QString::number(dataGse->GP2);
  jsonObj[QString::number(GUI_FIELD::GSE_GP3)] = QString::number(dataGse->GP3);
  jsonObj[QString::number(GUI_FIELD::GSE_GP4)] = QString::number(dataGse->GP4);
  jsonObj[QString::number(GUI_FIELD::GSE_GP5)] = QString::number(dataGse->GP5);
#endif
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

  case GUI_FIELD::MAIN_LOX:
    return {AV_CMD_MAIN_LOX, GSC_CMD};

  case GUI_FIELD::MAIN_FUEL:
    return {AV_CMD_MAIN_FUEL, GSC_CMD};

  case GUI_FIELD::PRESSURE_VALVE_FUEL:
    return {AV_CMD_DPR_FUEL, GSC_CMD};

  case GUI_FIELD::PRESSURE_VALVE_LOX:
    return {AV_CMD_DPR_LOX, GSC_CMD};

  case GUI_FIELD::SECURITY_DPR_FUEL:
    return {AV_CMD_SDPR_FUEL, GSC_CMD};

  case GUI_FIELD::SECURITY_DPR_LOX:
    return {AV_CMD_SDPR_LOX, GSC_CMD};

  case GUI_FIELD::GUI_CMD_GSE_IDLE:
    return {GSE_CMD_IDLE, GSE_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GSE_CALIBRATE:
    return {GSE_CMD_CALIBRATE, GSE_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GSE_ARM:
    return {GSE_CMD_ARM, GSE_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GSE_PASSIVATE:
    return {GSE_CMD_PASSIVATE, GSE_TELEMETRY};

  case GUI_FIELD::GUI_CMD_GSE_TOGGLE_ALL_GQD:
    return {GSE_CMD_TOGGLE_ALL_GQD, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQD1_NC:
    return {GSE_CMD_TOGGLE_GQD1, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQD2_NC:
    return {GSE_CMD_TOGGLE_GQD2, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQD3_NC:
    return {GSE_CMD_TOGGLE_GQD3, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQD4_NC:
    return {GSE_CMD_TOGGLE_GQD4, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQD5_NC:
    return {GSE_CMD_TOGGLE_GQD5, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GQD6_NC:
    return {GSE_CMD_TOGGLE_GQD6, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GPN_NC:
    return {GSE_CMD_TOGGLE_GPN, GSE_TELEMETRY};
  
  case GUI_FIELD::GSE_GVN_NC:
    return {GSE_CMD_TOGGLE_GVN, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GPA_NC:
    return {GSE_CMD_TOGGLE_GPA, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GFO_NCC:
    return {GSE_CMD_TOGGLE_GFO, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GDO_NCC:
    return {GSE_CMD_TOGGLE_GDO, GSE_TELEMETRY};


  case GUI_FIELD::GSE_GFD_NC:
    return {GSE_CMD_TOGGLE_GFD, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GDD_NC:
    return {GSE_CMD_TOGGLE_GDD, GSE_TELEMETRY};

  case GUI_FIELD::GSE_GFE_NC:
    return {GSE_CMD_TOGGLE_GFE, GSE_TELEMETRY};

  case GUI_FIELD::GSE_PUMP:
    return {GSE_CMD_TOGGLE_PUMP, GSE_TELEMETRY};

  default:
    throw std::invalid_argument("Invalid GUI_FIELD, no command matching");
    break;
  }
}
#endif
