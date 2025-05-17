
#include <string>
#include "../include/data_storage.h"



int main(int argc, char *argv[]) {
    printf("starting data-storage tests\n");

    //create db
    SqliteDB* db = new SqliteDB;
    printf("db created\n");
    
    //create first av up pkt
    AV_uplink_pkt* avup1 = new AV_uplink_pkt;
    *avup1 = {.id=1, .ts=4, .order_id=2, .order_value=3};
    Packet* pkt1 = new Packet;
    *pkt1 = {AV_UPLINK, avup1, NULL, NULL};

    //create second av up pkt
    AV_uplink_pkt* avup2 = new AV_uplink_pkt;
    *avup2 = {.id=2, .ts=4, .order_id=2, .order_value=3};
    Packet* pkt2 = new Packet;
    *pkt2 = {AV_UPLINK, avup2, NULL, NULL};

    //write pkt
    printf("write 1st pkt\n");
    if (db->write_pkt(*pkt1) != 0) {printf("write return != 0\n");}
    printf("write 2nd pkt\n");
    if (db->write_pkt(*pkt2) != 1) {printf("write retrn != 1\n");}

    printf("end of data-storage tests\n");
    return 0;
}
