#ifndef SQLITEDB_H
#define SQLITEDB_H

#include "../src/ServerSetup.h"
#include <sqlite3.h>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <time.h>
#include <vector>

#include "../../commons/ERT_RF_Protocol_Interface/Protocol.h"
#include "sqlite_orm.h"

typedef enum { AV_UPLINK, AV_DOWNLINK, GSE_DOWNLINK, INVALID } PacketType;

struct AV_uplink_pkt {
  uint32_t id;
  std::string ts;
  uint8_t order_id;
  uint8_t order_value;
};

#if RF_PROTOCOL_FIREHORN
struct AV_downlink_pkt {
  uint32_t id;
  std::string ts;
  uint32_t packet_nbr;
  float gnss_lon;
  float gnss_lat;
  float gnss_alt;
  int8_t gnss_vertical_speed;
  float N2_pressure;
  float fuel_pressure;
  float LOX_pressure;
  float fuel_level;
  float LOX_level;
  int16_t N2_temp;
  int16_t LOX_temp;
  int16_t LOX_inj_temp;
  float lpb_voltage;
  float hpb_voltage;
  int16_t av_fc_temp;
  int16_t ambient_temp;
  uint8_t engine_state;
  uint8_t av_state;
  uint8_t cam_rec;
};

struct GSE_downlink_pkt {
  uint32_t id;
  std::string ts;
  uint8_t GQN_NC1; // Nitrogen and Ethanol disconnect actuation
  uint8_t GQN_NC2; // LOX disconnect actuation
  uint8_t GQN_NC3; // reserved
  uint8_t GQN_NC4; // reserved

  uint8_t GQN_NC5; // Low mass flow anti-freeze lox disconnect
  uint8_t GPN_NC1; // Controls the activation of the pressure booster

  uint8_t GPN_NC2; // Control the opening of the high pressure bottle
  uint8_t GVN_NC;  // Vents the tube before disconnect
  uint8_t GFE_NC;  // Controls the filling of ethanol along with the pump
  uint8_t GFO_NCC; // Controls LOX filling
  uint8_t GDO_NCC; // Vent the tube before disconnect
  uint8_t PC_OLC;  // Trigger Lox disconnect and purge the tube of LOX

  float GP1; // Nitrogen pressure in the filling line
  float GP2; // LOX pressure in the deware
  float GP3; // Pressure in the low-pressure side of the gas booster
  float GP4; // Pressure before the pneumatic valve
  float GP5; // Pressure in the ethanol filling line
};

#else

struct AV_downlink_pkt {
  uint32_t id;
  std::string ts;
  uint32_t packet_nbr;
  uint16_t N2O_pressure;
  uint16_t ETH_pressure;
  uint8_t N2O_temp;
  uint8_t N2O_vent;
  uint8_t ETH_vent;
  uint8_t N2_solenoid;
  uint8_t N2O_main;
  uint8_t ETH_main;
  uint8_t N2O_sol;
  uint8_t ETH_sol;

  uint32_t gnss_lon; // Data with RTK correction
  uint32_t gnss_lat; // Data with RTK correction
  uint8_t sat_nbr;   // gnss : number of fixed satellite
  uint16_t gyro_x;   // raw gyro
  uint16_t gyro_y;   // raw gyro
  uint16_t gyro_z;   // raw gyro
  uint16_t acc_x;    // raw acc
  uint16_t acc_y;    // raw acc
  uint16_t acc_z;    // raw acc
  uint16_t baro;
  uint16_t kalman_pos_x;
  uint16_t kalman_pos_y;
  uint16_t kalman_pos_z;
  uint16_t kalman_yaw;
  uint16_t kalman_pitch;
  uint16_t kalman_roll;
  uint8_t gimbal_x;
  uint8_t gimbal_y;
  uint8_t HV_voltage;
  uint8_t LV_voltage;
  uint8_t AV_temp;
  uint8_t ID_config;
  uint8_t
      AV_state; // AV Power-up / Idle / Initialisation / Pressurization / Armed
                // / Motor Fire-up / Automatic Flight / Forced Landing / ABORT
};

struct GSE_downlink_pkt {
  uint32_t id;
  std::string ts;
  float tankPressure;
  float tankTemperature;
  float fillingPressure;
  uint8_t fillingN2O;
  uint8_t vent;
  bool disconnectActive;
  uint32_t loadcell1;
  uint32_t loadcell2;
  uint32_t loadcell3;
  uint32_t loadcell4;
};
#endif

typedef struct {
  PacketType type;
  AV_uplink_pkt *av_up_pkt;
  AV_downlink_pkt *av_down_pkt;
  GSE_downlink_pkt *gse_down_pkt;
} Packet;

