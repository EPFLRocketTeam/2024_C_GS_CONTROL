#include "../include/data_storage.h"
#include <string>

SqliteDB::SqliteDB(std::string path_to_db) {}

SqliteDB::~SqliteDB() {}

int write_pkt(Packet pkt) {}

int read_pkt(uint32_t pkt_id, Packet pkt) {}

int flush() {}

int delete_database(std::string path_to_db) {}
