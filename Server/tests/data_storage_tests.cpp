#include "../include/data_storage.h"
#include <gtest/gtest.h>

//function to create test db
SqliteDB* get_db() {
    SqliteDB* db = new SqliteDB;
    return db;
}

//functions to create test packets
av_uplink_t* get_avup() {
    av_uplink_t* avup = new av_uplink_t;
    *avup = {.order_id=1, .order_value=2};
    return avup;
}
av_downlink_t* get_avdw() {
    av_downlink_t* avdw = new av_downlink_t;
    *avdw = {.packet_nbr=9, .gnss_lon=10,
        .gnss_lat=11, .gnss_alt=12, .gnss_vertical_speed=13,
        .N2_pressure=14, .fuel_pressure=15, .LOX_pressure=16,
        .fuel_level=17, .LOX_level=18, .N2_temp=19, .LOX_temp=20,
        .LOX_inj_temp=21, .lpb_voltage=22, .hpb_voltage=23,
        .av_fc_temp=24, .ambient_temp=25, .engine_state=26,
        .av_state=27, .cam_rec=28};
    return avdw;
}
PacketGSE_downlink* get_gsdw() {
    GSE_cmd_status* status = new GSE_cmd_status;
    *status = {.fillingN2O=12, .vent=13};
    PacketGSE_downlink* gsdw = new PacketGSE_downlink;
    *gsdw = {.tankPressure=11.123456789, .tankTemperature=11.2,
        .fillingPressure=11.3, *status,
        .disconnectActive=true, .loadcell_raw=14};
    return gsdw;
}

//functions to compare the content of two pkts
void equal_avup(av_uplink_t* avup1, av_uplink_t* avup2) {
    printf("    o_id 1 = %d\n", avup1->order_id);
    printf("    o_id 2 = %d\n", avup2->order_id);
    EXPECT_EQ(avup1->order_id, avup2->order_id);
    EXPECT_EQ(avup1->order_value, avup2->order_value);
}
void equal_avdw(av_downlink_t* avdw1, av_downlink_t* avdw2) {
    EXPECT_EQ(avdw1->ambient_temp, avdw2->ambient_temp);
    EXPECT_EQ(avdw1->av_fc_temp, avdw2->av_fc_temp);
    EXPECT_EQ(avdw1->av_state, avdw2->av_state);
    EXPECT_EQ(avdw1->cam_rec, avdw2->cam_rec);
    EXPECT_EQ(avdw1->engine_state, avdw2->engine_state);
    EXPECT_EQ(avdw1->fuel_level, avdw2->fuel_level);
    EXPECT_EQ(avdw1->fuel_pressure, avdw2->fuel_pressure);
    EXPECT_EQ(avdw1->gnss_alt, avdw2->gnss_alt);
    EXPECT_EQ(avdw1->gnss_lat, avdw2->gnss_lat);
    EXPECT_EQ(avdw1->gnss_lon, avdw2->gnss_lon);
    EXPECT_EQ(avdw1->gnss_vertical_speed, avdw2->gnss_vertical_speed);
    EXPECT_EQ(avdw1->hpb_voltage, avdw2->hpb_voltage);
    EXPECT_EQ(avdw1->LOX_inj_temp, avdw2->LOX_inj_temp);
    EXPECT_EQ(avdw1->LOX_level, avdw2->LOX_level);
    EXPECT_EQ(avdw1->LOX_pressure, avdw2->LOX_pressure);
    EXPECT_EQ(avdw1->LOX_temp, avdw2->LOX_temp);
    EXPECT_EQ(avdw1->lpb_voltage, avdw2->lpb_voltage);
    EXPECT_EQ(avdw1->N2_pressure, avdw2->N2_pressure);
    EXPECT_EQ(avdw1->N2_temp, avdw2->N2_temp);
    EXPECT_EQ(avdw1->packet_nbr, avdw2->packet_nbr);
}
void equal_gsdw(PacketGSE_downlink* gsdw1, PacketGSE_downlink* gsdw2) {
    EXPECT_EQ(gsdw1->disconnectActive, gsdw2->disconnectActive);
    EXPECT_EQ(gsdw1->fillingPressure, gsdw2->fillingPressure);
    EXPECT_EQ(gsdw1->loadcell_raw, gsdw2->loadcell_raw);
    EXPECT_EQ(gsdw1->tankPressure, gsdw2->tankPressure);
    EXPECT_EQ(gsdw1->tankTemperature, gsdw2->tankTemperature);
    EXPECT_EQ(gsdw1->status.fillingN2O, gsdw2->status.fillingN2O);
    EXPECT_EQ(gsdw1->status.vent, gsdw2->status.vent);
}

