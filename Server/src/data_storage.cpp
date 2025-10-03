#include "../include/data_storage.h"
#include "sqlite_orm.h"
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

SqliteDB::SqliteDB()
    : storage(
#if RF_PROTOCOL_FIREHORN
          sqlite_orm::make_storage(
              this->PATH_TO_DB,
              sqlite_orm::make_table<AV_uplink_pkt>(
                  "AV_UPLINK",
                  sqlite_orm::make_column("id", &AV_uplink_pkt::id,
                                          sqlite_orm::primary_key()),
                  sqlite_orm::make_column("ts", &AV_uplink_pkt::ts),
                  sqlite_orm::make_column("order_id", &AV_uplink_pkt::order_id),
                  sqlite_orm::make_column("order_value",
                                          &AV_uplink_pkt::order_value)),
              sqlite_orm::make_table<AV_downlink_pkt>(
                  "AV_DOWNLINK",
                  sqlite_orm::make_column("id", &AV_downlink_pkt::id,
                                          sqlite_orm::primary_key()),
                  sqlite_orm::make_column("ts", &AV_downlink_pkt::ts),
                  sqlite_orm::make_column("packet_nbr",
                                          &AV_downlink_pkt::packet_nbr),
                  sqlite_orm::make_column("gnss_lon",
                                          &AV_downlink_pkt::gnss_lon),
                  sqlite_orm::make_column("gnss_lat",
                                          &AV_downlink_pkt::gnss_lat),
                  sqlite_orm::make_column("gnss_alt",
                                          &AV_downlink_pkt::gnss_alt),
                  sqlite_orm::make_column(
                      "gnss_vertical_speed",
                      &AV_downlink_pkt::gnss_vertical_speed),
                  sqlite_orm::make_column("N2_pressure",
                                          &AV_downlink_pkt::N2_pressure),
                  sqlite_orm::make_column("fuel_pressure",
                                          &AV_downlink_pkt::fuel_pressure),
                  sqlite_orm::make_column("LOX_pressure",
                                          &AV_downlink_pkt::LOX_pressure),
                  sqlite_orm::make_column("fuel_inj_pressure",
                                          &AV_downlink_pkt::fuel_inj_pressure),
                  sqlite_orm::make_column("chamber_pressure",
                                          &AV_downlink_pkt::chamber_pressure),
                  sqlite_orm::make_column("lox_inj_pressure",
                                          &AV_downlink_pkt::LOX_inj_pressure),
                  sqlite_orm::make_column("N2_temp", &AV_downlink_pkt::N2_temp),
                  sqlite_orm::make_column("N2_PT1000_temp",
                                          &AV_downlink_pkt::N2_PT1000_temp),
                  sqlite_orm::make_column("LOX_temp",
                                          &AV_downlink_pkt::LOX_temp),
                  sqlite_orm::make_column("LOX_inj_temp",
                                          &AV_downlink_pkt::LOX_inj_temp),
                  sqlite_orm::make_column("lpb_voltage",
                                          &AV_downlink_pkt::lpb_voltage),
                  sqlite_orm::make_column("lpb_current",
                                          &AV_downlink_pkt::lpb_current),
                  sqlite_orm::make_column("hpb_voltage",
                                          &AV_downlink_pkt::hpb_voltage),
                  sqlite_orm::make_column("hpb_current",
                                          &AV_downlink_pkt::hpb_current),
                  sqlite_orm::make_column("av_fc_temp",
                                          &AV_downlink_pkt::av_fc_temp),
                  sqlite_orm::make_column("ambient_temp",
                                          &AV_downlink_pkt::ambient_temp),
                  sqlite_orm::make_column("engine_state",
                                          &AV_downlink_pkt::engine_state),
                  sqlite_orm::make_column("av_state",
                                          &AV_downlink_pkt::av_state),
                  sqlite_orm::make_column("cam_rec",
                                          &AV_downlink_pkt::cam_rec),
                  sqlite_orm::make_column("LOX_cap_fls_0",
                                          &AV_downlink_pkt::LOX_cap_fls_0),
                  sqlite_orm::make_column("LOX_fls_10",
                                          &AV_downlink_pkt::LOX_fls_10),
                  sqlite_orm::make_column("LOX_fls_50",
                                          &AV_downlink_pkt::LOX_fls_50),
                  sqlite_orm::make_column("LOX_fls_80",
                                          &AV_downlink_pkt::LOX_fls_80),
                  sqlite_orm::make_column("LOX_fls_90",
                                          &AV_downlink_pkt::LOX_fls_90)),
              sqlite_orm::make_table<GSE_downlink_pkt>(
                  "GSE_DOWNLINK",
                  sqlite_orm::make_column("id", &GSE_downlink_pkt::id,
                                          sqlite_orm::primary_key()),
                  sqlite_orm::make_column("ts", &GSE_downlink_pkt::ts),
                  sqlite_orm::make_column("GQN_NC1",
                                          &GSE_downlink_pkt::GQN_NC1),
                  sqlite_orm::make_column("GQN_NC2",
                                          &GSE_downlink_pkt::GQN_NC2),
                  sqlite_orm::make_column("GQN_NC3",
                                          &GSE_downlink_pkt::GQN_NC3),
                  sqlite_orm::make_column("GQN_NC4",
                                          &GSE_downlink_pkt::GQN_NC4),
                  sqlite_orm::make_column("GQN_NC5",
                                          &GSE_downlink_pkt::GQN_NC5),
                  sqlite_orm::make_column("GPN_NC1",
                                          &GSE_downlink_pkt::GPN_NC1),
                  sqlite_orm::make_column("GPN_NC2",
                                          &GSE_downlink_pkt::GPN_NC2),
                  sqlite_orm::make_column("GVN_NC", &GSE_downlink_pkt::GVN_NC),
                  sqlite_orm::make_column("GFE_NC", &GSE_downlink_pkt::GFE_NC),
                  sqlite_orm::make_column("GFO_NCC",
                                          &GSE_downlink_pkt::GFO_NCC),
                  sqlite_orm::make_column("GDO_NCC",
                                          &GSE_downlink_pkt::GDO_NCC),
                  sqlite_orm::make_column("PC_OLC", &GSE_downlink_pkt::PC_OLC),
                  sqlite_orm::make_column("GP1", &GSE_downlink_pkt::GP1),
                  sqlite_orm::make_column("GP2", &GSE_downlink_pkt::GP2),
                  sqlite_orm::make_column("GP3", &GSE_downlink_pkt::GP3),
                  sqlite_orm::make_column("GP4", &GSE_downlink_pkt::GP4),
                  sqlite_orm::make_column("GP5", &GSE_downlink_pkt::GP5)))
#else
          sqlite_orm::make_storage(
              PATH_TO_DB,
              sqlite_orm::make_table<AV_uplink_pkt>(
                  "AV_UPLINK",
                  sqlite_orm::make_column("id", &AV_uplink_pkt::id,
                                          sqlite_orm::primary_key()),
                  sqlite_orm::make_column("ts", &AV_uplink_pkt::ts),
                  sqlite_orm::make_column("order_id", &AV_uplink_pkt::order_id),
                  sqlite_orm::make_column("order_value",
                                          &AV_uplink_pkt::order_value)),
              sqlite_orm::make_table<AV_downlink_pkt>(
                  "AV_DOWNLINK",
                  sqlite_orm::make_column("id", &AV_downlink_pkt::id,
                                          sqlite_orm::primary_key()),
                  sqlite_orm::make_column("ts", &AV_downlink_pkt::ts),
                  sqlite_orm::make_column("packet_nbr",
                                          &AV_downlink_pkt::packet_nbr),
                  sqlite_orm::make_column("N2O_pressure",
                                          &AV_downlink_pkt::N2O_pressure),
                  sqlite_orm::make_column("ETH_pressure",
                                          &AV_downlink_pkt::ETH_pressure),
                  sqlite_orm::make_column("N2O_temp",
                                          &AV_downlink_pkt::N2O_temp),
                  sqlite_orm::make_column("N2O_vent",
                                          &AV_downlink_pkt::N2O_vent),
                  sqlite_orm::make_column("ETH_vent",
                                          &AV_downlink_pkt::ETH_vent),
                  sqlite_orm::make_column("N2_solenoid",
                                          &AV_downlink_pkt::N2_solenoid),
                  sqlite_orm::make_column("N2O_main",
                                          &AV_downlink_pkt::N2O_main),
                  sqlite_orm::make_column("ETH_main",
                                          &AV_downlink_pkt::ETH_main),
                  sqlite_orm::make_column("N2O_sol", &AV_downlink_pkt::N2O_sol),
                  sqlite_orm::make_column("ETH_sol", &AV_downlink_pkt::ETH_sol),

                  sqlite_orm::make_column("N2O_igniter",
                                          &AV_downlink_pkt::N2O_igniter),
                  sqlite_orm::make_column("FUEL_igniter",
                                          &AV_downlink_pkt::FUEL_igniter),
                  sqlite_orm::make_column("igniter", &AV_downlink_pkt::igniter),
                  sqlite_orm::make_column("chamber_pressure",
                                          &AV_downlink_pkt::chamber_pressure),

                  sqlite_orm::make_column("gnss_lon",
                                          &AV_downlink_pkt::gnss_lon),
                  sqlite_orm::make_column("gnss_lat",
                                          &AV_downlink_pkt::gnss_lat),
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
                  sqlite_orm::make_column("kalman_yaw",
                                          &AV_downlink_pkt::kalman_yaw),
                  sqlite_orm::make_column("kalman_pitch",
                                          &AV_downlink_pkt::kalman_pitch),
                  sqlite_orm::make_column("kalman_roll",
                                          &AV_downlink_pkt::kalman_roll),
                  sqlite_orm::make_column("gimbal_x",
                                          &AV_downlink_pkt::gimbal_x),
                  sqlite_orm::make_column("gimbal_y",
                                          &AV_downlink_pkt::gimbal_y),
                  sqlite_orm::make_column("HV_voltage",
                                          &AV_downlink_pkt::HV_voltage),
                  sqlite_orm::make_column("LV_voltage",
                                          &AV_downlink_pkt::LV_voltage),
                  sqlite_orm::make_column("AV_temp", &AV_downlink_pkt::AV_temp),
                  sqlite_orm::make_column("ID_config",
                                          &AV_downlink_pkt::ID_config),
                  sqlite_orm::make_column("AV_state",
                                          &AV_downlink_pkt::AV_state)),
              /*sqlite_orm::make_table<GSE_downlink_pkt>(*/
              /*    "GSE_DOWNLINK",*/
              /*    sqlite_orm::make_column("id", &GSE_downlink_pkt::id,*/
              /*                            sqlite_orm::primary_key()),*/
              /*    sqlite_orm::make_column("ts", &GSE_downlink_pkt::ts),*/
              /*    sqlite_orm::make_column("tankPressure",*/
              /*                            &GSE_downlink_pkt::tankPressure),*/
              /*    sqlite_orm::make_column("tankTemperature",*/
              /*                            &GSE_downlink_pkt::tankTemperature),*/
              /*    sqlite_orm::make_column("fillingPressure",*/
              /*                            &GSE_downlink_pkt::fillingPressure),*/
              /*    sqlite_orm::make_column("fillingN2O",*/
              /*                            &GSE_downlink_pkt::fillingN2O),*/
              /*    sqlite_orm::make_column("vent", &GSE_downlink_pkt::vent),*/
              /*    sqlite_orm::make_column("disconnectActive",*/
              /*                            &GSE_downlink_pkt::disconnectActive),*/
              /*    sqlite_orm::make_column("loadcell1",*/
              /*                            &GSE_downlink_pkt::loadcell1),*/
              /*    sqlite_orm::make_column("loadcell2",*/
              /*                            &GSE_downlink_pkt::loadcell2),*/
              /*    sqlite_orm::make_column("loadcell3",*/
              /*                            &GSE_downlink_pkt::loadcell3),*/
              /*    sqlite_orm::make_column("loadcell4",*/
              /*                            &GSE_downlink_pkt::loadcell4)))*/
              sqlite_orm::make_table<GSE_downlink_pkt>(
                  "GSE_DOWNLINK",
                  sqlite_orm::make_column("id", &GSE_downlink_pkt::id,
                                          sqlite_orm::primary_key()),
                  sqlite_orm::make_column("ts", &GSE_downlink_pkt::ts),
                  sqlite_orm::make_column("GQN_NC1",
                                          &GSE_downlink_pkt::GQN_NC1),
                  sqlite_orm::make_column("GQN_NC2",
                                          &GSE_downlink_pkt::GQN_NC2),
                  sqlite_orm::make_column("GQN_NC3",
                                          &GSE_downlink_pkt::GQN_NC3),
                  sqlite_orm::make_column("GQN_NC4",
                                          &GSE_downlink_pkt::GQN_NC4),
                  sqlite_orm::make_column("GQN_NC5",
                                          &GSE_downlink_pkt::GQN_NC5),
                  sqlite_orm::make_column("GPN_NC1",
                                          &GSE_downlink_pkt::GPN_NC1),
                  sqlite_orm::make_column("GPN_NC2",
                                          &GSE_downlink_pkt::GPN_NC2),
                  sqlite_orm::make_column("GVN_NC", &GSE_downlink_pkt::GVN_NC),
                  sqlite_orm::make_column("GFE_NC", &GSE_downlink_pkt::GFE_NC),
                  sqlite_orm::make_column("GFO_NCC",
                                          &GSE_downlink_pkt::GFO_NCC),
                  sqlite_orm::make_column("GDO_NCC",
                                          &GSE_downlink_pkt::GDO_NCC),
                  sqlite_orm::make_column("PC_OLC", &GSE_downlink_pkt::PC_OLC),
                  sqlite_orm::make_column("GP1", &GSE_downlink_pkt::GP1),
                  sqlite_orm::make_column("GP2", &GSE_downlink_pkt::GP2),
                  sqlite_orm::make_column("GP3", &GSE_downlink_pkt::GP3),
                  sqlite_orm::make_column("GP4", &GSE_downlink_pkt::GP4),
                  sqlite_orm::make_column("GP5", &GSE_downlink_pkt::GP5)))

#endif
      ) {
  printf("creating db\n");
  storage.sync_schema(true);
  storage.pragma.journal_mode(sqlite_orm::journal_mode::WAL);
  this->pkt_id_avup = -1;
  this->pkt_id_avdw = -1;
  this->pkt_id_gsdw = -1;
}

