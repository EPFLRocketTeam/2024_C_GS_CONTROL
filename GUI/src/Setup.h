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

// <---------- DO NOT CHANGE ---------->
#ifdef ICARUS_GUI
namespace ui_elements {
// Instantiate a QMap with std::string keys and std::vector<std::string> values
inline QList<std::vector<GUI_FIELD>> valvesMap{
    {
     GUI_FIELD::HOPPER_N2O_VENT, GUI_FIELD::HOPPER_ETH_VENT,
      GUI_FIELD::HOPPER_N2O_MAIN, GUI_FIELD::HOPPER_ETH_MAIN},
    {GUI_FIELD::GSE_VENT, GUI_FIELD::GSE_FILLING_N2O}};

inline QList<std::vector<GUI_FIELD>> pushButtonMap{
    {
     GUI_CMD_HOPPER_IDLE, GUI_CMD_HOPPER_TARE_ORIENTATION, GUI_CMD_ARM, GUI_CMD_LAUNCH,
      GUI_CMD_ABORT, GUI_CMD_HOPPER_TARE, GUI_CMD_HOPPER_HOMING_GIMBAL,
      GUI_CMD_HOPPER_HOMING_MAIN_VALVES},
    {GUI_CMD_DISCONNECT}};
inline QMap<std::string, QList<std::vector<GUI_FIELD>>> controlMap{
    {"ValveControlButton", valvesMap}, {"QPushButton", pushButtonMap}};

inline std::vector<ValveInfo> valves = {{GUI_FIELD::GSE_VENT,
                                         {0.234569, 0.668},
                                         ValveButton::Orientation::Vertical},
                                        {GUI_FIELD::GSE_FILLING_N2O,
                                         {0.35, 0.5},
                                         ValveButton::Orientation::Horizontal
                                        },

                                        {GUI_FIELD::HOPPER_N2O_VENT,
                                         {0.50665, 0.338},
                                         ValveButton::Orientation::Horizontal},
                                        {GUI_FIELD::HOPPER_ETH_VENT,
                                         {0.785838, 0.338},
                                         ValveButton::Orientation::Horizontal},
                                        {GUI_FIELD::HOPPER_N2_SOL,
                                         {0.645736, 0.144315},
                                         ValveButton::Orientation::Horizontal},
                                        {GUI_FIELD::HOPPER_N2O_SOL,
                                         {0.585838, 0.638},
                                         ValveButton::Orientation::Vertical},
                                        {GUI_FIELD::HOPPER_ETH_SOL,
                                         {0.703604, 0.638},
                                         ValveButton::Orientation::Vertical},
                                        {GUI_FIELD::HOPPER_IGNITER,
                                         {0.55, 0.75},
                                         ValveButton::Orientation::Vertical}
                                        };

inline QList<std::vector<GUI_FIELD>> gseValvesMap{
    {
     GUI_FIELD::GSE_GQN_NC1, GUI_FIELD::GSE_GQN_NC2, GSE_GQN_NC5, GSE_GPN_NC1,
      GSE_GPN_NC2, GSE_GVN_NC, GSE_GFE_NC, GSE_GFO_NCC, GSE_GDO_NCC,
      GSE_PC_OLC}};

inline QList<std::vector<GUI_FIELD>> gsePushButtonMap{
    {
     GUI_CMD_GSE_IDLE, GUI_CMD_GSE_CALIBRATE, GUI_CMD_GSE_ARM,
      GUI_CMD_GSE_PASSIVATE}};
inline QMap<std::string, QList<std::vector<GUI_FIELD>>>
    gseControlMap{{"ValveControlButton", gseValvesMap},
                  {"QPushButton", gsePushButtonMap}};

inline std::vector<LabelInfo> labels = {
    {GUI_FIELD::GSE_TANK_PRESSURE, 0.09, 0.25},
    {GUI_FIELD::GSE_TANK_TEMPERATURE, 0.09, 0.315},
    {GUI_FIELD::GSE_FILLING_PRESSURE, 0.138, 0.576},
    {GUI_FIELD::HOPPER_CHAMBER_PRESSURE, 0.809595, 0.78905},
    {GUI_FIELD::HOPPER_N2O_PRESSURE, 0.447, 0.439},
    {GUI_FIELD::HOPPER_N2O_TEMP, 0.447, 0.502},
    {GUI_FIELD::HOPPER_ETH_PRESSURE, 0.893, 0.438}};

inline std::vector<ValveInfo> gseValves = {
    // LOX quadrant
    {GUI_FIELD::GSE_GFO_NCC,
     {0.186, 0.35},
     ValveButton::Orientation::Horizontal},
    {GUI_FIELD::GSE_GDO_NCC,
     {0.186, 0.247},
     ValveButton::Orientation::Horizontal},

    // Ethanol quadrant
    {GUI_FIELD::GSE_GFE_NC,
     {0.7, 0.382},
     ValveButton::Orientation::Horizontal},

    // N2 quadrant
    {GUI_FIELD::GSE_GPN_NC1,
     {0.15, 0.58},
     ValveButton::Orientation::Horizontal},
    {GUI_FIELD::GSE_GPN_NC2,
     {0.15, 0.7},
     ValveButton::Orientation::Horizontal},
    {GUI_FIELD::GSE_GVN_NC, {0.7, 0.565}, ValveButton::Orientation::Horizontal},

    // Air valves

    {GUI_FIELD::GSE_GQN_NC1,
     {0.515, 0.08},
     ValveButton::Orientation::Vertical},
    {GUI_FIELD::GSE_GQN_NC2,
     {0.585, 0.08},
     ValveButton::Orientation::Vertical},
    {GUI_FIELD::GSE_GQN_NC3,
     {0.655, 0.08},
     ValveButton::Orientation::Vertical},
    {GUI_FIELD::GSE_GQN_NC4,
     {0.725, 0.08},
     ValveButton::Orientation::Vertical},
    {GUI_FIELD::GSE_GQN_NC5,
     {0.78, 0.08},
     ValveButton::Orientation::Vertical},
    {GUI_FIELD::GSE_GQN_NC5,
     {0.78, 0.08},
     ValveButton::Orientation::Vertical},

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
inline QList<GUI_FIELD> tbd = {
    HOPPER_GIMBAL_X, HOPPER_GIMBAL_Y,     HOPPER_HV_VOLTAGE,
    HOPPER_AV_TEMP,  HOPPER_FIREUP_STATE, HOPPER_AV_STATE
};
inline QMap<QString, QList<GUI_FIELD>> data_sections = {
    {"GPS", gps},
    {"Imu", imu},
    {"Vehicule Spatial Data", position},
    {"TBD", tbd}};

inline QList<GUI_FIELD> gseDataFields = {
    GSE_FILLING_N2O, GSE_LOADCELL_1, GSE_LOADCELL_2,
    GSE_LOADCELL_3,  GSE_LOADCELL_4,
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
      new ValveControlView(gseValves, gseLabels, ":/images/gse_plumbing.svg",
                           ":/images/gse_plumbing.svg");

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
inline QList<std::vector<GUI_FIELD>> valvesMap{
    {
     
         GUI_FIELD::MAIN_LOX,
         GUI_FIELD::MAIN_FUEL,
         GUI_FIELD::VENT_LOX,
         GUI_FIELD::VENT_FUEL,
         GUI_FIELD::VENT_N2,
         GUI_FIELD::PRESSURE_VALVE_LOX,
         GUI_FIELD::PRESSURE_VALVE_FUEL,
     }};

inline QList<std::vector<GUI_FIELD>> pushButtonMap{
    {
     GUI_CMD_CALIBRATE, GUI_CMD_RECOVER},
    {
     GUI_CMD_ARM},
    {
     GUI_CMD_LAUNCH},
    {
     GUI_CMD_ABORT},
};
inline QMap<std::string, QList<std::vector<GUI_FIELD>>> controlMap{
    /*{"ValveControlButton", valvesMap},*/ {"QPushButton", pushButtonMap}};

inline QList<std::vector<GUI_FIELD>> gseValvesMap{
    {
     GUI_FIELD::GSE_GQN_NC1, GUI_FIELD::GSE_GQN_NC2, GSE_GQN_NC5, GSE_GPN_NC1,
      GSE_GPN_NC2, GSE_GVN_NC, GSE_GFE_NC, GSE_GFO_NCC, GSE_GDO_NCC,
      GSE_PC_OLC}};

inline QList<std::vector<GUI_FIELD>> gsePushButtonMap{
    {
     GUI_CMD_GSE_IDLE, GUI_CMD_GSE_CALIBRATE, GUI_CMD_GSE_ARM,
      GUI_CMD_GSE_PASSIVATE}};
inline QMap<std::string, QList<std::vector<GUI_FIELD>>>
    gseControlMap{/*{"ValveControlButton", gseValvesMap},*/
                  {"QPushButton", gsePushButtonMap}};

inline std::vector<ValveInfo> valves = {
    /*{GUI_FIELD::GSE_VENT, {0.24, 0.668}, ValveButton::Orientation::Horizontal},*/
    {GUI_FIELD::VENT_N2, {0.55, -0.005}, ValveButton::Orientation::Horizontal},
    {GUI_FIELD::MAIN_LOX, {0.487, 0.75}, ValveButton::Orientation::Vertical},
    {GUI_FIELD::MAIN_FUEL, {0.4, 0.78}, ValveButton::Orientation::Vertical},
    {GUI_FIELD::VENT_FUEL,
     {0.386, 0.249},
     ValveButton::Orientation::Horizontal},
    {GUI_FIELD::VENT_LOX, {0.625, 0.498}, ValveButton::Orientation::Horizontal},
    {GUI_FIELD::PRESSURE_VALVE_FUEL,
     {0.513, 0.225},
     ValveButton::Orientation::Vertical},
    {GUI_FIELD::PRESSURE_VALVE_LOX,
     {0.572, 0.33},
     ValveButton::Orientation::Vertical},
};

inline std::vector<LabelInfo> labels = {

    {GUI_FIELD::N2_PRESSURE, 0.68, 0.08},
    {GUI_FIELD::N2_TEMP, 0.68, 0.15},

    {GUI_FIELD::LOX_PRESSURE, 0.69, 0.58},
    {GUI_FIELD::LOX_TEMP, 0.69, 0.66},

    {GUI_FIELD::LOX_INJ_PRESSURE, 0.69, 0.785},
    {GUI_FIELD::LOX_INJ_TEMP, 0.69, 0.86},

    {GUI_FIELD::CHAMBER_PRESSURE, 0.69, 0.95},


    {GUI_FIELD::FUEL_PRESSURE, 0.35, 0.36},

    {GUI_FIELD::FUEL_INJ_PRESSURE, 0.3, 0.89},
 
};

inline std::vector<ValveInfo> gseValves = {
    // LOX quadrant
    {GUI_FIELD::GSE_GFO_NCC,
     {0.186, 0.35},
     ValveButton::Orientation::Horizontal},
    {GUI_FIELD::GSE_GDO_NCC,
     {0.186, 0.247},
     ValveButton::Orientation::Horizontal},

    // Ethanol quadrant
    {GUI_FIELD::GSE_GFE_NC,
     {0.7, 0.382},
     ValveButton::Orientation::Horizontal},

    // N2 quadrant
    {GUI_FIELD::GSE_GPN_NC1,
     {0.15, 0.58},
     ValveButton::Orientation::Horizontal},
    {GUI_FIELD::GSE_GPN_NC2,
     {0.15, 0.7},
     ValveButton::Orientation::Horizontal},
    {GUI_FIELD::GSE_GVN_NC, {0.7, 0.565}, ValveButton::Orientation::Horizontal},

    // Air valves

    {GUI_FIELD::GSE_GQN_NC1,
     {0.515, 0.08},
     ValveButton::Orientation::Vertical},
    {GUI_FIELD::GSE_GQN_NC2,
     {0.585, 0.08},
     ValveButton::Orientation::Vertical},
    {GUI_FIELD::GSE_GQN_NC3,
     {0.655, 0.08},
     ValveButton::Orientation::Vertical},
    {GUI_FIELD::GSE_GQN_NC4,
     {0.725, 0.08},
     ValveButton::Orientation::Vertical},
    {GUI_FIELD::GSE_GQN_NC5,
     {0.78, 0.08},
     ValveButton::Orientation::Vertical},
    {GUI_FIELD::GSE_GQN_NC5,
     {0.78, 0.08},
     ValveButton::Orientation::Vertical},

};

inline std::vector<LabelInfo> gseLabels = {};

inline QList<GUI_FIELD> gps = {
    GNSS_LON,
    GNSS_LAT,
    GNSS_ALT,
    GNSS_VERTICAL_SPEED,
};

inline QList<GUI_FIELD> tbd = {
    PACKET_NBR, AV_STATE, LPB_VOLTAGE, LPB_CURRENT, HPB_VOLTAGE, HPB_CURRENT, AV_FC_TEMP, AMBIENT_TEMP, CAM_REC,
};
inline QMap<QString, QList<GUI_FIELD>> data_sections = {{"GPS", gps},
                                                        {"TBD", tbd}};

inline QList<GUI_FIELD> gseDataFields = {
    GSE_GP1, GSE_GP2, GSE_GP3, GSE_GP4, GSE_GP5,
};
inline QMap<QString, QList<GUI_FIELD>> gse_sections = {{"GSE", gseDataFields}};

inline QFrame *middlePlaceholder;
inline QFrame *leftPlaceholder;
inline QFrame *rightPlaceholder;

inline QFrame *gseMiddlePlaceholder;

/*inline QString connectedBackgroundImage = ":/images/prop_firehorn_connect.svg";*/
/*inline QString disconnectedBackgroundImage =*/
/*    ":/images/prop_firehorn_disconnect.svg";*/
inline QString connectedBackgroundImage = ":/images/prop_firehorn_V4.svg";
inline QString disconnectedBackgroundImage =
    ":/images/prop_firehorn_V4.svg";


inline void init_views() {
  middlePlaceholder = new ValveControlView(
      valves, labels, connectedBackgroundImage, disconnectedBackgroundImage);
  gseMiddlePlaceholder =
      new ValveControlView(gseValves, gseLabels, ":/images/gse_plumbing.svg",
                           ":/images/gse_plumbing.svg");

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
inline QString backgroundColorCode = "#161A36";
inline QString primary = "#B8C196";
inline QString secondary = "#BDB979";
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
        border-color: #B8C196;
        }
        #child {
            color: #B8C196;
            background: transparent;
        }
        )")
      .arg(col::backgroundColorCode)
      .arg("rgba(30, 35, 69, 225)")
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