class SqliteDB {
public:
  /*to be called one time at the initialisation of the server*/
  SqliteDB();

  ~SqliteDB();

  /*return value : add to buffer (0), add to buffer and flush (1), packet type
  specified is NULL (2) put null pointer for unused packet types*/
  int write_pkt(const Packet pkt);

  Packet read_pkt(PacketType type, uint32_t pkt_id);

  std::vector<AV_uplink_pkt> read_all_avup();
  std::vector<AV_downlink_pkt> read_all_avdw();
  std::vector<GSE_downlink_pkt> read_all_gsdw();

  /*sould not be called unless for the tests*/
  int delete_database();

#if RF_PROTOCOL_FIREHORN
#include "../../commons/ERT_RF_Protocol_Interface/PacketDefinition_Firehorn.h"
#elif RF_PROTOCOL_ICARUS
#include "../../commons/ERT_RF_Protocol_Interface/PacketDefinition_Icarus.h"
#endif

#if RF_PROTOCOL_FIREHORN
  /*process pkt before writing it
  this consist of adding id, adding timestamp, adding PacketType
  and returns the Packet ready to be written*/
  Packet process_pkt(av_uplink_t *avup, av_downlink_unpacked *avdw,
                     gse_downlink_t *gsdw);

  /*unprocess (only usefull to verify integrity of data in the unit tests)
  takes pkt, unprocess and writes it at the right place (avup, avdw or gsdw)*/
  void unprocess_pkt(Packet pkt, av_uplink_t *avup, av_downlink_unpacked *avdw,
                     gse_downlink_t *gsdw);
#endif

#if RF_PROTOCOL_ICARUS
  /*process pkt before writing it
  this consist of adding id, adding timestamp, adding PacketType
  and returns the Packet ready to be written*/
  Packet process_pkt(av_uplink_t *avup, av_downlink_t *avdw,
                     gse_downlink_t *gsdw);

  /*unprocess (only usefull to verify integrity of data in the unit tests)
  takes pkt, unprocess and writes it at the right place (avup, avdw or gsdw)*/
  void unprocess_pkt(Packet pkt, av_uplink_t *avup, av_downlink_t *avdw,
                     gse_downlink_t *gsdw);
#endif

private:
  uint32_t pkt_id_avup;

  uint32_t pkt_id_avdw;

  uint32_t pkt_id_gsdw;

  const size_t BATCH_SIZE = 6; // value to be reconsidered

  const char *PATH_TO_DB = "test.db";

  std::vector<AV_uplink_pkt *> buffer_av_up;

  std::vector<AV_downlink_pkt *> buffer_av_down;

  std::vector<GSE_downlink_pkt *> buffer_gse_down;

  uint32_t get_pkt_id(PacketType type);

  /*timestamp format : RFC3339 with miliseconds*/
  std::string get_current_ts();

  /*return value : flush buffer (0), buffer already empty (1)*/
  int flushAvUp();

  int flushAvDown();

  int flushGseDown();

