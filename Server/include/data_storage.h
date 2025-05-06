#include <stdint.h>
#include <stddef.h>
#include <time.h>
#include "../../commons/ERT_RF_Protocol_Interface/PacketDefinition_Firehorn.h"

struct timespec ts;

struct AV_uplink_pkt {
    uint8_t id;
    timespec ts;
    uint8_t order_id;
	uint8_t order_value;
};

struct AV_downlink_pkt {
    uint8_t id;
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
    uint8_t id;
    timespec ts;
    float tankPressure;
	float tankTemperature;
	float fillingPressure;
    uint8_t fillingN2O;
	uint8_t vent;
	bool disconnectActive;
	uint32_t loadcell_raw;
};
/*
struct GSE_cmd_pkt {
    uint8_t id;
    timespec ts;
    
};*/

class SqliteDB {
	public:
		int create_database();

		int write_AV_uplink_pkt(AV_uplink_pkt* pkt_ptr);
		
		int write_AV_downlink_pkt(AV_downlink_pkt* pkt_ptr);
		
		int write_GSE_downlink_pkt(GSE_downlink_pkt* pkt_ptr);
		
		//int write_GSE_cmd_pkt();
		
		int read_AV_uplink_pkt(AV_uplink_pkt* pkt_ptr);
		
		int read_AV_downlink_pkt(AV_downlink_pkt* pkt_ptr);
		
		int read_GSE_downlink_pkt(GSE_downlink_pkt* pkt_ptr);
		
		//int read_GSE_cmd_pkt();
		
		int delete_database();
	private:
		uint32_t pkt_id;

		uint32_t get_next_pkt_id();
	};

