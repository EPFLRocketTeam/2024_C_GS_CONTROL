#include "../include/data_storage.h"

SqliteDB::SqliteDB()
    : storage(sqlite_orm::make_storage(this->PATH_TO_DB,
    sqlite_orm::make_table<AV_uplink_pkt>("AV_UPLINK",
        sqlite_orm::make_column("id", &AV_uplink_pkt::id, sqlite_orm::primary_key()),
        sqlite_orm::make_column("ts", &AV_uplink_pkt::ts),
        sqlite_orm::make_column("order_id", &AV_uplink_pkt::order_id),
        sqlite_orm::make_column("order_value", &AV_uplink_pkt::order_value)
    ),
    sqlite_orm::make_table<AV_downlink_pkt>("AV_DOWNLINK",
        sqlite_orm::make_column("id", &AV_downlink_pkt::id, sqlite_orm::primary_key()),
        sqlite_orm::make_column("ts", &AV_downlink_pkt::ts),
        sqlite_orm::make_column("packet_nbr", &AV_downlink_pkt::packet_nbr),
        sqlite_orm::make_column("gnss_lon", &AV_downlink_pkt::gnss_lon),
        sqlite_orm::make_column("gnss_lat", &AV_downlink_pkt::gnss_lat),
        sqlite_orm::make_column("gnss_alt", &AV_downlink_pkt::gnss_alt),
        sqlite_orm::make_column("gnss_vertical_speed", &AV_downlink_pkt::gnss_vertical_speed),
        sqlite_orm::make_column("N2_pressure", &AV_downlink_pkt::N2_pressure),
        sqlite_orm::make_column("fuel_pressure", &AV_downlink_pkt::fuel_pressure),
        sqlite_orm::make_column("LOX_pressure", &AV_downlink_pkt::LOX_pressure),
        sqlite_orm::make_column("fuel_level", &AV_downlink_pkt::fuel_level),
        sqlite_orm::make_column("LOX_level", &AV_downlink_pkt::LOX_level),
        sqlite_orm::make_column("N2_temp", &AV_downlink_pkt::N2_temp),
        sqlite_orm::make_column("LOX_temp", &AV_downlink_pkt::LOX_temp),
        sqlite_orm::make_column("LOX_inj_temp", &AV_downlink_pkt::LOX_inj_temp),
        sqlite_orm::make_column("lpb_voltage", &AV_downlink_pkt::lpb_voltage),
        sqlite_orm::make_column("hpb_voltage", &AV_downlink_pkt::hpb_voltage),
        sqlite_orm::make_column("av_fc_temp", &AV_downlink_pkt::av_fc_temp),
        sqlite_orm::make_column("ambient_temp", &AV_downlink_pkt::ambient_temp),
        sqlite_orm::make_column("engine_state", &AV_downlink_pkt::engine_state),
        sqlite_orm::make_column("av_state", &AV_downlink_pkt::av_state),
        sqlite_orm::make_column("cam_rec", &AV_downlink_pkt::cam_rec)
    ),
    sqlite_orm::make_table<GSE_downlink_pkt>("GSE_DOWNLINK",
        sqlite_orm::make_column("id", &GSE_downlink_pkt::id, sqlite_orm::primary_key()),
        sqlite_orm::make_column("ts", &GSE_downlink_pkt::ts),
        sqlite_orm::make_column("tankPressure", &GSE_downlink_pkt::tankPressure),
        sqlite_orm::make_column("tankTemperature", &GSE_downlink_pkt::tankTemperature),
        sqlite_orm::make_column("fillingPressure", &GSE_downlink_pkt::fillingPressure),
        sqlite_orm::make_column("fillingN2O", &GSE_downlink_pkt::fillingN2O),
        sqlite_orm::make_column("vent", &GSE_downlink_pkt::vent),
        sqlite_orm::make_column("disconnectActive", &GSE_downlink_pkt::disconnectActive),
        sqlite_orm::make_column("loadcell_raw", &GSE_downlink_pkt::loadcell_raw)
    )
))
{
storage.sync_schema();
}

SqliteDB::~SqliteDB() {}

int SqliteDB::write_pkt(const Packet pkt) {

    switch(pkt.type) {
        case PacketType::AV_UPLINK: {
            AV_uplink_pkt* avUpPkt = pkt.av_up_pkt;
            if (avUpPkt == NULL) {return 2;}
            printf("place av up pkt in buffer\n");
            buffer_av_up.emplace_back(*avUpPkt);
            if(buffer_av_up.size() >= BATCH_SIZE) {
                printf("av up buffer is full --> call flush\n");
                flushAvUp();
                return 1;
            }
            break;
        }
        case PacketType::AV_DOWNLINK: {
            AV_downlink_pkt* avDownPkt = pkt.av_down_pkt;
            if (avDownPkt == NULL) {return 2;}
            printf("place av down pkt in buffer\n");
            buffer_av_down.emplace_back(*avDownPkt);
            if(buffer_av_down.size() >= BATCH_SIZE) {
                printf("av down buffer is full --> call flush\n");
                flushAvDown();
                return 1;
            }
            break;
        }
        case PacketType::GSE_DOWNLINK: {
            GSE_downlink_pkt* gseDownPkt = pkt.gse_down_pkt;
            if (gseDownPkt == NULL) {return 2;}
            printf("place gse down pkt in buffer\n");
            buffer_gse_down.emplace_back(*gseDownPkt);
            if(buffer_gse_down.size() >= BATCH_SIZE) {
                printf("gse down buffer is full --> call flush\n");
                flushGseDown();
                return 1;
            }
            break;
        }
    }
    return 0;
}

int SqliteDB::read_pkt(uint32_t pkt_id, Packet pkt) {}

int SqliteDB::flushAvUp() {
    if (buffer_av_up.empty()) {return 1;}

    printf("starting transaction\n");
    this->storage.transaction([this]() -> bool {
        for (const auto& pktavup : buffer_av_up) {
            this->storage.replace(pktavup);
        }
        return true;
    });
    buffer_av_up.clear();
    return 0;
}

int SqliteDB::flushAvDown() {
    if (buffer_av_down.empty()) {return 1;}

    printf("starting transaction\n");
    this->storage.transaction([this]() -> bool {
        for (const auto& pktavdown : buffer_av_down) {
            this->storage.replace(pktavdown);
        }
        return true;
    });
    buffer_av_down.clear();
    return 0;
}

int SqliteDB::flushGseDown() {
    if (buffer_gse_down.empty()) {return 1;}

    printf("starting transaction\n");
    this->storage.transaction([this]() -> bool {
        for (const auto& pktgsedown : buffer_gse_down) {
            this->storage.replace(pktgsedown);
        }
        return true;
    });
    buffer_gse_down.clear();
    return 0;
}

int SqliteDB::delete_database() {}
