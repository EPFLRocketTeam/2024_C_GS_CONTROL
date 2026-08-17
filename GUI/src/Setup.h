/**
                          __  _               __  _  _
                         / _|(_)             / _|(_)| |
       ___  ___   _ __  | |_  _   __ _      | |_  _ | |  ___
      / __|/ _ \ | '_ \ |  _|| | / _` |     |  _|| || | / _ \
     | (__| (_) || | | || |  | || (_| |     | |  | || ||  __/
      \___|\___/ |_| |_||_|  |_| \__, |     |_|  |_||_| \___|
                                  __/ |
                                 |___/
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief config file to setup GUI
*/

#ifndef SETUP_H
#define SETUP_H

#include "FieldUtil.h"
#include "GSManagerView.h"
#include "TelemetryView.h"
#include "ValveControlView.h"
#include "components/IcarusCommandsView.h"
#include <QException>
#include <QFile>
#include <QTextStream>
#include <qboxlayout.h>
#include <qframe.h>
#include <qlist.h>
#include <qmap.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qpushbutton.h>
#include <stdexcept>
// ----------------------------- Setup Views -----------------------------------
using LeftView = TelemetryView;
using MiddleView = ValveControlView;
using RightView = GSManagerView;

// CHange the debug level to show the debug messages
#define DEBUG_LOG true

// Launch timer configuration
constexpr double LAUNCH_DELAY = -30.0; // seconds

