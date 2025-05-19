#include "../include/data_storage.h"
#include <gtest/gtest.h>


TEST(exampleTest, failureTest) {
    EXPECT_EQ(1, 2);
}



int main(int argc, char *argv[]) {
    printf("starting data-storage tests\n");

    printf("creating db\n");
    SqliteDB* db = new SqliteDB;

    printf("creating test packets\n");
    //create first av up pkt
    AV_uplink_pkt* avup1 = new AV_uplink_pkt;
    *avup1 = {.id=1, .ts=2, .order_id=3, .order_value=4};
    Packet* pkt1 = new Packet;
    *pkt1 = {AV_UPLINK, avup1, NULL, NULL};

    //create second av up pkt
    AV_uplink_pkt* avup2 = new AV_uplink_pkt;
    *avup2 = {.id=2, .ts=5, .order_id=6, .order_value=7};
    Packet* pkt2 = new Packet;
    *pkt2 = {AV_UPLINK, avup2, NULL, NULL};

    //create first av dw pkt
    AV_downlink_pkt* avdw1 = new AV_downlink_pkt;
    *avdw1 = {.id=3, .ts=8, .packet_nbr=9, .gnss_lon=10,
        .gnss_lat=11, .gnss_alt=12, .gnss_vertical_speed=13,
        .N2_pressure=14, .fuel_pressure=15, .LOX_pressure=16,
        .fuel_level=17, .LOX_level=18, .N2_temp=19, .LOX_temp=20,
        .LOX_inj_temp=21, .lpb_voltage=22, .hpb_voltage=23,
        .av_fc_temp=24, .ambient_temp=25, .engine_state=26,
        .av_state=27, .cam_rec=28};
    Packet* pkt3 = new Packet;
    *pkt3 = {AV_DOWNLINK, NULL, avdw1, NULL};

    //create second av dw pkt
    AV_downlink_pkt* avdw2 = new AV_downlink_pkt;
    *avdw2 = {.id=4, .ts=5, .packet_nbr=10, .gnss_lon=11,
        .gnss_lat=12, .gnss_alt=13, .gnss_vertical_speed=14,
        .N2_pressure=15, .fuel_pressure=16, .LOX_pressure=17,
        .fuel_level=18, .LOX_level=19, .N2_temp=20, .LOX_temp=21,
        .LOX_inj_temp=22, .lpb_voltage=23, .hpb_voltage=24,
        .av_fc_temp=25, .ambient_temp=26, .engine_state=27,
        .av_state=28, .cam_rec=29};
    Packet* pkt4 = new Packet;
    *pkt4 = {AV_DOWNLINK, NULL, avdw2, NULL};

    //create first gse dw pkt
    GSE_downlink_pkt* gsdw1 = new GSE_downlink_pkt;
    *gsdw1 = {.id=5, .ts=5, .tankPressure=11.1, .tankTemperature=11.2,
        .fillingPressure=11.3, .fillingN2O=12, .vent=13,
        .disconnectActive=true, .loadcell_raw=14};
    Packet* pkt5 = new Packet;
    *pkt5 = {GSE_DOWNLINK, NULL, NULL, gsdw1};

    //create second gse dw pkt
    GSE_downlink_pkt* gsdw2 = new GSE_downlink_pkt;
    *gsdw2 = {.id=6, .ts=5, .tankPressure=12.1, .tankTemperature=12.2,
        .fillingPressure=12.3, .fillingN2O=13, .vent=14,
        .disconnectActive=false, .loadcell_raw=15};
    Packet* pkt6 = new Packet;
    *pkt6 = {GSE_DOWNLINK, NULL, NULL, gsdw2};

    //write pkt
    printf("writing packets\n");
    if (db->write_pkt(*pkt1) != 0) {printf("write return != 0 (pkt1)\n");}
    if (db->write_pkt(*pkt2) != 1) {printf("write retrn != 1 (pkt2)\n");}
    if (db->write_pkt(*pkt3) != 0) {printf("write return != 0 (pkt3)\n");}
    if (db->write_pkt(*pkt4) != 1) {printf("write retrn != 1 (pkt4)\n");}
    if (db->write_pkt(*pkt5) != 0) {printf("write return != 0 (pkt5)\n");}
    if (db->write_pkt(*pkt6) != 1) {printf("write retrn != 1 (pkt6)\n");}

    printf("end of data-storage tests\n");
    return 0;
}