void SqliteDB::ensureArchiveDir(const std::string &projectRoot) {
  // Compose the path: <projectRoot>/archives
  fs::path archiveDir = fs::path(projectRoot) / "archives";

  std::error_code ec;
  if (!fs::exists(archiveDir))
    fs::create_directories(archiveDir, ec);
}

SqliteDB::~SqliteDB() {
  printf("destroying db\n");
  flushAvUp();
  flushAvDown();
  flushGseDown();
  ensureArchiveDir(PROJECT_ROOT_PATH);
#if RF_PROTOCOL_FIREHORN
  std::string proj_name = "Firehorn";
#else
  std::string proj_name = "Icarus";
#endif
  storage.make_backup_to(PROJECT_ROOT_PATH "/archives/archive-" + proj_name +
                         "-" + storage.current_timestamp() + ".db");
}

int SqliteDB::write_pkt(const Packet pkt) {
  printf("write_pkt called\n");
  switch (pkt.type) {
  case PacketType::AV_UPLINK: {
    AV_uplink_pkt *avUpPkt = pkt.av_up_pkt;
    if (avUpPkt == NULL) {
      return 2;
    }
    printf("    place av up pkt in buffer\n");
    buffer_av_up.emplace_back(avUpPkt);
    if (buffer_av_up.size() >= BATCH_SIZE) {
      printf("    av up buffer is full\n");
      flushAvUp();
      return 1;
    }
    break;
  }
  case PacketType::AV_DOWNLINK: {
    AV_downlink_pkt *avDownPkt = pkt.av_down_pkt;
    if (avDownPkt == NULL) {
      return 2;
    }
    printf("    place av down pkt in buffer\n");
    buffer_av_down.emplace_back(avDownPkt);
    last_valid_avdw_pkt = *avDownPkt;
    if (buffer_av_down.size() >= BATCH_SIZE) {
      printf("    av down buffer is full\n");
      flushAvDown();
      return 1;
    }
    break;
  }
  case PacketType::GSE_DOWNLINK: {
    GSE_downlink_pkt *gseDownPkt = pkt.gse_down_pkt;
    if (gseDownPkt == NULL) {
      return 2;
    }
    printf("    place gse down pkt in buffer\n");
    buffer_gse_down.emplace_back(gseDownPkt);
    if (buffer_gse_down.size() >= BATCH_SIZE) {
      printf("    gse down buffer is full\n");
      flushGseDown();
      return 1;
    }
    break;
  }
  }
  return 0;
}