// <---------- DO NOT CHANGE ---------->
#ifdef ICARUS_GUI
namespace ui_elements {
// Instantiate a QMap with std::string keys and std::vector<std::string> values
inline QList<std::vector<GUI_FIELD>> valvesMap{
    {GUI_FIELD::HOPPER_N2O_VENT, GUI_FIELD::HOPPER_ETH_VENT,
     GUI_FIELD::HOPPER_N2O_MAIN, GUI_FIELD::HOPPER_ETH_MAIN},
    {GUI_FIELD::GSE_VENT, GUI_FIELD::GSE_FILLING_N2O}};

inline QList<std::vector<GUI_FIELD>> pushButtonMap{
    {GUI_CMD_HOPPER_IDLE, GUI_CMD_HOPPER_TARE_ORIENTATION, GUI_CMD_ARM,
     GUI_CMD_LAUNCH, GUI_CMD_ABORT, GUI_CMD_HOPPER_TARE,
     GUI_CMD_HOPPER_HOMING_X, GUI_CMD_HOPPER_HOMING_Y,
     GUI_CMD_HOPPER_HOMING_MAIN_FUEL, GUI_CMD_HOPPER_HOMING_MAIN_N2O, GUI_CMD_HOPPER_SPARE_F1, GUI_CMD_HOPPER_SPARE_F2, GUI_CMD_HOPPER_SPARE_F3},
    {GUI_CMD_DISCONNECT}};
inline QMap<std::string, QList<std::vector<GUI_FIELD>>> controlMap{
    {"ValveControlButton", valvesMap}, {"QPushButton", pushButtonMap}};

inline std::vector<ValveInfo> valves = {{GUI_FIELD::GSE_VENT,
                                         {0.234569, 0.668},
                                         ValveButton::Orientation::Vertical,
                                         false},
                                        {GUI_FIELD::GSE_FILLING_N2O,
                                         {0.23, 0.45},
                                         ValveButton::Orientation::Horizontal, false},

                                        {GUI_FIELD::HOPPER_N2O_VENT,
                                         {0.50665, 0.318},
                                         ValveButton::Orientation::Horizontal, false},
                                        {GUI_FIELD::HOPPER_ETH_VENT,
                                         {0.785838, 0.318},
                                         ValveButton::Orientation::Horizontal, false},
                                        {GUI_FIELD::HOPPER_N2_SOL,
                                         {0.645736, 0.124315},
                                         ValveButton::Orientation::Horizontal, false},

                                        {GUI_FIELD::HOPPER_N2O_SOL,
                                         {0.585838, 0.638},
                                         ValveButton::Orientation::Vertical, false},
                                        {GUI_FIELD::HOPPER_N2O_MAIN,
                                         {0.585838, 0.57},
                                         ValveButton::Orientation::Vertical, true},


                                        {GUI_FIELD::HOPPER_ETH_SOL,
                                         {0.703604, 0.638},
                                         ValveButton::Orientation::Vertical, false},
                                        {GUI_FIELD::HOPPER_ETH_MAIN,
                                         {0.703604, 0.57},
                                         ValveButton::Orientation::Vertical, true},

                                        {GUI_FIELD::HOPPER_IGNITER,
                                         {0.5, 0.85},
                                         ValveButton::Orientation::Vertical, false}};

inline QList<std::vector<GUI_FIELD>> gseValvesMap{
    {GUI_FIELD::GSE_GQN_NC1, GUI_FIELD::GSE_GQN_NC2, GSE_GQN_NC5, GSE_GPN_NC,
     GSE_GPA_NC, GSE_GVN_NC, GSE_GFE_NC, GSE_GFO_NCC, GSE_GDO_NCC, GSE_PC_OLC}};

inline QList<std::vector<GUI_FIELD>> gsePushButtonMap{
    {GUI_CMD_GSE_IDLE, GUI_CMD_GSE_CALIBRATE, GUI_CMD_GSE_ARM,
     GUI_CMD_GSE_PASSIVATE}};
inline QMap<std::string, QList<std::vector<GUI_FIELD>>> gseControlMap{
    {"ValveControlButton", gseValvesMap}, {"QPushButton", gsePushButtonMap}};

inline std::vector<LabelInfo> labels = {
    {GUI_FIELD::GSE_TANK_PRESSURE, 0.09, 0.25},
    {GUI_FIELD::GSE_TANK_TEMPERATURE, 0.09, 0.315},
    {GUI_FIELD::GSE_FILLING_PRESSURE, 0.138, 0.576},
    {GUI_FIELD::HOPPER_CHAMBER_PRESSURE, 0.839595, 0.82105},
    {GUI_FIELD::HOPPER_O_INJ_PRESSURE, 0.89, 0.74105},
    {GUI_FIELD::HOPPER_ETH_INJ_PRESSURE, 0.447, 0.75105},
    {GUI_FIELD::HOPPER_O_LINE_PRESSURE, 0.89, 0.65},
    {GUI_FIELD::HOPPER_ETH_LINE_PRESSURE, 0.447, 0.64},
    {GUI_FIELD::HOPPER_N2O_PRESSURE, 0.447, 0.439},
    {GUI_FIELD::HOPPER_N2O_TEMP, 0.447, 0.502},
    {GUI_FIELD::HOPPER_ETH_PRESSURE, 0.893, 0.438}};

inline std::vector<ValveInfo> gseValves = {
    // LOX quadrant
    {GUI_FIELD::GSE_GFO_NCC,
     {0.186, 0.35},
     ValveButton::Orientation::Horizontal,false},
    {GUI_FIELD::GSE_GDO_NCC,
     {0.186, 0.247},
     ValveButton::Orientation::Horizontal,false},

    // Ethanol quadrant
    {GUI_FIELD::GSE_GFE_NC, {0.7, 0.382}, ValveButton::Orientation::Horizontal,false},
    {GUI_FIELD::GSE_PUMP, {0.575, 0.382}, ValveButton::Orientation::Horizontal,false},

    // N2 quadrant
    {GUI_FIELD::GSE_GPN_NC, {0.15, 0.58}, ValveButton::Orientation::Horizontal,false},
    {GUI_FIELD::GSE_GPA_NC, {0.15, 0.7}, ValveButton::Orientation::Horizontal,false},
    {GUI_FIELD::GSE_GVN_NC, {0.7, 0.565}, ValveButton::Orientation::Horizontal,false},

    // Air valves

    {GUI_FIELD::GSE_GQN_NC1, {0.515, 0.08}, ValveButton::Orientation::Vertical,false},
    {GUI_FIELD::GSE_GQN_NC2, {0.585, 0.08}, ValveButton::Orientation::Vertical,false},
    {GUI_FIELD::GSE_GQN_NC3, {0.655, 0.08}, ValveButton::Orientation::Vertical,false},
    {GUI_FIELD::GSE_GQN_NC4, {0.725, 0.08}, ValveButton::Orientation::Vertical,false},
    {GUI_FIELD::GSE_GQN_NC5, {0.78, 0.08}, ValveButton::Orientation::Vertical,false},
    {GUI_FIELD::GSE_GQN_NC5, {0.78, 0.08}, ValveButton::Orientation::Vertical,false},

};

inline std::vector<LabelInfo> gseLabels = {};

inline QList<GUI_FIELD> gps = {
    HOPPER_GNSS_LON,
    HOPPER_GNSS_LAT,
    HOPPER_SAT_NBR,
};
inline QList<GUI_FIELD> imu = {
    HOPPER_GYRO_X, HOPPER_GYRO_Y, HOPPER_GYRO_Z,
    HOPPER_ACC_X,  HOPPER_ACC_Y,  HOPPER_ACC_Z,
};
inline QList<GUI_FIELD> position = {
    HOPPER_BARO,         HOPPER_KALMAN_POS_X, HOPPER_KALMAN_POS_Y,
    HOPPER_KALMAN_POS_Z, HOPPER_KALMAN_YAW,   HOPPER_KALMAN_PITCH,
    HOPPER_KALMAN_ROLL,
};
inline QList<GUI_FIELD> tempProbe = {
    HOPPER_TEMP1,
    HOPPER_TEMP2,
    HOPPER_TEMP3,
    };
inline QList<GUI_FIELD> tbd = {HOPPER_GIMBAL_X,     HOPPER_GIMBAL_Y,
                               HOPPER_HV_VOLTAGE, HOPPER_LV_VOLTAGE,   HOPPER_AV_TEMP,
                               HOPPER_FIREUP_STATE, HOPPER_AV_STATE};
inline QMap<QString, QList<GUI_FIELD>> data_sections = {
    {"GPS", gps},
    {"Imu", imu},
    {"Vehicule Spatial Data", position},
    {"Temperature Probes", tempProbe},
    {"TBD", tbd}};

inline QList<GUI_FIELD> gseDataFields = {
    GSE_FILLING_N2O, HOPPER_LOADCELL1, HOPPER_LOADCELL2,
    HOPPER_LOADCELL3,  HOPPER_LOADCELL4,
};
inline QMap<QString, QList<GUI_FIELD>> gse_sections = {{"GSE", gseDataFields}};

inline QFrame *middlePlaceholder;
inline QFrame *leftPlaceholder;
inline QFrame *gseMiddlePlaceholder;
inline QFrame *rightPlaceholder;

inline QString connectedBackgroundImage = ":/images/prop_icarus_connect.svg";
inline QString disconnectedBackgroundImage =
    ":/images/prop_icarus_disconnect.svg";

inline void init_views() {
  middlePlaceholder = new ValveControlView(
      valves, labels, connectedBackgroundImage, disconnectedBackgroundImage);
  gseMiddlePlaceholder =
      new ValveControlView(gseValves, gseLabels, ":/images/pfs_firehorn2_V1.svg",
                           ":/images/pfs_firehorn2_V1.svg");

  leftPlaceholder = new TelemetryView(data_sections);
  QVBoxLayout *rightLayout = new QVBoxLayout;
  rightLayout->setAlignment(Qt::AlignLeft);
  rightLayout->addWidget(new GSManagerView());
  rightLayout->addWidget(new IcarusCommandsView);
  rightLayout->addWidget(new TelemetryView(gse_sections));
  rightLayout->addStretch(1);
  rightLayout->setContentsMargins(0, 0, 0, 0);
  rightPlaceholder = new QFrame();
  rightPlaceholder->setContentsMargins(0, 0, 0, 0);
  rightPlaceholder->setLayout(rightLayout);
}
} // namespace ui_elements
#endif

