#include "packet_helper.h"
#include <sstream>

std::string to_string(const av_downlink_t &packet) {
    std::ostringstream oss;
    oss << "Packet Number: " << packet.packet_nbr << "\n"
        << "Timestamp: " << packet.timestamp << "\n"
        << "GNSS Lon: " << packet.gnss_lon << "\n"
        << "GNSS Lat: " << packet.gnss_lat << "\n"
        << "GNSS Alt: " << packet.gnss_alt << "\n"
        << "GNSS Lon R: " << packet.gnss_lon_r << "\n"
        << "GNSS Lat R: " << packet.gnss_lat_r << "\n"
        << "GNSS Alt R: " << packet.gnss_alt_r << "\n"
        << "GNSS Vertical Speed: " << packet.gnss_vertical_speed << "\n"
        << "N2 Pressure: " << packet.N2_pressure << "\n"
        << "Fuel Pressure: " << packet.fuel_pressure << "\n"
        << "LOX Pressure: " << packet.LOX_pressure << "\n"
        << "Fuel Level: " << packet.fuel_level << "\n"
        << "LOX Level: " << packet.LOX_level << "\n"
        << "Engine Temp: " << packet.engine_temp << "\n"
        << "Igniter Pressure: " << packet.igniter_pressure << "\n"
        << "LOX Inj Pressure: " << packet.LOX_inj_pressure << "\n"
        << "Fuel Inj Pressure: " << packet.fuel_inj_pressure << "\n"
        << "Chamber Pressure: " << packet.chamber_pressure << "\n"
        << "AV State: " << static_cast<int>(packet.av_state) << "\n"
        << "GNSS Choice: " << static_cast<int>(packet.gnss_choice);
    return oss.str();
}