Packet SqliteDB::read_last_av() {
  Packet pkt;
#ifdef RF_PROTOCOL_FIREHORN

  
  try {
    AV_downlink_pkt *content = new AV_downlink_pkt;
    uint32_t pktId = pkt_id_avdw;

    *content = last_valid_avdw_pkt;

    pkt = Packet{.type = AV_UPLINK,
                 .av_up_pkt = NULL,
                 .av_down_pkt = content,
                 .gse_down_pkt = NULL};
  } catch (const std::runtime_error &e) {
    std::cerr << "      trying to read at invalid pkt_id\n"
              << e.what() << std::endl;
  }
#endif
  return pkt;
}

Packet SqliteDB::read_pkt(PacketType type, uint32_t pkt_id) {
  printf("read_pkt called\n");
  Packet pkt;
  switch (type) {
  case PacketType::AV_UPLINK: {
    try {
      AV_uplink_pkt *content = new AV_uplink_pkt;
      *content = storage.get<AV_uplink_pkt>(pkt_id);
      pkt = {.type = AV_UPLINK,
             .av_up_pkt = content,
             .av_down_pkt = NULL,
             .gse_down_pkt = NULL};
    } catch (const std::runtime_error &e) {
      std::cerr << "      trying to read at invalid pkt_id\n"
                << e.what() << std::endl;
      break;
    }
    return pkt;
  }
  case PacketType::AV_DOWNLINK: {
    try {
      AV_downlink_pkt *content = new AV_downlink_pkt;
      *content = storage.get<AV_downlink_pkt>(pkt_id);
      pkt = {.type = AV_DOWNLINK,
             .av_up_pkt = NULL,
             .av_down_pkt = content,
             .gse_down_pkt = NULL};
    } catch (const std::runtime_error &e) {
      std::cerr << "      trying to read at invalid pkt_id\n"
                << e.what() << std::endl;
      break;
    }
    return pkt;
  }
  case PacketType::GSE_DOWNLINK: {
    try {
      GSE_downlink_pkt *content = new GSE_downlink_pkt;
      *content = storage.get<GSE_downlink_pkt>(pkt_id);
      pkt = {.type = GSE_DOWNLINK,
             .av_up_pkt = NULL,
             .av_down_pkt = NULL,
             .gse_down_pkt = content};
    } catch (const std::runtime_error &e) {
      std::cerr << "      trying to read at invalid pkt_id\n"
                << e.what() << std::endl;
      break;
    }
    return pkt;
  }
  }
  return {.type = INVALID,
          .av_up_pkt = NULL,
          .av_down_pkt = NULL,
          .gse_down_pkt = NULL};
}