#ifdef FIREHORN_GUI
namespace ui_elements {
// Instantiate a QMap with std::string keys and std::vector<std::string> values

inline QList<std::vector<GUI_FIELD>> pushButtonMap{
    {GUI_CMD_CALIBRATE,GUI_CMD_ARM,GUI_CMD_PRESSURIZE,GUI_CMD_LAUNCH, GUI_CMD_RECOVER},

    {GUI_CMD_ABORT},
};
inline QMap<std::string, QList<std::vector<GUI_FIELD>>> controlMap{
    {"QPushButton", pushButtonMap}};

inline QList<std::vector<GUI_FIELD>> gsePushButtonMap{
    {GUI_CMD_GSE_IDLE, GUI_CMD_GSE_CALIBRATE, GUI_CMD_GSE_ARM,
     GUI_CMD_GSE_PASSIVATE,GUI_CMD_GSE_TOGGLE_ALL_GQD}};
inline QMap<std::string, QList<std::vector<GUI_FIELD>>> gseControlMap{
    {"QPushButton", gsePushButtonMap}};

inline float c_line = 0.4861;
inline std::vector<ValveInfo> valves = {

    {GUI_FIELD::SECURITY_DPR_FUEL, {c_line,0.139}, ValveButton::Orientation::Vertical, false},
    {GUI_FIELD::SECURITY_DPR_LOX, {c_line,0.55}, ValveButton::Orientation::Vertical, false},
    {GUI_FIELD::MAIN_LOX, {c_line, 0.815}, ValveButton::Orientation::Vertical, false},
    {GUI_FIELD::MAIN_FUEL, {0.374, 0.84}, ValveButton::Orientation::Vertical, false},
    {GUI_FIELD::VENT_FUEL, {0.35, 0.252}, ValveButton::Orientation::Horizontal, false},
    {GUI_FIELD::VENT_LOX, {0.57, 0.652}, ValveButton::Orientation::Horizontal, false},
};
inline std::vector<BallValveInfo> dpr_valves = {
    {GUI_FIELD::PRESSURE_VALVE_FUEL,
     {c_line, 0.24},
     BallValveButton::Orientation::Vertical, false},
    {GUI_FIELD::PRESSURE_VALVE_LOX,
     {c_line, 0.637},
     BallValveButton::Orientation::Vertical, false},
};

inline std::vector<LabelInfo> labels = {

    {GUI_FIELD::N2_PRESSURE_1, 0.685, 0.058},

    {GUI_FIELD::N2_PRESSURE_2, 0.685, 0.456},


    {GUI_FIELD::LOX_PRESSURE, 0.685, 0.705},
    {GUI_FIELD::LOX_FLS_TEMP_8, 0.92, 0.632},           // LOX float level sensor at TBD%
    {GUI_FIELD::LOX_FLS_TEMP_7, 0.92, 0.632 + 0.044*1}, // LOX float level sensor at TBD%
    {GUI_FIELD::LOX_FLS_TEMP_6, 0.92, 0.632 + 0.044*2}, // LOX float level sensor at TBD%
    {GUI_FIELD::LOX_FLS_TEMP_5, 0.92, 0.632 + 0.044*3}, // LOX float level sensor at TBD%
    {GUI_FIELD::LOX_FLS_TEMP_4, 0.92, 0.632 + 0.044*4}, // LOX float level sensor at TBD%
    {GUI_FIELD::LOX_FLS_TEMP_3, 0.92, 0.632 + 0.044*5}, // LOX float level sensor at TBD%
    {GUI_FIELD::LOX_FLS_TEMP_2, 0.92, 0.632 + 0.044*6}, // LOX float level sensor at TBD%
    {GUI_FIELD::LOX_FLS_TEMP_1, 0.92, 0.632 + 0.044*7}, // LOX float level sensor at TBD%

    {GUI_FIELD::LOX_INJ_PRESSURE, 0.685, 0.85},
    //{GUI_FIELD::LOX_INJ_TEMP, 0.67, 0.865},

    {GUI_FIELD::CHAMBER_PRESSURE, 0.685, 0.914},
    {GUI_FIELD::CHAMBER_TEMP, 0.685, 0.96},

    {GUI_FIELD::FUEL_PRESSURE, 0.22, 0.33},

    {GUI_FIELD::FUEL_INJ_PRESSURE, 0.18, 0.8926},

    {GUI_FIELD::PRESSURE_VALVE_FUEL,c_line + 0.1f , 0.24}, 
    {GUI_FIELD::PRESSURE_VALVE_LOX, c_line + 0.1f , 0.637}, 

};

inline std::vector<ValveInfo> gseValves = {
    // LOX quadrant
    {GUI_FIELD::GSE_GFO_NCC,
     {0.2, 0.698},
     ValveButton::Orientation::Horizontal,false},
    {GUI_FIELD::GSE_GDO_NCC,
     {0.3, 0.59},
     ValveButton::Orientation::Horizontal,false},

    // Ethanol quadrant
    {GUI_FIELD::GSE_GFE_NC, {0.32, 0.498}, ValveButton::Orientation::Horizontal,false},
    {GUI_FIELD::GSE_PUMP, {0.15, 0.498}, ValveButton::Orientation::Horizontal,false},

    // N2 quadrant
    {GUI_FIELD::GSE_GPN_NC, {0.78, 0.067}, ValveButton::Orientation::Horizontal,false},
    {GUI_FIELD::GSE_GVN_NC, {0.9, 0.14}, ValveButton::Orientation::Horizontal,false},
    {GUI_FIELD::GSE_GPA_NC, {0.52, 0.15}, ValveButton::Orientation::Horizontal,false},

    {GUI_FIELD::GSE_GFD_NC, {0.15, 0.879}, ValveButton::Orientation::Horizontal,false},
    {GUI_FIELD::GSE_GDD_NC, {0.45, 0.7}, ValveButton::Orientation::Horizontal,false},

    // Air valves for Quick Disconnect Actuation
    {GUI_FIELD::GSE_GQD1_NC, {0.59, 0.578},         ValveButton::Orientation::Horizontal,false},
    {GUI_FIELD::GSE_GQD2_NC, {0.59, 0.579 + 0.075*1}, ValveButton::Orientation::Horizontal,false},
    {GUI_FIELD::GSE_GQD3_NC, {0.59, 0.579 + 0.075*2}, ValveButton::Orientation::Horizontal,false},
    {GUI_FIELD::GSE_GQD4_NC, {0.59, 0.579 + 0.075*3}, ValveButton::Orientation::Horizontal,false},
    {GUI_FIELD::GSE_GQD5_NC, {0.59, 0.579 + 0.075*4}, ValveButton::Orientation::Horizontal,false},
    {GUI_FIELD::GSE_GQD6_NC, {0.59, 0.579 + 0.075*5}, ValveButton::Orientation::Horizontal,false},
};
inline std::vector<BallValveInfo> gseDpr_valves = {};
inline std::vector<LabelInfo> gseLabels = {
    {GUI_FIELD::GSE_GP1, 0.985, 0.6},
    {GUI_FIELD::GSE_GP2, 0.985, 0.6 + 0.06*1},
    {GUI_FIELD::GSE_GP3, 0.985, 0.6 + 0.06*2},
    {GUI_FIELD::GSE_GP4, 0.985, 0.6 + 0.06*3},
    {GUI_FIELD::GSE_GP5, 0.985, 0.6 + 0.06*4},

};

inline QList<GUI_FIELD> gps = {
    GNSS_LON,
    GNSS_LAT,
    GNSS_ALT,
    DOWNRANGE
};

inline QList<GUI_FIELD> tbd = {
    PACKET_NBR,  
    AV_STATE,    
    LPB_VOLTAGE,  
    LPB_CURRENT,
    VOUT_5V_VOLTAGE,
    VOUT_5V_CURRENT,
    VOUT_24V_VOLTAGE, 
    VOUT_24V_CURRENT, 
    AV_FC_TEMP,     
    AMBIENT_TEMP,
    CAM_REC,     
    AV_PYROS,
    AV_PACKET_FREQ
};
inline QMap<QString, QList<GUI_FIELD>> data_sections = {{"GPS", gps},
                                                        {"INFOS", tbd}};

inline QList<GUI_FIELD> gseDataFields = {
    GSE_GP1, GSE_GP2, GSE_GP3, GSE_GP4, GSE_GP5,
};
inline QMap<QString, QList<GUI_FIELD>> gse_sections = {{"GSE", gseDataFields}};

inline QFrame *middlePlaceholder;
inline QFrame *leftPlaceholder;
inline QFrame *rightPlaceholder;

inline QFrame *gseMiddlePlaceholder;

/*inline QString connectedBackgroundImage =
 * ":/images/prop_firehorn_connect.svg";*/
/*inline QString disconnectedBackgroundImage =*/
/*    ":/images/prop_firehorn_disconnect.svg";*/
inline QString connectedBackgroundImage = ":/images/prop_firehorn2_V1.svg";
inline QString disconnectedBackgroundImage = ":/images/prop_firehorn2_V1.svg";

inline void init_views() {
  middlePlaceholder = new ValveControlView(
      valves, dpr_valves, labels, connectedBackgroundImage, disconnectedBackgroundImage);
  gseMiddlePlaceholder =
      new ValveControlView(gseValves, gseDpr_valves, gseLabels, ":/images/pfs_firehorn2_V1.svg",
                           ":/images/pfs_firehorn2_V1.svg");

  leftPlaceholder = new TelemetryView(data_sections);
  QVBoxLayout *rightLayout = new QVBoxLayout;
  rightLayout->setAlignment(Qt::AlignLeft);
  rightLayout->addWidget(new GSManagerView());
  // rightLayout->addWidget(new IcarusCommandsView);
  rightLayout->addWidget(new TelemetryView(gse_sections));
  rightLayout->addStretch(1);
  rightLayout->setContentsMargins(0, 0, 0, 0);
  rightPlaceholder = new QFrame();
  rightPlaceholder->setContentsMargins(0, 0, 0, 0);
  rightPlaceholder->setLayout(rightLayout);
}
} // namespace ui_elements
#endif

