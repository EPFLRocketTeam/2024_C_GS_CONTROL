
#include <string>
#include "../include/data_storage.h"



int main(int argc, char *argv[]) {
    printf("starting data-storage tests\n");

    //create db
    SqliteDB* db = new SqliteDB;
    printf("db created\n");
    //create av up pkt
    AV_uplink_pkt* avup = new AV_uplink_pkt;
    *avup = {.id=1, .ts=4, .order_id=2, .order_value=3};
    Packet* pkt = new Packet;
    *pkt = {AV_UPLINK, avup, NULL, NULL};

    //write pkt
    printf("write 1st pkt\n");
    if (db->write_pkt(*pkt) != 0) {printf("write return != 0\n");}
    printf("write 2nd pkt\n");
    if (db->write_pkt(*pkt) != 1) {printf("write retrn != 1\n");}

    printf("end of data-storage tests\n");
    return 0;
}