std::vector<AV_uplink_pkt> SqliteDB::read_all_avup() {
  size_t nbrPkt = storage.count<AV_uplink_pkt>();
  std::vector<AV_uplink_pkt> avups;
  avups.resize(nbrPkt);
  avups = storage.get_all<AV_uplink_pkt>();
  return avups;
}
std::vector<AV_downlink_pkt> SqliteDB::read_all_avdw() {
  size_t nbrPkt = storage.count<AV_downlink_pkt>();
  std::vector<AV_downlink_pkt> avdws;
  avdws.resize(nbrPkt);
  avdws = storage.get_all<AV_downlink_pkt>();
  return avdws;
}
std::vector<GSE_downlink_pkt> SqliteDB::read_all_gsdw() {
  size_t nbrPkt = storage.count<GSE_downlink_pkt>();
  std::vector<GSE_downlink_pkt> gsdws;
  gsdws.resize(nbrPkt);
  gsdws = storage.get_all<GSE_downlink_pkt>();
  return gsdws;
}

int SqliteDB::flushAvUp() {
  printf("flushAvUp called\n");
  if (buffer_av_up.empty()) {
    return 1;
  }

  printf("    starting transaction\n");
  this->storage.transaction([this]() -> bool {
    for (const AV_uplink_pkt *pktavup : buffer_av_up) {
      this->storage.replace(*pktavup);
      delete pktavup;
    }
    return true;
  });
  buffer_av_up.clear();
  return 0;
}