// ----------------------------- MainWindow setup ------------------------------
namespace mws {
const QString title = "Firehorn Project Ground Control Station";
const int x = 100;
const int y = 100;
const int width = 1080;
const int height = 1920;
const int middleSectionWidth = 60; // % left and right  will be (100-x)/2
const int sideWidth = (100 - middleSectionWidth) / 2;
} // namespace mws

namespace auth {
inline QString key; // Define the key as an inline variable

// Function to load the key from a file
inline void loadKeyFromFile(const QString &filePath) {
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    std::string error_msg =
        "Failed to open the keys file at path: " + filePath.toStdString();
    throw std::runtime_error(error_msg);
  }

  QTextStream in(&file);
  key = in.readLine().trimmed(); // Read the first line and trim whitespace
  file.close();

  if (key.isEmpty()) {
    throw std::runtime_error("The key file is empty or invalid.");
  }
}
} // namespace auth

namespace network {
const QString serverIP = "127.0.0.1";
const int serverPort = 12345;
} // namespace network
// ----------------------------- Colour ----------------------------------------
namespace col {
inline QString backgroundColorCode = "#131313";
inline QString primary = "#FFFFFF";
inline QString secondary = "#FFFFFF";
inline QString accent = "#F5251A";
inline QString complementary = "#457069";
inline QString complementaryLighter = "#538078";

inline QString background() {
  return "background-color: " + backgroundColorCode + ";";
}
inline QString defaultCardStyle(QString id) {
  return QString(R"(
        #%3 {
        background: qradialgradient(cx:0.5, cy:0.5, radius:0.5, fx:0.5, fy:0.5, stop:0 %1, stop:0.95 %2);
        border-radius: 10%;
        border-width: 2px;
        border-color: #FFFFFF;
        }
        #child {
            color: #FFFFFF;
            background: transparent;
        }
        )")
      .arg(col::backgroundColorCode)
      .arg("rgba(15, 15, 20, 225)")
      .arg(id);
}

inline QString labelStyle =
    QString(
        "font-size: 14pt; color: %1;font-weight: 400;background: transparent;")
        .arg(col::primary);
inline QString getButtonStyle(QString id) {
  return QString(R"(
            QPushButton#%5 {
            color: %4;
            font: bold 14px;
            background: %1;
            border:2px solid %1;
            border-radius: 10px;
            }
            QPushButton#%5:hover {
                background-color: %3;     
                
            }
            QPushButton#%5:pressed {
                background-color: %2!important;     
                border:2px solid %4;
            }
            
        )")
      .arg(col::complementary)
      .arg(col::backgroundColorCode)
      .arg(col::complementaryLighter)
      .arg(col::primary)
      .arg(id);
}
} // namespace col

#endif /* SETUP_H */
