#include "../include/data_storage.h"
#include "../../commons/ERT_RF_Protocol_Interface/PacketDefinition_Firehorn.h"
#include <chrono>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

SqliteDB::SqliteDB()
    : storage(sqlite_orm::make_storage(
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
              sqlite_orm::make_column("gnss_lon", &AV_downlink_pkt::gnss_lon),
              sqlite_orm::make_column("gnss_lat", &AV_downlink_pkt::gnss_lat),
              sqlite_orm::make_column("gnss_alt", &AV_downlink_pkt::gnss_alt),
              sqlite_orm::make_column("gnss_vertical_speed",
                                      &AV_downlink_pkt::gnss_vertical_speed),
              sqlite_orm::make_column("N2_pressure",
                                      &AV_downlink_pkt::N2_pressure),
              sqlite_orm::make_column("fuel_pressure",
                                      &AV_downlink_pkt::fuel_pressure),
              sqlite_orm::make_column("LOX_pressure",
                                      &AV_downlink_pkt::LOX_pressure),
              sqlite_orm::make_column("fuel_level",
                                      &AV_downlink_pkt::fuel_level),
              sqlite_orm::make_column("LOX_level", &AV_downlink_pkt::LOX_level),
              sqlite_orm::make_column("N2_temp", &AV_downlink_pkt::N2_temp),
              sqlite_orm::make_column("LOX_temp", &AV_downlink_pkt::LOX_temp),
              sqlite_orm::make_column("LOX_inj_temp",
                                      &AV_downlink_pkt::LOX_inj_temp),
              sqlite_orm::make_column("lpb_voltage",
                                      &AV_downlink_pkt::lpb_voltage),
              sqlite_orm::make_column("hpb_voltage",
                                      &AV_downlink_pkt::hpb_voltage),
              sqlite_orm::make_column("av_fc_temp",
                                      &AV_downlink_pkt::av_fc_temp),
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
              sqlite_orm::make_column("tankPressure",
                                      &GSE_downlink_pkt::tankPressure),
              sqlite_orm::make_column("tankTemperature",
                                      &GSE_downlink_pkt::tankTemperature),
              sqlite_orm::make_column("fillingPressure",
                                      &GSE_downlink_pkt::fillingPressure),
              sqlite_orm::make_column("fillingN2O",
                                      &GSE_downlink_pkt::fillingN2O),
              sqlite_orm::make_column("vent", &GSE_downlink_pkt::vent),
              sqlite_orm::make_column("disconnectActive",
                                      &GSE_downlink_pkt::disconnectActive),
              sqlite_orm::make_column("loadcell_raw",
                                      &GSE_downlink_pkt::loadcell_raw)))) {
  printf("creating db\n");
  storage.sync_schema();
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
  storage.make_backup_to(PROJECT_ROOT_PATH "/archives/archive-" + storage.current_timestamp() + ".db");
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
  return NULL;
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

Packet SqliteDB::process_pkt(av_uplink_t *avup, av_downlink_unpacked *avdw,
                             PacketGSE_downlink *gsdw) {
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
    AV_downlink_pkt *raw_avdw =
        static_cast<AV_downlink_pkt *>(malloc(sizeof(AV_downlink_pkt)));
    *raw_avdw = {.id = id,
                 .ts = ts,
                 .packet_nbr = avdw->packet_nbr,
                 .gnss_lon = avdw->gnss_lon,
                 .gnss_lat = avdw->gnss_lat,
                 .gnss_alt = avdw->gnss_alt,
                 .gnss_vertical_speed = avdw->gnss_vertical_speed,
                 .N2_pressure = avdw->N2_pressure,
                 .fuel_pressure = avdw->fuel_pressure,
                 .LOX_pressure = avdw->LOX_pressure,
                 .fuel_level = avdw->fuel_level,
                 .LOX_level = avdw->LOX_level,
                 .N2_temp = avdw->N2_temp,
                 .LOX_temp = avdw->LOX_temp,
                 .LOX_inj_temp = avdw->LOX_inj_temp,
                 .lpb_voltage = avdw->lpb_voltage,
                 .hpb_voltage = avdw->hpb_voltage,
                 .av_fc_temp = avdw->av_fc_temp,
                 .ambient_temp = avdw->ambient_temp,
                 .engine_state = avdw->engine_state,
                 .av_state = avdw->av_state,
                 .cam_rec = avdw->cam_rec};
    return (Packet){.type = AV_DOWNLINK,
                    .av_up_pkt = NULL,
                    .av_down_pkt = raw_avdw,
                    .gse_down_pkt = NULL};
  }
  // process pkt if PacketGSE_downlink
  if (avup == NULL && avdw == NULL) {
    uint32_t id = this->get_pkt_id(GSE_DOWNLINK);
    GSE_downlink_pkt *raw_gsdw =
        static_cast<GSE_downlink_pkt *>(malloc(sizeof(GSE_downlink_pkt)));
    *raw_gsdw = {.id = id,
                 .ts = ts,
                 .tankPressure = gsdw->tankPressure,
                 .tankTemperature = gsdw->tankTemperature,
                 .fillingPressure = gsdw->fillingPressure,
                 .fillingN2O = gsdw->status.fillingN2O,
                 .vent = gsdw->status.vent,
                 .disconnectActive = gsdw->disconnectActive,
                 .loadcell_raw = gsdw->loadcell_raw};
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
                             av_downlink_unpacked *avdw,
                             PacketGSE_downlink *gsdw) {
  printf("unprocess_pkt called\n");
  switch (pkt.type) {
  case PacketType::AV_UPLINK: {
    *avup = (av_uplink_t){.order_id = pkt.av_up_pkt->order_id,
                          .order_value = pkt.av_up_pkt->order_value};
    break;
  }
  case PacketType::AV_DOWNLINK: {
    *avdw = (av_downlink_unpacked){
        .packet_nbr = pkt.av_down_pkt->packet_nbr,
        .gnss_lon = pkt.av_down_pkt->gnss_lon,
        .gnss_lat = pkt.av_down_pkt->gnss_lat,
        .gnss_alt = pkt.av_down_pkt->gnss_alt,
        .gnss_vertical_speed = pkt.av_down_pkt->gnss_vertical_speed,
        .N2_pressure = pkt.av_down_pkt->N2_pressure,
        .fuel_pressure = pkt.av_down_pkt->fuel_pressure,
        .LOX_pressure = pkt.av_down_pkt->LOX_pressure,
        .fuel_level = pkt.av_down_pkt->fuel_level,
        .LOX_level = pkt.av_down_pkt->LOX_level,
        .N2_temp = pkt.av_down_pkt->N2_temp,
        .LOX_temp = pkt.av_down_pkt->LOX_temp,
        .LOX_inj_temp = pkt.av_down_pkt->LOX_inj_temp,
        .lpb_voltage = pkt.av_down_pkt->lpb_voltage,
        .hpb_voltage = pkt.av_down_pkt->hpb_voltage,
        .av_fc_temp = pkt.av_down_pkt->av_fc_temp,
        .ambient_temp = pkt.av_down_pkt->ambient_temp,
        .engine_state = pkt.av_down_pkt->engine_state,
        .av_state = pkt.av_down_pkt->av_state,
        .cam_rec = pkt.av_down_pkt->cam_rec};
    break;
  }
  case PacketType::GSE_DOWNLINK: {
    GSE_cmd_status status =
        (GSE_cmd_status){.fillingN2O = pkt.gse_down_pkt->fillingN2O,
                         .vent = pkt.gse_down_pkt->vent};
    *gsdw = (PacketGSE_downlink){
        .tankPressure = pkt.gse_down_pkt->tankPressure,
        .tankTemperature = pkt.gse_down_pkt->tankTemperature,
        .fillingPressure = pkt.gse_down_pkt->fillingPressure,
        .status = status,
        .disconnectActive = pkt.gse_down_pkt->disconnectActive,
        .loadcell_raw = pkt.gse_down_pkt->loadcell_raw};
    break;
  }
  }
  return;
}

int SqliteDB::delete_database() {}