int SqliteDB::flushAvDown() {
  printf("flushAvDown called\n");
  if (buffer_av_down.empty()) {
    return 1;
  }

  printf("    starting transaction\n");
  this->storage.transaction([this]() -> bool {
    for (const AV_downlink_pkt *pktavdown : buffer_av_down) {
      this->storage.replace(*pktavdown);
      delete pktavdown;
    }
    return true;
  });
  buffer_av_down.clear();
  return 0;
}

int SqliteDB::flushGseDown() {
  printf("flushGseDown called\n");
  if (buffer_gse_down.empty()) {
    return 1;
  }

  printf("    starting transaction\n");
  this->storage.transaction([this]() -> bool {
    for (const GSE_downlink_pkt *pktgsedown : buffer_gse_down) {
      this->storage.replace(*pktgsedown);
      delete pktgsedown;
    }
    return true;
  });
  buffer_gse_down.clear();
  return 0;
}

uint32_t SqliteDB::get_pkt_id(PacketType type) {
  switch (type) {
  case PacketType::AV_UPLINK: {
    return this->pkt_id_avup += 1;
  }
  case PacketType::AV_DOWNLINK: {
    return this->pkt_id_avdw += 1;
  }
  case PacketType::GSE_DOWNLINK: {
    return this->pkt_id_gsdw += 1;
  }
  }
  return 0;
}

std::string SqliteDB::get_current_ts() {
  auto now = std::chrono::system_clock::now();
  auto seconds = std::chrono::floor<std::chrono::seconds>(now);
  auto millis =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - seconds)
          .count();

  std::time_t time = std::chrono::system_clock::to_time_t(seconds);
  std::tm tm = *std::gmtime(&time);

  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
  oss << "." << std::setfill('0') << std::setw(3) << millis;
  oss << "Z";

  return oss.str();
}

