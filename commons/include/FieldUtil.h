#ifndef FIELD_UTIL_H
#define FIELD_UTIL_H
#include <../ERT_RF_Protocol_Interface/Protocol.h>
#include <QString>

enum GUI_FIELD {
  IGNITER_LOX = 42,
  IGNITER_FUEL,
  PRESSURE_VALVE_LOX,
  PRESSURE_VALVE_FUEL,
  MAIN_LOX,
  MAIN_FUEL,
  VENT_LOX,
  VENT_FUEL,
  VENT_N2,
  ORDER_ID,
  ORDER_VALUE,
  PACKET_NBR,
  TIMESTAMP,
  GNSS_LON,
  GNSS_LAT,
  GNSS_ALT,
  GNSS_LON_R,
  GNSS_LAT_R,
  GNSS_ALT_R,
  GNSS_VERTICAL_SPEED,
  N2_PRESSURE,
  FUEL_PRESSURE,
  LOX_PRESSURE,
  FUEL_LEVEL,
  LOX_LEVEL,
  LOX_TEMP,
  LOX_INJ_TEMP,
  N2_TEMP,
  IGNITER_PRESSURE,
  LOX_INJ_PRESSURE,
  FUEL_INJ_PRESSURE,
  CHAMBER_PRESSURE,
  LPB_VOLTAGE,
  HPB_VOLTAGE,
  LPB_CURRENT,
  HPB_CURRENT,
  AV_FC_TEMP,
  AMBIENT_TEMP,
  CAM_REC,
  ENGINE_STATE,
  AV_STATE,
  GNSS_CHOICE,
  GSE_FILLING_N2O,
  VENT_GSE,
  GSE_GQN_NC1,
  GSE_GQN_NC2,
  GSE_GQN_NC3,
  GSE_GQN_NC4,
  GSE_GQN_NC5,
  GSE_GPN_NC1,
  GSE_GPN_NC2,
  GSE_GVN_NC,
  GSE_GFE_NC,
  GSE_GFO_NCC,
  GSE_GDO_NCC,
  GSE_PC_OLC,
  GSE_GP1,
  GSE_GP2,
  GSE_GP3,
  GSE_GP4,
  GSE_GP5,
  GSE_TANK_PRESSURE,
  GSE_TANK_TEMPERATURE,
  GSE_FILLING_PRESSURE,
  GSE_DISCONNECT_ACTIVE,
  GSE_LOADCELL_1,
  GSE_LOADCELL_2,
  GSE_LOADCELL_3,
  GSE_LOADCELL_4,
  GSE_VENT,
  GSE_CMD_STATUS,
  DOWNRANGE,
  SERIAL_STATUS,
  SERIAL_NAME_USE,
  AV_TIMER,
  GSE_TIMER,
  CMD_1,
  CMD_2,
  CMD_3,
  GUI_CMD_LAUNCH,
  GUI_CMD_SET_SERIAL_STATUS,
  GUI_CMD_CALIBRATE,
  GUI_CMD_GSE_IDLE,
  GUI_CMD_GSE_CALIBRATE,
  GUI_CMD_RECOVER,
  GUI_CMD_ARM,
  GUI_CMD_GSE_ARM,
  GUI_CMD_GSE_PASSIVATE,
  GUI_CMD_HOPPER_TARE,
  GUI_CMD_HOPPER_IDLE,
  GUI_CMD_HOPPER_TARE_ORIENTATION,
  GUI_CMD_HOPPER_HOMING_GIMBAL,
  GUI_CMD_HOPPER_HOMING_MAIN_VALVES,
  GUI_CMD_IGNITION,
  GUI_CMD_ABORT,
  GUI_CMD_MANUAL_DEPLOY,
  GUI_CMD_IGNITER_LOX,
  GUI_CMD_IGNITER_FUEL,
  GUI_CMD_MAIN_LOX,
  GUI_CMD_MAIN_FUEL,
  GUI_CMD_MAIN_N2O,
  GUI_CMD_VENT_LOX,
  GUI_CMD_VENT_FUEL,
  GUI_CMD_VENT_N2,
  GUI_CMD_PRESSURIZE,
  GUI_CMD_FILLING_N2O,
  GUI_CMD_FILLING_LOX,
  GUI_CMD_VENT,
  GUI_CMD_VENT_N2O,
  GUI_CMD_GIMBALL_X,
  GUI_CMD_GIMBALL_Y,
  GUI_CMD_SERVO_1,
  GUI_CMD_SERVO_2,
  GUI_CMD_P_LOX,
  GUI_CMD_P_FUEL,
  GUI_CMD_GSE_TOGGLE_11,
  GUI_CMD_GSE_TOGGLE_12,
  GUI_CMD_GSE_TOGGLE_13,
  GUI_CMD_GSE_TOGGLE_14,
  GUI_CMD_GSE_TOGGLE_15,
  GUI_CMD_GSE_TOGGLE_16,
  GUI_CMD_GSE_TOGGLE_21,
  GUI_CMD_GSE_TOGGLE_22,
  GUI_CMD_GSE_TOGGLE_23,
  GUI_CMD_GSE_TOGGLE_24,
  GUI_CMD_GSE_TOGGLE_25,
  GUI_CMD_GSE_TOGGLE_26,
  GUI_CMD_GSE_TOGGLE_27,
  GUI_CMD_GSE_TOGGLE_28,
  GUI_CMD_DISCONNECT,
  HOPPER_PACKET_NBR,   // 32 bits: packet counter (10Hz update)
  HOPPER_N2O_PRESSURE, // 16 bits (12-bit ADC value)
  HOPPER_ETH_PRESSURE, // 16 bits
  HOPPER_N2O_TEMP,     // 8 bits
  HOPPER_N2O_VENT,     // 1 bit
  HOPPER_ETH_VENT,     // 1 bit
  HOPPER_N2_SOL,       // 1 bit
  HOPPER_N2O_MAIN,     // 8 bits
  HOPPER_ETH_MAIN,     // 8 bits
  HOPPER_N2O_SOL,      // 8 bits
  HOPPER_ETH_SOL,      // 8 bits
  HOPPER_IGNITER,      
  HOPPER_CHAMBER_PRESSURE,      
  HOPPER_GNSS_LON,     // 32 bits (float)
  HOPPER_GNSS_LAT,     // 32 bits (float)
  HOPPER_SAT_NBR,      // 8 bits (number of fixed satellites)
  HOPPER_GYRO_X,       // 16 bits (raw gyro)
  HOPPER_GYRO_Y,       // 16 bits
  HOPPER_GYRO_Z,       // 16 bits
  HOPPER_ACC_X,        // 16 bits (raw accelerometer)
  HOPPER_ACC_Y,        // 16 bits
  HOPPER_ACC_Z,        // 16 bits
  HOPPER_BARO,         // 16 bits (barometric reading)
  HOPPER_KALMAN_POS_X, // 16 bits (Kalman filter position estimate)
  HOPPER_KALMAN_POS_Y, // 16 bits
  HOPPER_KALMAN_POS_Z, // 16 bits
  HOPPER_KALMAN_YAW,   // 16 bits (Kalman filter angle)
  HOPPER_KALMAN_PITCH, // 16 bits
  HOPPER_KALMAN_ROLL,  // 16 bits
  HOPPER_GIMBAL_X,     // 8 bits (gimbal position)
  HOPPER_GIMBAL_Y,     // 8 bits
  HOPPER_HV_VOLTAGE,   // 8 bits (high-voltage measurement)
  HOPPER_LV_VOLTAGE,   // 8 bits (low-voltage measurement)
  HOPPER_AV_TEMP,      // 8 bits (AV temperature)
  HOPPER_ID_CONFIG,    // 8 bits (configuration ID: PID, flight mode, etc.)
  HOPPER_AV_STATE,     // 8 bits (AV state)
  HOPPER_FIREUP_STATE,
  UNKNOWN
};

