#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include <string>
#include <sqlite3.h>
#include <vector>
#include "../../commons/ERT_RF_Protocol_Interface/PacketDefinition_Firehorn.h"

typedef enum { AV_UPLINK, AV_DOWNLINK, GSE_DOWNLINK } PacketType;

typedef struct {
    PacketType type;
    void *data;
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
		/*to be called one time at the initialisation of the server
		the constructor creates the sqlite3 DB where specified*/
		SqliteDB(std::string path_to_db);

		~SqliteDB();

		int write_pkt(Packet pkt);

		int read_pkt(uint32_t pkt_id, Packet pkt);

		int flush();
		
		/*sould not be called unless for the tests*/
		int delete_database(std::string path_to_db);
	private:
		uint32_t pkt_id;

		std::string path_to_db;

		sqlite3* db;

		const size_t BATCH_SIZE = 100;

		std::vector<Packet> buffer;

		uint32_t update_pkt_id();

		timespec get_current_ts();
	};
