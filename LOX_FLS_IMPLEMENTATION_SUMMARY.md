# LOX Float Level Sensor Implementation Summary

## Overview
Added 5 new LOX float level sensor (FLS) temperature measurements to the Firehorn GUI display system.

## New Sensor Fields
1. **LOX_CAP_FLS_0** - Capacitive float level sensor at 0% (bottom)
2. **LOX_FLS_10** - Float level sensor at 10%
3. **LOX_FLS_50** - Float level sensor at 50%
4. **LOX_FLS_80** - Float level sensor at 80%
5. **LOX_FLS_90** - Float level sensor at 90% (top)

## Files Modified

### 1. **commons/include/FieldUtil.h**
   - Added 5 new enum values to `GUI_FIELD` enum
   - Added name mappings in `enumToFieldName()` function
   - Added reverse mappings in `fieldNameToEnum()` function

### 2. **commons/ERT_RF_Protocol_Interface/PacketDefinition_Firehorn.h**
   - Added 5 new int8_t fields to `av_downlink_t` packed structure (bitfields)
   - Added 5 new float fields to `av_downlink_unpacked_t` structure
   - Fixed missing `LOX_inj_temp` field in packed structure

### 3. **commons/ERT_RF_Protocol_Interface/DownlinkCompression_Firehorn.h**
   - Added compression logic for 5 new sensors in `encode_downlink()` (divide by 2)
   - Added decompression logic for 5 new sensors in `decode_downlink()` (multiply by 2)
   - Fixed missing `LOX_inj_temp` compression/decompression

### 4. **Server/include/data_storage.h**
   - Added 5 new float fields to `AV_downlink_pkt` struct

### 5. **Server/src/data_storage.cpp**
   - Added 5 new database columns to SQLite schema using `sqlite_orm::make_column()`
   - Added field mapping in packet processing (`process_pkt()`)
   - Added field mapping in packet unprocessing (`unprocess_pkt()`)

### 6. **Server/src/RequestAdapter.cpp**
   - Added 5 new fields to JSON object creation for GUI data
   - Updated debug output string with new fields
   - Updated argument chain for debug logging

### 7. **Server/src/server.cpp**
   - Added test data generation for 5 new sensors using `distTemp(gen)`

### 8. **GUI/src/Setup.h**
   - **Replaced** `LOX_INJ_TEMP` at position (0.83, 0.665) with `LOX_FLS_90`
   - Added `LOX_FLS_80` at (0.83, 0.715)
   - Added `LOX_FLS_50` at (0.83, 0.765)
   - Added `LOX_FLS_10` at (0.83, 0.815)
   - Added `LOX_CAP_FLS_0` at (0.83, 0.865) - bottom sensor
   - Sensors are stacked vertically with 0.05 spacing

## GUI Display Layout
The sensors are displayed in the FIREHORN_GUI section, positioned on the right side of the LOX tank:
- **Top (90%)**: x=0.83, y=0.665
- **80%**: x=0.83, y=0.715
- **50%**: x=0.83, y=0.765
- **10%**: x=0.83, y=0.815
- **Bottom (0%)**: x=0.83, y=0.865

All sensors display temperature values in °C.

## Data Flow
1. **RF Protocol** → `av_downlink_t` (compressed, bitfields)
2. **Decompression** → `av_downlink_unpacked_t` (floats)
3. **Database** → `AV_downlink_pkt` (floats)
4. **RequestAdapter** → JSON with GUI_FIELD enum keys
5. **GUI Display** → LabelInfo at specified coordinates

## Testing
- Test data generation is implemented in `server.cpp`
- All sensors use the same temperature distribution for testing
- Database schema includes all new columns for data persistence

## Notes
- The original `LOX_INJ_TEMP` field was replaced with `LOX_FLS_90` as requested
- All sensors follow the same data type and unit convention (temperature in °C)
- The implementation follows the existing pattern for other temperature sensors
- GUI coordinates may need fine-tuning based on the actual SVG background layout