Packet SqliteDB::process_pkt(av_uplink_t *avup,
#if RF_PROTOCOL_FIREHORN
                             av_downlink_unpacked_t *avdw,
#else
                             av_downlink_t *avdw,
#endif
                             gse_downlink_t *gsdw) {
  printf("process_pkt called\n");
  if (avup == NULL && avdw == NULL && gsdw == NULL) {
    return {.type = INVALID,
            .av_up_pkt = NULL,
            .av_down_pkt = NULL,
            .gse_down_pkt = NULL};
  }
  std::string ts = this->get_current_ts();
  // process pkt if av_uplink_t
  if (avdw == NULL && gsdw == NULL) {
    uint32_t id = this->get_pkt_id(AV_UPLINK);
    AV_uplink_pkt *raw_avup =
        static_cast<AV_uplink_pkt *>(malloc(sizeof(AV_uplink_pkt)));
    *raw_avup = {.id = id,
                 .ts = ts,
                 .order_id = avup->order_id,
                 .order_value = avup->order_value};
    return {.type = AV_UPLINK,
            .av_up_pkt = raw_avup,
            .av_down_pkt = NULL,
            .gse_down_pkt = NULL};
  }
  // process pkt if av_downlink_unpacked
  if (avup == NULL && gsdw == NULL) {
    uint32_t id = this->get_pkt_id(AV_DOWNLINK);
#if RF_PROTOCOL_FIREHORN
    AV_downlink_pkt *raw_avdw =
        new AV_downlink_pkt{
            .id = id,
            .ts = ts,
            .packet_nbr = avdw->packet_nbr,
            .gnss_lon = avdw->gnss_lon,
            .gnss_lat = avdw->gnss_lat,
            .gnss_alt = avdw->gnss_alt,
            .gnss_vertical_speed = avdw->gnss_vertical_speed,
            .N2_pressure = avdw->N2_pressure,
            .N2_temp = avdw->N2_temp,
            .N2_PT1000_temp = avdw->N2_PT1000_temp,
            .fuel_pressure = static_cast<float>(avdw->fuel_pressure),
            .LOX_pressure = static_cast<float>(avdw->LOX_pressure),
            .LOX_temp = avdw->LOX_temp,
            .LOX_inj_pressure =
                static_cast<float>(avdw->LOX_inj_pressure),
            .LOX_inj_temp = avdw->LOX_inj_temp,
            .fuel_inj_pressure =
                static_cast<float>(avdw->fuel_inj_pressure),
            .chamber_pressure =
                static_cast<float>(avdw->chamber_pressure),
            .engine_state = avdw->engine_state,
            .lpb_voltage = avdw->lpb_voltage,
            .lpb_current = avdw->lpb_current,
            .hpb_voltage = avdw->hpb_voltage,
            .hpb_current = avdw->hpb_current,
            .av_fc_temp = avdw->av_fc_temp,
            .ambient_temp = avdw->ambient_temp,
            .av_state = avdw->av_state,
            .cam_rec = avdw->cam_rec,
            .LOX_cap_fls_0 = avdw->LOX_cap_fls_0,
            .LOX_fls_10 = avdw->LOX_fls_10,
            .LOX_fls_50 = avdw->LOX_fls_50,
            .LOX_fls_80 = avdw->LOX_fls_80,
            .LOX_fls_90 = avdw->LOX_fls_90};
#else
    AV_downlink_pkt *raw_avdw = new AV_downlink_pkt{
        .id = id,
        .ts = ts,
        .packet_nbr = avdw->packet_nbr,
        .N2O_pressure = avdw->N2O_pressure,
        .ETH_pressure = avdw->ETH_pressure,
        .N2O_temp = avdw->N2O_temp,
        .N2O_vent = avdw->N2O_vent,
        .ETH_vent = avdw->ETH_vent,
        .N2_solenoid = avdw->N2_solenoid,
        .N2O_main = avdw->N2O_main,
        .ETH_main = avdw->ETH_main,
        .N2O_sol = avdw->N2O_sol,
        .ETH_sol = avdw->ETH_sol,
        .N2O_igniter = avdw->N2O_igniter,
        /*.FUEL_igniter = avdw->ETH_igniter,*/
        .igniter = avdw->igniter,
        .chamber_pressure = avdw->chamber_pressure,

        .gnss_lon = avdw->gnss_lon,
        .gnss_lat = avdw->gnss_lat,
        .sat_nbr = avdw->sat_nbr,
        .gyro_x = avdw->gyro_x,
        .gyro_y = avdw->gyro_y,
        .gyro_z = avdw->gyro_z,
        .acc_x = avdw->acc_x,
        .acc_y = avdw->acc_y,
        .acc_z = avdw->acc_z,
        .baro = avdw->baro,
        .kalman_pos_x = avdw->kalman_pos_x,
        .kalman_pos_y = avdw->kalman_pos_y,
        .kalman_pos_z = avdw->kalman_pos_z,
        .kalman_yaw = avdw->kalman_yaw,
        .kalman_pitch = avdw->kalman_pitch,
        .kalman_roll = avdw->kalman_roll,
        .gimbal_x = avdw->gimbal_x,
        .gimbal_y = avdw->gimbal_y,
        .HV_voltage = avdw->HV_voltage,
        .LV_voltage = avdw->LV_voltage,
        .AV_temp = avdw->AV_temp,
        .ID_config = avdw->ID_config,
        .AV_state = avdw->AV_state,
    };
    raw_avdw->FUEL_igniter = avdw->ETH_igniter;
#endif

    return (Packet){.type = AV_DOWNLINK,
                    .av_up_pkt = NULL,
                    .av_down_pkt = raw_avdw,
                    .gse_down_pkt = NULL};
  }
  // process pkt if gse_downlink_t
  if (avup == NULL && avdw == NULL) {
    uint32_t id = this->get_pkt_id(GSE_DOWNLINK);
    /*#if RF_PROTOCOL_FIREHORN*/
    GSE_downlink_pkt *raw_gsdw = new GSE_downlink_pkt{.id = id,
                                                      .ts = ts,
                                                      .GQN_NC1 = gsdw->GQN_NC1,
                                                      .GQN_NC2 = gsdw->GQN_NC2,
                                                      .GQN_NC3 = gsdw->GQN_NC3,
                                                      .GQN_NC4 = gsdw->GQN_NC4,
                                                      .GQN_NC5 = gsdw->GQN_NC5,
                                                      .GPN_NC1 = gsdw->GPN_NC1,
                                                      .GPN_NC2 = gsdw->GPN_NC2,
                                                      .GVN_NC = gsdw->GVN_NC,
                                                      .GFE_NC = gsdw->GFE_NC,
                                                      .GFO_NCC = gsdw->GFO_NCC,
                                                      .GDO_NCC = gsdw->GDO_NCC,
                                                      .PC_OLC = gsdw->PC_OLC,
                                                      .GP1 = gsdw->GP1,
                                                      .GP2 = gsdw->GP2,
                                                      .GP3 = gsdw->GP3,
                                                      .GP4 = gsdw->GP4,
                                                      .GP5 = gsdw->GP5};
    /*#else*/
    /*    GSE_downlink_pkt *raw_gsdw =*/
    /*        static_cast<fs_downlink_t *>(malloc(sizeof(fs_downlink_t)));*/
    /*    *raw_gsdw = {*/
    /*        .id = id,*/
    /*        .ts = ts,*/
    /*        .tankPressure = gsdw->tankPressure,*/
    /*        .tankTemperature = gsdw->tankTemperature,*/
    /*        .fillingPressure = gsdw->fillingPressure,*/
    /*        .fillingN2O = gsdw->status.fillingN2O,*/
    /*        .vent = gsdw->status.vent,*/
    /*        .disconnectActive = gsdw->disconnectActive,*/
    /*        .loadcell2 = gsdw->loadcell2,*/
    /*        .loadcell3 = gsdw->loadcell3,*/
    /*        .loadcell4 = gsdw->loadcell4,*/
    /*#endif*/
    return (Packet){.type = GSE_DOWNLINK,
                    .av_up_pkt = NULL,
                    .av_down_pkt = NULL,
                    .gse_down_pkt = raw_gsdw};
  }
  return {.type = INVALID,
          .av_up_pkt = NULL,
          .av_down_pkt = NULL,
          .gse_down_pkt = NULL};
}