namespace fieldUtil {
inline QString enumToFieldName(GUI_FIELD field) {
  QString name;
  switch (field) {
  case PRESSURE_VALVE_FUEL:
    name = "PRESSURE VALVE FUEL";
    break;
  case PRESSURE_VALVE_LOX:
    name = "PRESSURE VALVE LOX";
    break;
  case MAIN_LOX:
    name = "MAIN LOX";
    break;
  case MAIN_FUEL:
    name = "MAIN FUEL";
    break;
  case VENT_LOX:
    name = "VENT LOX";
    break;
  case VENT_FUEL:
    name = "VENT FUEL";
    break;
  case VENT_N2:
    name = "VENT N2";
    break;
  case ORDER_ID:
    name = "ORDER ID";
    break;
  case ORDER_VALUE:
    name = "ORDER VALUE";
    break;
  case PACKET_NBR:
    name = "PACKET NBR";
    break;
  case TIMESTAMP:
    name = "TIMESTAMP";
    break;
  case GNSS_LON:
    name = "GNSS LON";
    break;
  case GNSS_LAT:
    name = "GNSS LAT";
    break;
  case GNSS_ALT:
    name = "GNSS ALT";
    break;
  case GNSS_LON_R:
    name = "GNSS LON R";
    break;
  case GNSS_LAT_R:
    name = "GNSS LAT R";
    break;
  case GNSS_ALT_R:
    name = "GNSS ALT R";
    break;
  case GNSS_VERTICAL_SPEED:
    name = "GNSS VERTICAL SPEED";
    break;
  case N2_PRESSURE:
    name = "N2 PRESSURE";
    break;
  case FUEL_PRESSURE:
    name = "FUEL PRESSURE";
    break;
  case LOX_PRESSURE:
    name = "LOX PRESSURE";
    break;
  case FUEL_LEVEL:
    name = "FUEL LEVEL";
    break;
  case LOX_LEVEL:
    name = "LOX LEVEL";
    break;
  case LOX_TEMP:
    name = "LOX TEMPERATURE";
    break;
  case N2_TEMP:
    name = "N2 TEMPERATURE";
    break;
  case LOX_INJ_TEMP:
    name = "LOX INJ TEMPERATURE";
    break;
  case IGNITER_PRESSURE:
    name = "IGNITER PRESSURE";
    break;
  case LOX_INJ_PRESSURE:
    name = "LOX INJ PRESSURE";
    break;
  case FUEL_INJ_PRESSURE:
    name = "FUEL INJ PRESSURE";
    break;
  case CHAMBER_PRESSURE:
    name = "CHAMBER PRESSURE";
    break;
  case LPB_VOLTAGE:
    name = "LPB VOLTAGE";
    break;
  case HPB_VOLTAGE:
    name = "HPB VOLTAGE";
    break;
  case LPB_CURRENT:
    name = "LPB CURRENT";
    break;
  case HPB_CURRENT:
    name = "HPB CURRENT";
    break;
  case AV_FC_TEMP:
    name = "AV FC TEMPERATURE";
    break;
  case AMBIENT_TEMP:
    name = "AMBIENT TEMPERATURE";
    break;
  case AV_STATE:
    name = "AV STATE";
    break;
  case CAM_REC:
    name = "CAM REC";
    break;
  case GNSS_CHOICE:
    name = "GNSS CHOICE";
    break;
  case GSE_FILLING_N2O:
    name = "GSE FILLING N2O";
    break;
  case VENT_GSE:
    name = "VENT GSE";
    break;
  case GSE_TANK_PRESSURE:
    name = "GSE TANK PRESSURE";
    break;
  case GSE_TANK_TEMPERATURE:
    name = "GSE TANK TEMPERATURE";
    break;
  case GSE_FILLING_PRESSURE:
    name = "GSE FILLING PRESSURE";
    break;
  case GSE_DISCONNECT_ACTIVE:
    name = "GSE DISCONNECT ACTIVE";
    break;
  case GSE_LOADCELL_1:
    name = "GSE LOADCELL 1";
    break;
  case GSE_LOADCELL_2:
    name = "GSE LOADCELL 2";
    break;
  case GSE_LOADCELL_3:
    name = "GSE LOADCELL 3";
    break;
  case GSE_LOADCELL_4:
    name = "GSE LOADCELL 4";
    break;
  case ENGINE_STATE:
    name = "ENGINE STATE";
    break;
  case GSE_VENT:
    name = "GSE VENT";
    break;
  case GSE_CMD_STATUS:
    name = "GSE CMD STATUS";
    break;
  case HOPPER_PACKET_NBR:
    name = "HOPPER PACKET NBR";
    break;
  case HOPPER_N2O_PRESSURE:
    name = "HOPPER N2O PRESSURE";
    break;
  case HOPPER_IGNITER:
    name = "HOPPER IGNITER";
    break;
  case HOPPER_ETH_PRESSURE:
    name = "HOPPER ETH PRESSURE";
    break;
  case HOPPER_N2O_TEMP:
    name = "HOPPER N2O TEMP";
    break;
  case HOPPER_N2O_VENT:
    name = "HOPPER N2O VENT";
    break;
  case HOPPER_ETH_VENT:
    name = "HOPPER ETH VENT";
    break;
  case HOPPER_N2_SOL:
    name = "HOPPER N2 SOLENOID";
    break;
  case HOPPER_N2O_SOL:
    name = "HOPPER N2O SOL";
    break;
  case HOPPER_ETH_SOL:
    name = "HOPPER ETH SOL";
    break;
  case HOPPER_N2O_MAIN:
    name = "HOPPER N2O MAIN";
    break;
  case HOPPER_ETH_MAIN:
    name = "HOPPER ETH MAIN";
    break;
  case HOPPER_GNSS_LON:
    name = "HOPPER GNSS LON";
    break;
  case HOPPER_GNSS_LAT:
    name = "HOPPER GNSS LAT";
    break;
  case HOPPER_SAT_NBR:
    name = "HOPPER SAT NBR";
    break;
  case HOPPER_GYRO_X:
    name = "HOPPER GYRO X";
    break;
  case HOPPER_GYRO_Y:
    name = "HOPPER GYRO Y";
    break;
  case HOPPER_GYRO_Z:
    name = "HOPPER GYRO Z";
    break;
  case HOPPER_ACC_X:
    name = "HOPPER ACC X";
    break;
  case HOPPER_ACC_Y:
    name = "HOPPER ACC Y";
    break;
  case HOPPER_ACC_Z:
    name = "HOPPER ACC Z";
    break;
  case HOPPER_BARO:
    name = "HOPPER BARO";
    break;
  case HOPPER_KALMAN_POS_X:
    name = "HOPPER KALMAN POS X";
    break;
  case HOPPER_KALMAN_POS_Y:
    name = "HOPPER KALMAN POS Y";
    break;
  case HOPPER_KALMAN_POS_Z:
    name = "HOPPER KALMAN POS Z";
    break;
  case HOPPER_KALMAN_YAW:
    name = "HOPPER KALMAN YAW";
    break;
  case HOPPER_KALMAN_PITCH:
    name = "HOPPER KALMAN PITCH";
    break;
  case HOPPER_KALMAN_ROLL:
    name = "HOPPER KALMAN ROLL";
    break;
  case HOPPER_GIMBAL_X:
    name = "HOPPER GIMBAL X";
    break;
  case HOPPER_GIMBAL_Y:
    name = "HOPPER GIMBAL Y";
    break;
  case HOPPER_HV_VOLTAGE:
    name = "HOPPER HV VOLTAGE";
    break;
  case HOPPER_LV_VOLTAGE:
    name = "HOPPER LV VOLTAGE";
    break;
  case HOPPER_AV_TEMP:
    name = "HOPPER AV TEMP";
    break;
  case HOPPER_ID_CONFIG:
    name = "HOPPER ID CONFIG";
    break;
  case HOPPER_AV_STATE:
    name = "HOPPER AV STATE";
    break;
  case HOPPER_FIREUP_STATE:
    name = "HOPPER FIRE UP STATE";
    break;
  case GUI_CMD_CALIBRATE:
    name = "CALIBRATE";
    break;
  case GUI_CMD_GIMBALL_X:
    name = "CMD GIMBAL X";
    break;
  case GUI_CMD_GIMBALL_Y:
    name = "CMD GIMBAL Y";
    break;
  case GUI_CMD_RECOVER:
    name = "RECOVER";
    break;
  case GUI_CMD_GSE_IDLE:
    name = "GSE IDLE";
    break;
  case GUI_CMD_GSE_CALIBRATE:
    name = "GSE CALIBRATE";
    break;
  case GUI_CMD_GSE_ARM:
    name = "GSE ARM";
    break;

  case GUI_CMD_GSE_PASSIVATE:
    name = "GSE PASSIVATE";
    break;
  case GUI_CMD_HOPPER_TARE:
    name = "HOPPER TARE PRESSURE";
    break;
  case GUI_CMD_HOPPER_IDLE:
    name = "HOPPER IDLE";
    break;
  case GUI_CMD_HOPPER_TARE_ORIENTATION:
    name = "HOPPER TARE ORIENTATION";
    break;
  case GUI_CMD_HOPPER_HOMING_GIMBAL:
    name = "HOMING GIMBAL";
    break;
  case GUI_CMD_HOPPER_HOMING_MAIN_VALVES:
    name = "HOMING VALVES";
    break;

  case GUI_CMD_SERVO_1:
    name = "SERVO 1";
    break;

  case GUI_CMD_SERVO_2:
    name = "SERVO 2";
    break;

  case GUI_CMD_GSE_TOGGLE_11:
    name = "GSE TOGGLE 11";
    break;

  case GUI_CMD_GSE_TOGGLE_12:
    name = "GSE TOGGLE 12";
    break;

  case GUI_CMD_GSE_TOGGLE_13:
    name = "GSE TOGGLE 13";
    break;

  case GUI_CMD_GSE_TOGGLE_14:
    name = "GSE TOGGLE 14";
    break;

  case GUI_CMD_GSE_TOGGLE_15:
    name = "GSE TOGGLE 15";
    break;

  case GUI_CMD_GSE_TOGGLE_16:
    name = "GSE TOGGLE 16";
    break;

  case GUI_CMD_GSE_TOGGLE_21:
    name = "GSE TOGGLE 21";
    break;

  case GUI_CMD_GSE_TOGGLE_22:
    name = "GSE TOGGLE 22";
    break;

  case GUI_CMD_GSE_TOGGLE_23:
    name = "GSE TOGGLE 23";
    break;

  case GUI_CMD_GSE_TOGGLE_24:
    name = "GSE TOGGLE 24";
    break;

  case GUI_CMD_GSE_TOGGLE_25:
    name = "GSE TOGGLE 25";
    break;

  case GUI_CMD_GSE_TOGGLE_26:
    name = "GSE TOGGLE 26";
    break;

  case GUI_CMD_GSE_TOGGLE_27:
    name = "GSE TOGGLE 27";
    break;

  case GUI_CMD_GSE_TOGGLE_28:
    name = "GSE TOGGLE 28";
    break;

  case GUI_CMD_IGNITION:
    name = "IGNITION";
    break;
  case GUI_CMD_MANUAL_DEPLOY:
    name = "MANUAL DEPLOY";
    break;
  case GUI_CMD_PRESSURIZE:
    name = "PRESSURIZE";
    break;
  case GUI_CMD_ARM:
    name = "ARM";
    break;
  case GUI_CMD_LAUNCH:
    name = "LAUNCH";
    break;
  case GUI_CMD_ABORT:
    name = "ABORT";
    break;
  case GUI_CMD_DISCONNECT:
    name = "DISCONNECT";
    break;
  case GUI_CMD_FILLING_LOX:
    name = "FILLING LOX";
    break;
  case GUI_CMD_FILLING_N2O:
    name = "FILLING N2O";
    break;

  case GSE_GQN_NC1:
    name = "GQN NC1";
    break;

  case GSE_GQN_NC2:
    name = "GQN NC2";
    break;

  case GSE_GQN_NC3:
    name = "GQN NC3";
    break;
  case GSE_GQN_NC4:
    name = "GQN NC4";
    break;
  case GSE_GQN_NC5:
    name = "GQN NC5";
    break;

  case GSE_GPN_NC1:
    name = "GPN NC1";
    break;

  case GSE_GPN_NC2:
    name = "GPN NC2";
    break;

  case GSE_GVN_NC:
    name = "GVN NC";
    break;

  case GSE_GFE_NC:
    name = "GFE NC";
    break;

  case GSE_GFO_NCC:
    name = "GFO NCC";
    break;

  case GSE_GDO_NCC:
    name = "GDO NCC";
    break;

  case GSE_PC_OLC:
    name = "PC OLC";
    break;

  case GSE_GP1:
    name = "GP1";
    break;

  case GSE_GP2:
    name = "GP2";
    break;

  case GSE_GP3:
    name = "GP3";
    break;

  case GSE_GP4:
    name = "GP4";
    break;

  case GSE_GP5:
    name = "GP5";
    break;

  default:
    name = "UNKNOWN";
    break;
  }
  return name;
}

inline GUI_FIELD fieldNameToEnum(const QString &fieldName) {
  if (fieldName == "PRESSURE VALVE LOX")
    return PRESSURE_VALVE_LOX;
  else if (fieldName == "PRESSURE VALVE FUEL")
    return PRESSURE_VALVE_FUEL;
  else if (fieldName == "MAIN LOX")
    return MAIN_LOX;
  else if (fieldName == "MAIN FUEL")
    return MAIN_FUEL;
  else if (fieldName == "VENT LOX")
    return VENT_LOX;
  else if (fieldName == "VENT N2")
    return VENT_N2;
  else if (fieldName == "VENT FUEL")
    return VENT_FUEL;
  else if (fieldName == "ORDER ID")
    return ORDER_ID;
  else if (fieldName == "ORDER VALUE")
    return ORDER_VALUE;
  else if (fieldName == "PACKET NBR")
    return PACKET_NBR;
  else if (fieldName == "TIMESTAMP")
    return TIMESTAMP;
  else if (fieldName == "GNSS LON")
    return GNSS_LON;
  else if (fieldName == "GNSS LAT")
    return GNSS_LAT;
  else if (fieldName == "GNSS ALT")
    return GNSS_ALT;
  else if (fieldName == "GNSS LON R")
    return GNSS_LON_R;
  else if (fieldName == "GNSS LAT R")
    return GNSS_LAT_R;
  else if (fieldName == "GNSS ALT R")
    return GNSS_ALT_R;
  else if (fieldName == "GNSS VERTICAL SPEED")
    return GNSS_VERTICAL_SPEED;
  else if (fieldName == "N2 PRESSURE")
    return N2_PRESSURE;
  else if (fieldName == "FUEL PRESSURE")
    return FUEL_PRESSURE;
  else if (fieldName == "LOX PRESSURE")
    return LOX_PRESSURE;
  else if (fieldName == "FUEL LEVEL")
    return FUEL_LEVEL;
  else if (fieldName == "LOX LEVEL")
    return LOX_LEVEL;
  else if (fieldName == "LOX TEMPERATURE")
    return LOX_TEMP;
  else if (fieldName == "N2 TEMPERATURE")
    return N2_TEMP;
  else if (fieldName == "LOX INJ TEMPERATURE")
    return LOX_INJ_TEMP;
  else if (fieldName == "IGNITER PRESSURE")
    return IGNITER_PRESSURE;
  else if (fieldName == "LOX INJ PRESSURE")
    return LOX_INJ_PRESSURE;
  else if (fieldName == "FUEL INJ PRESSURE")
    return FUEL_INJ_PRESSURE;
  else if (fieldName == "CHAMBER PRESSURE")
    return CHAMBER_PRESSURE;
  else if (fieldName == "LPB VOLTAGE")
    return LPB_VOLTAGE;
  else if (fieldName == "HPB VOLTAGE")
    return HPB_VOLTAGE;
  else if (fieldName == "LPB CURRENT")
    return LPB_CURRENT;
  else if (fieldName == "HPB CURRENT")
    return HPB_CURRENT;
  else if (fieldName == "AV FC TEMPERATURE")
    return AV_FC_TEMP;
  else if (fieldName == "AMBIENT TEMPERATURE")
    return AMBIENT_TEMP;
  else if (fieldName == "AV STATE")
    return AV_STATE;
  else if (fieldName == "CAM REC")
    return CAM_REC;
  else if (fieldName == "GNSS CHOICE")
    return GNSS_CHOICE;
  else if (fieldName == "GSE FILLING N2O")
    return GSE_FILLING_N2O;
  else if (fieldName == "VENT GSE")
    return VENT_GSE;
  else if (fieldName == "GSE TANK PRESSURE")
    return GSE_TANK_PRESSURE;
  else if (fieldName == "GSE TANK TEMPERATURE")
    return GSE_TANK_TEMPERATURE;
  else if (fieldName == "GSE FILLING PRESSURE")
    return GSE_FILLING_PRESSURE;
  else if (fieldName == "GSE DISCONNECT ACTIVE")
    return GSE_DISCONNECT_ACTIVE;
  else if (fieldName == "GSE LOADCELL 1")
    return GSE_LOADCELL_1;
  else if (fieldName == "GSE LOADCELL 2")
    return GSE_LOADCELL_2;
  else if (fieldName == "GSE LOADCELL 3")
    return GSE_LOADCELL_3;
  else if (fieldName == "GSE LOADCELL 4")
    return GSE_LOADCELL_4;
  else if (fieldName == "ENGINE STATE")
    return ENGINE_STATE;
  else if (fieldName == "GSE VENT")
    return GSE_VENT;
  else if (fieldName == "GSE CMD STATUS")
    return GSE_CMD_STATUS;
  else if (fieldName == "HOPPER PACKET NBR")
    return HOPPER_PACKET_NBR;
  else if (fieldName == "HOPPER N2O PRESSURE")
    return HOPPER_N2O_PRESSURE;
  else if (fieldName == "HOPPER IGNITER")
    return HOPPER_IGNITER;
  else if (fieldName == "HOPPER ETH PRESSURE")
    return HOPPER_ETH_PRESSURE;
  else if (fieldName == "HOPPER N2O TEMP")
    return HOPPER_N2O_TEMP;
  else if (fieldName == "HOPPER N2O VENT")
    return HOPPER_N2O_VENT;
  else if (fieldName == "HOPPER ETH VENT")
    return HOPPER_ETH_VENT;
  else if (fieldName == "HOPPER N2 SOLENOID")
    return HOPPER_N2_SOL;
  else if (fieldName == "HOPPER N2O MAIN")
    return HOPPER_N2O_MAIN;
  else if (fieldName == "HOPPER N2O MAIN")
    return HOPPER_N2O_SOL;
  else if (fieldName == "HOPPER N2O SOL")
    return HOPPER_ETH_MAIN;
  else if (fieldName == "HOPPER ETH SOL")
    return HOPPER_ETH_SOL;
  else if (fieldName == "HOPPER GNSS LON")
    return HOPPER_GNSS_LON;
  else if (fieldName == "HOPPER GNSS LAT")
    return HOPPER_GNSS_LAT;
  else if (fieldName == "HOPPER SAT NBR")
    return HOPPER_SAT_NBR;
  else if (fieldName == "HOPPER GYRO X")
    return HOPPER_GYRO_X;
  else if (fieldName == "HOPPER GYRO Y")
    return HOPPER_GYRO_Y;
  else if (fieldName == "HOPPER GYRO Z")
    return HOPPER_GYRO_Z;
  else if (fieldName == "HOPPER ACC X")
    return HOPPER_ACC_X;
  else if (fieldName == "HOPPER ACC Y")
    return HOPPER_ACC_Y;
  else if (fieldName == "HOPPER ACC Z")
    return HOPPER_ACC_Z;
  else if (fieldName == "HOPPER BARO")
    return HOPPER_BARO;
  else if (fieldName == "HOPPER KALMAN POS X")
    return HOPPER_KALMAN_POS_X;
  else if (fieldName == "HOPPER KALMAN POS Y")
    return HOPPER_KALMAN_POS_Y;
  else if (fieldName == "HOPPER KALMAN POS Z")
    return HOPPER_KALMAN_POS_Z;
  else if (fieldName == "HOPPER KALMAN YAW")
    return HOPPER_KALMAN_YAW;
  else if (fieldName == "HOPPER KALMAN PITCH")
    return HOPPER_KALMAN_PITCH;
  else if (fieldName == "HOPPER KALMAN ROLL")
    return HOPPER_KALMAN_ROLL;
  else if (fieldName == "HOPPER GIMBAL X")
    return HOPPER_GIMBAL_X;
  else if (fieldName == "HOPPER GIMBAL Y")
    return HOPPER_GIMBAL_Y;
  else if (fieldName == "HOPPER HV VOLTAGE")
    return HOPPER_HV_VOLTAGE;
  else if (fieldName == "HOPPER LV VOLTAGE")
    return HOPPER_LV_VOLTAGE;
  else if (fieldName == "HOPPER AV TEMP")
    return HOPPER_AV_TEMP;
  else if (fieldName == "HOPPER ID CONFIG")
    return HOPPER_ID_CONFIG;
  else if (fieldName == "HOPPER AV STATE")
    return HOPPER_AV_STATE;
  else if (fieldName == "HOPPER FIRE UP STATE")
    return HOPPER_FIREUP_STATE;
  else if (fieldName == "GSE IDLE")
    return GUI_CMD_GSE_IDLE;
  else if (fieldName == "GSE CALIBRATE")
    return GUI_CMD_GSE_CALIBRATE;
  else if (fieldName == "GSE ARM")
    return GUI_CMD_GSE_ARM;
  else if (fieldName == "GSE PASSIVATE")
    return GUI_CMD_GSE_PASSIVATE;
  else if (fieldName == "SERVO 1")
    return GUI_CMD_SERVO_1;
  else if (fieldName == "HOPPER TARE ORIENTATION")
    return GUI_CMD_HOPPER_TARE_ORIENTATION;
  else if (fieldName == "HOPPER TARE")
    return GUI_CMD_HOPPER_TARE;
  else if (fieldName == "HOPPER IDLE")
    return GUI_CMD_HOPPER_IDLE;
  else if (fieldName == "HOMING VALVES")
    return GUI_CMD_HOPPER_HOMING_MAIN_VALVES;
  else if (fieldName == "HOMING GIMBAL")
    return GUI_CMD_HOPPER_HOMING_GIMBAL;

  else if (fieldName == "SERVO 2")
    return GUI_CMD_SERVO_2;
  else if (fieldName == "GSE TOGGLE 11")
    return GUI_CMD_GSE_TOGGLE_11;
  else if (fieldName == "GSE TOGGLE 12")
    return GUI_CMD_GSE_TOGGLE_12;
  else if (fieldName == "GSE TOGGLE 13")
    return GUI_CMD_GSE_TOGGLE_13;
  else if (fieldName == "GSE TOGGLE 14")
    return GUI_CMD_GSE_TOGGLE_14;
  else if (fieldName == "GSE TOGGLE 15")
    return GUI_CMD_GSE_TOGGLE_15;
  else if (fieldName == "GSE TOGGLE 16")
    return GUI_CMD_GSE_TOGGLE_16;
  else if (fieldName == "GSE TOGGLE 21")
    return GUI_CMD_GSE_TOGGLE_21;
  else if (fieldName == "GSE TOGGLE 22")
    return GUI_CMD_GSE_TOGGLE_22;
  else if (fieldName == "GSE TOGGLE 23")
    return GUI_CMD_GSE_TOGGLE_23;
  else if (fieldName == "GSE TOGGLE 24")
    return GUI_CMD_GSE_TOGGLE_24;
  else if (fieldName == "GSE TOGGLE 25")
    return GUI_CMD_GSE_TOGGLE_25;
  else if (fieldName == "GSE TOGGLE 26")
    return GUI_CMD_GSE_TOGGLE_26;
  else if (fieldName == "GSE TOGGLE 27")
    return GUI_CMD_GSE_TOGGLE_27;
  else if (fieldName == "GSE TOGGLE 28")
    return GUI_CMD_GSE_TOGGLE_28;
  else if (fieldName == "CALIBRATE")
    return GUI_CMD_CALIBRATE;
  else if (fieldName == "RECOVER")
    return GUI_CMD_RECOVER;
  else if (fieldName == "CMD GIMBAL X")
    return GUI_CMD_GIMBALL_X;
  else if (fieldName == "CMD GIMBAL Y")
    return GUI_CMD_GIMBALL_Y;
  else if (fieldName == "IGNITION")
    return GUI_CMD_IGNITION;
  else if (fieldName == "MANUAL DEPLOY")
    return GUI_CMD_MANUAL_DEPLOY;
  else if (fieldName == "PRESSURIZE")
    return GUI_CMD_PRESSURIZE;
  else if (fieldName == "ARM")
    return GUI_CMD_ARM;
  else if (fieldName == "LAUNCH")
    return GUI_CMD_LAUNCH;
  else if (fieldName == "ABORT")
    return GUI_CMD_ABORT;
  else if (fieldName == "DISCONNECT")
    return GUI_CMD_DISCONNECT;
  else if (fieldName == "FILLING LOX")
    return GUI_CMD_FILLING_LOX;
  else if (fieldName == "FILLING N2O")
    return GUI_CMD_FILLING_N2O;
  else if (fieldName == "GSE GQN NC1")
    return GSE_GQN_NC1;
  else if (fieldName == "GSE GQN NC2")
    return GSE_GQN_NC2;
  else if (fieldName == "GSE GQN NC3")
    return GSE_GQN_NC3;
  else if (fieldName == "GSE GQN NC4")
    return GSE_GQN_NC4;
  else if (fieldName == "GSE GQN NC5")
    return GSE_GQN_NC5;
  else if (fieldName == "GSE GPN NC1")
    return GSE_GPN_NC1;
  else if (fieldName == "GSE GPN NC2")
    return GSE_GPN_NC2;
  else if (fieldName == "GSE GVN NC")
    return GSE_GVN_NC;
  else if (fieldName == "GSE GFE NC")
    return GSE_GFE_NC;
  else if (fieldName == "GSE GFO NCC")
    return GSE_GFO_NCC;
  else if (fieldName == "GSE GDO NCC")
    return GSE_GDO_NCC;
  else if (fieldName == "GSE PC OLC")
    return GSE_PC_OLC;
  else if (fieldName == "GSE GP1")
    return GSE_GP1;
  else if (fieldName == "GSE GP2")
    return GSE_GP2;
  else if (fieldName == "GSE GP3")
    return GSE_GP3;
  else if (fieldName == "GSE GP4")
    return GSE_GP4;
  else if (fieldName == "GSE GP5")
    return GSE_GP5;
  else
    return UNKNOWN;
}

} // namespace fieldUtil

#endif /* ENUM_GUI_FIELD_UTIL */
