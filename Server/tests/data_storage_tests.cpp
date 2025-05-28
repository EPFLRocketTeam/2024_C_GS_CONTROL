#include "../include/data_storage.h"
#include <gtest/gtest.h>


TEST(exampleTest, failureTest) {
    EXPECT_EQ(1, 2);
}



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

    printf("end of data-storage tests\n");
    return 0;
}