void SqliteDB::unprocess_pkt(Packet pkt, av_uplink_t *avup,
#if RF_PROTOCOL_FIREHORN
                             av_downlink_unpacked_t *avdw,
#else
                             av_downlink_t *avdw,
#endif
                             gse_downlink_t *gsdw) {
  printf("unprocess_pkt called\n");
  switch (pkt.type) {
  case PacketType::AV_UPLINK: {
    *avup = (av_uplink_t){.order_id = pkt.av_up_pkt->order_id,
                          .order_value = pkt.av_up_pkt->order_value};
    break;
  }
  case PacketType::AV_DOWNLINK: {
#if RF_PROTOCOL_FIREHORN
    *avdw = (av_downlink_unpacked_t){
        .packet_nbr = pkt.av_down_pkt->packet_nbr,
        .gnss_lon = pkt.av_down_pkt->gnss_lon,
        .gnss_lat = pkt.av_down_pkt->gnss_lat,
        .gnss_alt = pkt.av_down_pkt->gnss_alt,
        .gnss_vertical_speed = pkt.av_down_pkt->gnss_vertical_speed,
        .N2_pressure = pkt.av_down_pkt->N2_pressure,
        .N2_temp = pkt.av_down_pkt->N2_temp,
        .N2_PT1000_temp = pkt.av_down_pkt->N2_PT1000_temp,
        .fuel_pressure =
            static_cast<uint8_t>(pkt.av_down_pkt->fuel_pressure),
        .LOX_pressure =
            static_cast<uint8_t>(pkt.av_down_pkt->LOX_pressure),
        .LOX_temp = pkt.av_down_pkt->LOX_temp,
        .LOX_inj_pressure = static_cast<uint8_t>(
            pkt.av_down_pkt->LOX_inj_pressure),
        .LOX_inj_temp = pkt.av_down_pkt->LOX_inj_temp,
        .fuel_inj_pressure = static_cast<uint8_t>(
            pkt.av_down_pkt->fuel_inj_pressure),
        .chamber_pressure = static_cast<uint16_t>(
            pkt.av_down_pkt->chamber_pressure),
        .engine_state = pkt.av_down_pkt->engine_state,
        .lpb_voltage = pkt.av_down_pkt->lpb_voltage,
        .lpb_current = pkt.av_down_pkt->lpb_current,
        .hpb_voltage = pkt.av_down_pkt->hpb_voltage,
        .hpb_current = pkt.av_down_pkt->hpb_current,
        .av_fc_temp = pkt.av_down_pkt->av_fc_temp,
        .ambient_temp = pkt.av_down_pkt->ambient_temp,
        .av_state = pkt.av_down_pkt->av_state,
        .cam_rec = pkt.av_down_pkt->cam_rec,
        .LOX_cap_fls_0 = pkt.av_down_pkt->LOX_cap_fls_0,
        .LOX_fls_10 = pkt.av_down_pkt->LOX_fls_10,
        .LOX_fls_50 = pkt.av_down_pkt->LOX_fls_50,
        .LOX_fls_80 = pkt.av_down_pkt->LOX_fls_80,
        .LOX_fls_90 = pkt.av_down_pkt->LOX_fls_90};

#else
    *avdw = (av_downlink_t){
        .packet_nbr = pkt.av_down_pkt->packet_nbr,

        .N2O_pressure = pkt.av_down_pkt->N2O_pressure,
        .ETH_pressure = pkt.av_down_pkt->ETH_pressure,
        .N2O_temp = pkt.av_down_pkt->N2O_temp,
        .N2O_vent = pkt.av_down_pkt->N2O_vent,
        .ETH_vent = pkt.av_down_pkt->ETH_vent,
        .N2_solenoid = pkt.av_down_pkt->N2_solenoid,
        .N2O_main = pkt.av_down_pkt->N2O_main,
        .ETH_main = pkt.av_down_pkt->ETH_main,
        .N2O_sol = pkt.av_down_pkt->N2O_sol,
        .ETH_sol = pkt.av_down_pkt->ETH_sol,
        .N2O_igniter = pkt.av_down_pkt->N2O_igniter,
        /*.ETH_igniter = pkt.av_down_pkt->FUEL_igniter,*/
        .igniter = pkt.av_down_pkt->igniter,
        .chamber_pressure = pkt.av_down_pkt->chamber_pressure,

        .gnss_lon = pkt.av_down_pkt->gnss_lon,
        .gnss_lat = pkt.av_down_pkt->gnss_lat,
        .sat_nbr = pkt.av_down_pkt->sat_nbr,

        .gyro_x = pkt.av_down_pkt->gyro_x,
        .gyro_y = pkt.av_down_pkt->gyro_y,
        .gyro_z = pkt.av_down_pkt->gyro_z,

        .acc_x = pkt.av_down_pkt->acc_x,
        .acc_y = pkt.av_down_pkt->acc_y,
        .acc_z = pkt.av_down_pkt->acc_z,

        .baro = pkt.av_down_pkt->baro,

        .kalman_pos_x = pkt.av_down_pkt->kalman_pos_x,
        .kalman_pos_y = pkt.av_down_pkt->kalman_pos_y,
        .kalman_pos_z = pkt.av_down_pkt->kalman_pos_z,
        .kalman_yaw = pkt.av_down_pkt->kalman_yaw,
        .kalman_pitch = pkt.av_down_pkt->kalman_pitch,
        .kalman_roll = pkt.av_down_pkt->kalman_roll,

        .gimbal_x = pkt.av_down_pkt->gimbal_x,
        .gimbal_y = pkt.av_down_pkt->gimbal_y,

        .HV_voltage = pkt.av_down_pkt->HV_voltage,
        .LV_voltage = pkt.av_down_pkt->LV_voltage,
        .AV_temp = pkt.av_down_pkt->AV_temp,
        .ID_config = pkt.av_down_pkt->ID_config,
        .AV_state = pkt.av_down_pkt->AV_state,
    };
    avdw->ETH_igniter = pkt.av_down_pkt->FUEL_igniter;
#endif
    break;
  }
  case PacketType::GSE_DOWNLINK: {
    /*#if RF_PROTOCOL_ICARUS*/
    /**/
    /*    GSE_cmd_status status =*/
    /*        (GSE_cmd_status){.fillingN2O = pkt.gse_down_pkt->fillingN2O,*/
    /*                         .vent = pkt.gse_down_pkt->vent};*/
    /*    *gsdw =*/
    /*        (gse_downlink_t){.tankPressure = pkt.gse_down_pkt->tankPressure,*/
    /*                         .tankTemperature =
     * pkt.gse_down_pkt->tankTemperature,*/
    /*                         .fillingPressure =
     * pkt.gse_down_pkt->fillingPressure,*/
    /*                         .status = status,*/
    /*                         .disconnectActive =
     * pkt.gse_down_pkt->disconnectActive,*/
    /*                         .loadcell1 = pkt.gse_down_pkt->loadcell1,*/
    /*                         .loadcell2 = pkt.gse_down_pkt->loadcell2,*/
    /*                         .loadcell3 = pkt.gse_down_pkt->loadcell3,*/
    /*                         .loadcell4 = pkt.gse_down_pkt->loadcell4};*/
    /*#else*/
    *gsdw = (gse_downlink_t){.GQN_NC1 = pkt.gse_down_pkt->GQN_NC1,
                             .GQN_NC2 = pkt.gse_down_pkt->GQN_NC2,
                             .GQN_NC3 = pkt.gse_down_pkt->GQN_NC3,
                             .GQN_NC4 = pkt.gse_down_pkt->GQN_NC4,
                             .GQN_NC5 = pkt.gse_down_pkt->GQN_NC5,
                             .GPN_NC1 = pkt.gse_down_pkt->GPN_NC1,
                             .GPN_NC2 = pkt.gse_down_pkt->GPN_NC2,
                             .GVN_NC = pkt.gse_down_pkt->GVN_NC,
                             .GFE_NC = pkt.gse_down_pkt->GFE_NC,
                             .GFO_NCC = pkt.gse_down_pkt->GFO_NCC,
                             .GDO_NCC = pkt.gse_down_pkt->GDO_NCC,
                             .PC_OLC = pkt.gse_down_pkt->PC_OLC,
                             .GP1 = pkt.gse_down_pkt->GP1,
                             .GP2 = pkt.gse_down_pkt->GP2,
                             .GP3 = pkt.gse_down_pkt->GP3,
                             .GP4 = pkt.gse_down_pkt->GP4,
                             .GP5 = pkt.gse_down_pkt->GP5};

    /*#endif*/
    break;
  }
  }
  return;
}

int SqliteDB::delete_database() { return 0; }