TEST(readWriteTest, singleAvup) {
    SqliteDB* db = get_db();
    av_uplink_t* avup = get_avup();
    db->write_pkt(db->process_pkt(avup,NULL,NULL));
    delete db;
    SqliteDB* newdb = get_db();
    av_uplink_t* avupRead = new av_uplink_t;
    Packet packet = newdb->read_pkt(AV_UPLINK, 0);
    newdb->unprocess_pkt(packet, avupRead, NULL, NULL);
    equal_avup(avup, avupRead);
}

TEST(exampleTest, failureTest) {
    EXPECT_EQ(1, 2);
}


/*
int main(int argc, char *argv[]) {
    printf("starting data-storage tests\n");

    printf("creating db\n");
    SqliteDB* db = new SqliteDB;

    printf("creating packets\n");
    av_uplink_t* avup1 = new av_uplink_t;
    *avup1 = {.order_id=1, .order_value=2};
    av_uplink_t* avup2 = new av_uplink_t;
    *avup2 = {.order_id=3, .order_value=4};
    av_downlink_t* avdw1 = new av_downlink_t;
    *avdw1 = {.packet_nbr=9, .gnss_lon=10,
        .gnss_lat=11, .gnss_alt=12, .gnss_vertical_speed=13,
        .N2_pressure=14, .fuel_pressure=15, .LOX_pressure=16,
        .fuel_level=17, .LOX_level=18, .N2_temp=19, .LOX_temp=20,
        .LOX_inj_temp=21, .lpb_voltage=22, .hpb_voltage=23,
        .av_fc_temp=24, .ambient_temp=25, .engine_state=26,
        .av_state=27, .cam_rec=28};
    av_downlink_t* avdw2 = new av_downlink_t;
    *avdw2 = {.packet_nbr=10, .gnss_lon=11,
        .gnss_lat=12, .gnss_alt=13, .gnss_vertical_speed=14,
        .N2_pressure=15, .fuel_pressure=16, .LOX_pressure=17,
        .fuel_level=18, .LOX_level=19, .N2_temp=20, .LOX_temp=21,
        .LOX_inj_temp=22, .lpb_voltage=23, .hpb_voltage=24,
        .av_fc_temp=25, .ambient_temp=26, .engine_state=27,
        .av_state=28, .cam_rec=29};
    GSE_cmd_status* status1 = new GSE_cmd_status;
    *status1 = {.fillingN2O=12, .vent=13};
    GSE_cmd_status* status2 = new GSE_cmd_status;
    *status2 = {.fillingN2O=13, .vent=14};
    PacketGSE_downlink* gsdw1 = new PacketGSE_downlink;
    *gsdw1 = {.tankPressure=11.123456789, .tankTemperature=11.2,
        .fillingPressure=11.3, *status1,
        .disconnectActive=true, .loadcell_raw=14};
    PacketGSE_downlink* gsdw2 = new PacketGSE_downlink;
    *gsdw2 = {.tankPressure=12.1, .tankTemperature=12.2,
        .fillingPressure=12.3, *status2,
        .disconnectActive=false, .loadcell_raw=15};
    
    printf("processing and writing packets\n");
    db->write_pkt(db->process_pkt(avup1, NULL, NULL));
    db->write_pkt(db->process_pkt(avup2, NULL, NULL));
    db->write_pkt(db->process_pkt(NULL, avdw1, NULL));
    db->write_pkt(db->process_pkt(NULL, avdw2, NULL));
    db->write_pkt(db->process_pkt(NULL, NULL, gsdw1));
    db->write_pkt(db->process_pkt(NULL, NULL, gsdw2));

    printf("reading packets by id\n");
    Packet read_avup2 = db->read_pkt(AV_UPLINK, 1);
    Packet read_gsdw1 = db->read_pkt(GSE_DOWNLINK, 0);
    Packet read_gsdw2 = db->read_pkt(GSE_DOWNLINK, 1);
    Packet read_avdw1 = db->read_pkt(AV_DOWNLINK, 0);
    Packet read_avup1 = db->read_pkt(AV_UPLINK, 0);
    Packet read_avdw2 = db->read_pkt(AV_DOWNLINK, 1);

    printf("flush and free db\n");
    delete db;

    printf("end of data-storage tests\n");
    return 0;
}
*/