  void ensureArchiveDir(const std::string &projectRoot);

#if RF_PROTOCOL_FIREHORN
  using Storage = decltype(sqlite_orm::make_storage(
      PATH_TO_DB,
      sqlite_orm::make_table<AV_uplink_pkt>(
          "AV_UPLINK",
          sqlite_orm::make_column("id", &AV_uplink_pkt::id,
                                  sqlite_orm::primary_key()),
          sqlite_orm::make_column("ts", &AV_uplink_pkt::ts),
          sqlite_orm::make_column("order_id", &AV_uplink_pkt::order_id),
          sqlite_orm::make_column("order_value", &AV_uplink_pkt::order_value)),
      sqlite_orm::make_table<AV_downlink_pkt>(
          "AV_DOWNLINK",
          sqlite_orm::make_column("id", &AV_downlink_pkt::id,
                                  sqlite_orm::primary_key()),
          sqlite_orm::make_column("ts", &AV_downlink_pkt::ts),
          sqlite_orm::make_column("packet_nbr", &AV_downlink_pkt::packet_nbr),
          sqlite_orm::make_column("gnss_lon", &AV_downlink_pkt::gnss_lon),
          sqlite_orm::make_column("gnss_lat", &AV_downlink_pkt::gnss_lat),
          sqlite_orm::make_column("gnss_alt", &AV_downlink_pkt::gnss_alt),
          sqlite_orm::make_column("gnss_vertical_speed",
                                  &AV_downlink_pkt::gnss_vertical_speed),
          sqlite_orm::make_column("N2_pressure", &AV_downlink_pkt::N2_pressure),
          sqlite_orm::make_column("fuel_pressure",
                                  &AV_downlink_pkt::fuel_pressure),
          sqlite_orm::make_column("LOX_pressure",
                                  &AV_downlink_pkt::LOX_pressure),
          sqlite_orm::make_column("fuel_level", &AV_downlink_pkt::fuel_level),
          sqlite_orm::make_column("LOX_level", &AV_downlink_pkt::LOX_level),
          sqlite_orm::make_column("N2_temp", &AV_downlink_pkt::N2_temp),
          sqlite_orm::make_column("LOX_temp", &AV_downlink_pkt::LOX_temp),
          sqlite_orm::make_column("LOX_inj_temp",
                                  &AV_downlink_pkt::LOX_inj_temp),
          sqlite_orm::make_column("lpb_voltage", &AV_downlink_pkt::lpb_voltage),
          sqlite_orm::make_column("hpb_voltage", &AV_downlink_pkt::hpb_voltage),
          sqlite_orm::make_column("av_fc_temp", &AV_downlink_pkt::av_fc_temp),
          sqlite_orm::make_column("ambient_temp",
                                  &AV_downlink_pkt::ambient_temp),
          sqlite_orm::make_column("engine_state",
                                  &AV_downlink_pkt::engine_state),
          sqlite_orm::make_column("av_state", &AV_downlink_pkt::av_state),
          sqlite_orm::make_column("cam_rec", &AV_downlink_pkt::cam_rec)),
      sqlite_orm::make_table<GSE_downlink_pkt>(
          "GSE_DOWNLINK",
          sqlite_orm::make_column("id", &GSE_downlink_pkt::id,
                                  sqlite_orm::primary_key()),
          sqlite_orm::make_column("ts", &GSE_downlink_pkt::ts),
          sqlite_orm::make_column("GQN_NC1", &GSE_downlink_pkt::GQN_NC1),
          sqlite_orm::make_column("GQN_NC2", &GSE_downlink_pkt::GQN_NC2),
          sqlite_orm::make_column("GQN_NC3", &GSE_downlink_pkt::GQN_NC3),
          sqlite_orm::make_column("GQN_NC4", &GSE_downlink_pkt::GQN_NC4),
          sqlite_orm::make_column("GQN_NC5", &GSE_downlink_pkt::GQN_NC5),
          sqlite_orm::make_column("GPN_NC1", &GSE_downlink_pkt::GPN_NC1),
          sqlite_orm::make_column("GPN_NC2", &GSE_downlink_pkt::GPN_NC2),
          sqlite_orm::make_column("GVN_NC", &GSE_downlink_pkt::GVN_NC),
          sqlite_orm::make_column("GFE_NC", &GSE_downlink_pkt::GFE_NC),
          sqlite_orm::make_column("GFO_NCC", &GSE_downlink_pkt::GFO_NCC),
          sqlite_orm::make_column("GDO_NCC", &GSE_downlink_pkt::GDO_NCC),
          sqlite_orm::make_column("PC_OLC", &GSE_downlink_pkt::PC_OLC),
          sqlite_orm::make_column("GP1", &GSE_downlink_pkt::GP1),
          sqlite_orm::make_column("GP2", &GSE_downlink_pkt::GP2),
          sqlite_orm::make_column("GP3", &GSE_downlink_pkt::GP3),
          sqlite_orm::make_column("GP4", &GSE_downlink_pkt::GP4),
          sqlite_orm::make_column("GP5", &GSE_downlink_pkt::GP5))));
#else
  using Storage = decltype(sqlite_orm::make_storage(
      PATH_TO_DB,
      sqlite_orm::make_table<AV_uplink_pkt>(
          "AV_UPLINK",
          sqlite_orm::make_column("id", &AV_uplink_pkt::id,
                                  sqlite_orm::primary_key()),
          sqlite_orm::make_column("ts", &AV_uplink_pkt::ts),
          sqlite_orm::make_column("order_id", &AV_uplink_pkt::order_id),
          sqlite_orm::make_column("order_value", &AV_uplink_pkt::order_value)),
      sqlite_orm::make_table<AV_downlink_pkt>(
          "AV_DOWNLINK",
          sqlite_orm::make_column("id", &AV_downlink_pkt::id,
                                  sqlite_orm::primary_key()),
          sqlite_orm::make_column("ts", &AV_downlink_pkt::ts),
          sqlite_orm::make_column("packet_nbr", &AV_downlink_pkt::packet_nbr),
          sqlite_orm::make_column("N2O_pressure",
                                  &AV_downlink_pkt::N2O_pressure),
          sqlite_orm::make_column("ETH_pressure",
                                  &AV_downlink_pkt::ETH_pressure),
          sqlite_orm::make_column("N2O_temp", &AV_downlink_pkt::N2O_temp),
          sqlite_orm::make_column("N2O_vent", &AV_downlink_pkt::N2O_vent),
          sqlite_orm::make_column("ETH_vent", &AV_downlink_pkt::ETH_vent),
          sqlite_orm::make_column("N2_solenoid", &AV_downlink_pkt::N2_solenoid),
          sqlite_orm::make_column("N2O_main", &AV_downlink_pkt::N2O_main),
          sqlite_orm::make_column("ETH_main", &AV_downlink_pkt::ETH_main),
          sqlite_orm::make_column("N2O_sol", &AV_downlink_pkt::N2O_sol),
          sqlite_orm::make_column("ETH_sol", &AV_downlink_pkt::ETH_sol),

          sqlite_orm::make_column("gnss_lon", &AV_downlink_pkt::gnss_lon),
          sqlite_orm::make_column("gnss_lat", &AV_downlink_pkt::gnss_lat),
          sqlite_orm::make_column("sat_nbr", &AV_downlink_pkt::sat_nbr),
          sqlite_orm::make_column("gyro_x", &AV_downlink_pkt::gyro_x),
          sqlite_orm::make_column("gyro_y", &AV_downlink_pkt::gyro_y),
          sqlite_orm::make_column("gyro_z", &AV_downlink_pkt::gyro_z),
          sqlite_orm::make_column("acc_x", &AV_downlink_pkt::acc_x),
          sqlite_orm::make_column("acc_y", &AV_downlink_pkt::acc_y),
          sqlite_orm::make_column("acc_z", &AV_downlink_pkt::acc_z),
          sqlite_orm::make_column("baro", &AV_downlink_pkt::baro),
          sqlite_orm::make_column("kalman_pos_x",
                                  &AV_downlink_pkt::kalman_pos_x),
          sqlite_orm::make_column("kalman_pos_y",
                                  &AV_downlink_pkt::kalman_pos_y),
          sqlite_orm::make_column("kalman_pos_z",
                                  &AV_downlink_pkt::kalman_pos_z),
          sqlite_orm::make_column("kalman_yaw", &AV_downlink_pkt::kalman_yaw),
          sqlite_orm::make_column("kalman_pitch",
                                  &AV_downlink_pkt::kalman_pitch),
          sqlite_orm::make_column("kalman_roll", &AV_downlink_pkt::kalman_roll),
          sqlite_orm::make_column("gimbal_x", &AV_downlink_pkt::gimbal_x),
          sqlite_orm::make_column("gimbal_y", &AV_downlink_pkt::gimbal_y),
          sqlite_orm::make_column("HV_voltage", &AV_downlink_pkt::HV_voltage),
          sqlite_orm::make_column("LV_voltage", &AV_downlink_pkt::LV_voltage),
          sqlite_orm::make_column("AV_temp", &AV_downlink_pkt::AV_temp),
          sqlite_orm::make_column("ID_config", &AV_downlink_pkt::ID_config),
          sqlite_orm::make_column("AV_state", &AV_downlink_pkt::AV_state)),
      sqlite_orm::make_table<GSE_downlink_pkt>(
          "GSE_DOWNLINK",
          sqlite_orm::make_column("id", &GSE_downlink_pkt::id,
                                  sqlite_orm::primary_key()),
          sqlite_orm::make_column("ts", &GSE_downlink_pkt::ts),
          sqlite_orm::make_column("tankPressure",
                                  &GSE_downlink_pkt::tankPressure),
          sqlite_orm::make_column("tankTemperature",
                                  &GSE_downlink_pkt::tankTemperature),
          sqlite_orm::make_column("fillingPressure",
                                  &GSE_downlink_pkt::fillingPressure),
          sqlite_orm::make_column("fillingN2O", &GSE_downlink_pkt::fillingN2O),
          sqlite_orm::make_column("vent", &GSE_downlink_pkt::vent),
          sqlite_orm::make_column("disconnectActive",
                                  &GSE_downlink_pkt::disconnectActive),
          sqlite_orm::make_column("loadcell1", &GSE_downlink_pkt::loadcell1),
          sqlite_orm::make_column("loadcell2", &GSE_downlink_pkt::loadcell2),
          sqlite_orm::make_column("loadcell3", &GSE_downlink_pkt::loadcell3),
          sqlite_orm::make_column("loadcell4", &GSE_downlink_pkt::loadcell4))));
#endif

  Storage storage;
};

#endif /* SQLITEDB_H */
