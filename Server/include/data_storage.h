#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <string>
#include <sqlite3.h>
#include <vector>
#include "sqlite_orm.h"
#include "../../commons/ERT_RF_Protocol_Interface/PacketDefinition_Firehorn.h"

typedef enum { AV_UPLINK, AV_DOWNLINK, GSE_DOWNLINK } PacketType;

typedef struct {
    PacketType type;
    AV_uplink_pkt* av_up_pkt;
	AV_downlink_pkt* av_down_pkt;
	GSE_downlink_pkt* gse_down_pkt;
} Packet;

struct timespec ts;

struct AV_uplink_pkt {
    uint32_t id;
    timespec ts;
    uint8_t order_id;
	uint8_t order_value;
};

struct AV_downlink_pkt {
    uint32_t id;
    timespec ts;
    uint32_t packet_nbr;
	int32_t	 gnss_lon;
	int32_t	 gnss_lat;
	uint16_t gnss_alt;
	int8_t 	 gnss_vertical_speed;
	uint16_t N2_pressure;
	uint16_t fuel_pressure;
	uint16_t LOX_pressure;
	uint8_t  fuel_level;
	uint8_t  LOX_level;
	int16_t  N2_temp;
	int16_t  LOX_temp;
	int16_t  LOX_inj_temp;
	uint8_t  lpb_voltage;
	uint8_t  hpb_voltage;
	int16_t  av_fc_temp;
	int16_t  ambient_temp;
	uint8_t  engine_state;
	uint8_t  av_state;
	uint8_t  cam_rec;
};

struct GSE_downlink_pkt {
    uint32_t id;
    timespec ts;
    float tankPressure;
	float tankTemperature;
	float fillingPressure;
    uint8_t fillingN2O;
	uint8_t vent;
	bool disconnectActive;
	uint32_t loadcell_raw;
};

class SqliteDB {
	public:
		/*to be called one time at the initialisation of the server*/
		SqliteDB();

		~SqliteDB();

		/*add to buffer (0); add to buffer and flush (1); packet type specified is NULL (2)*/
		int write_pkt(const Packet pkt);

		/*put null pointer for unused packet types*/
		int read_pkt(uint32_t pkt_id, Packet pkt);

		int flush();
		
		/*sould not be called unless for the tests*/
		int delete_database();

	private:
		uint32_t pkt_id;

		std::string path_to_db;

		sqlite3* db;

		const size_t BATCH_SIZE = 100;

		const char* PATH_TO_DB = "../../";

		std::vector<AV_uplink_pkt> buffer_av_up;

		std::vector<AV_downlink_pkt> buffer_av_down;

		std::vector<GSE_downlink_pkt> buffer_gse_down;

		uint32_t update_pkt_id();

		timespec get_current_ts();

		using Storage = decltype(sqlite_orm::make_storage(PATH_TO_DB,
			sqlite_orm::make_table<Packet>("AV_UPLINK",
				sqlite_orm::make_column("id", &AV_uplink_pkt::id, sqlite_orm::primary_key()),
				sqlite_orm::make_column("ts", &AV_uplink_pkt::ts),
				sqlite_orm::make_column("order_id", &AV_uplink_pkt::order_id),
				sqlite_orm::make_column("order_value", &AV_uplink_pkt::order_value)
			),
			sqlite_orm::make_table<Packet>("AV_DOWNLINK",
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
			sqlite_orm::make_table<Packet>("GSE_DOWNLINK",
				sqlite_orm::make_column("id", &GSE_downlink_pkt::id, sqlite_orm::primary_key()),
				sqlite_orm::make_column("ts", &GSE_downlink_pkt::ts),
				sqlite_orm::make_column("tankPressure", &GSE_downlink_pkt::tankPressure),
				sqlite_orm::make_column("tankTemperature", &GSE_downlink_pkt::tankTemperature),
				sqlite_orm::make_column("fillingPressure", &GSE_downlink_pkt::fillingPressure),
				sqlite_orm::make_column("fillingN2O", &GSE_downlink_pkt::fillingN2O),
				sqlite_orm::make_column("vent", &GSE_downlink_pkt::vent),
				sqlite_orm::make_column("disconnectActive", &GSE_downlink_pkt::disconnectActive),
				sqlite_orm::make_column("loadcell_raw", &GSE_downlink_pkt::loadcell_raw)
			)));

			Storage* storage;
	};
