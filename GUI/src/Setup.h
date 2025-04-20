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
#include "TelemetryView.h"
#include "GSManagerView.h"
#include "ValveControlView.h"
#include "components/IcarusCommandsView.h"
#include <qboxlayout.h>
#include <qframe.h>
#include <qlist.h>
#include <qmap.h>
#include <qnamespace.h>
#include <qobject.h>
#include <QFile>
#include <QTextStream>
#include <QException>
#include <qpushbutton.h>
#include <stdexcept>
// ----------------------------- Setup Views -----------------------------------
using LeftView = TelemetryView;
using MiddleView = ValveControlView;
using RightView = GSManagerView;


// just set the version of the GUI you want to use to true and the other to false
// /!\ if changing the version shown, you also need to make sure that in commons/CMakeLists.txt line 27 the 
// definition RF_PROTOCOL_FIREHORN or RF_PROTOCOL_ICARUS is added
#define ICARUS_GUI true
#define FIREHORN_GUI false
// CHange the debug level to show the debug messages
#define DEBUG_LOG true


// <---------- DO NOT CHANGE ---------->
#if ICARUS_GUI 
namespace ui_elements {
// Instantiate a QMap with std::string keys and std::vector<std::string> values
    inline QMap<std::string, std::vector<GUI_FIELD>> valvesMap{{"Engine Valves", {GUI_FIELD::HOPPER_N2O_VENT, GUI_FIELD::HOPPER_ETH_VENT, 
        GUI_FIELD::HOPPER_N2O_MAIN, GUI_FIELD::HOPPER_ETH_MAIN}}, 
        {"GSE Valves", {GUI_FIELD::GSE_VENT, GUI_FIELD::GSE_FILLING_N2O}}};

    inline QMap<std::string, std::vector<GUI_FIELD>> pushButtonMap{{"Command", {GUI_CMD_CALIBRATE, GUI_CMD_PRESSURIZE,
                                                        GUI_CMD_ARM, GUI_CMD_LAUNCH, GUI_CMD_ABORT}},
                                                                {"GSE Command", {GUI_CMD_DISCONNECT}}};
    inline QMap<std::string, QMap<std::string, std::vector<GUI_FIELD>>> controlMap{{"ValveControlButton", valvesMap},
                                                                            {"QPushButton", pushButtonMap}};
    inline std::vector<ValveInfo> valves = {
        {GUI_FIELD::GSE_VENT, {0.234569, 0.668}, ValveButton::Orientation::Vertical},
        {GUI_FIELD::HOPPER_N2O_VENT, {0.50665, 0.338}, ValveButton::Orientation::Horizontal},
        {GUI_FIELD::HOPPER_ETH_VENT, {0.785838, 0.338}, ValveButton::Orientation::Horizontal},
        {GUI_FIELD::HOPPER_N2_SOL, {0.645736, 0.144315}, ValveButton::Orientation::Horizontal},
        {GUI_FIELD::HOPPER_N2O_MAIN, {0.585838, 0.638}, ValveButton::Orientation::Vertical},
        {GUI_FIELD::HOPPER_ETH_MAIN, {0.703604, 0.638}, ValveButton::Orientation::Vertical}
    };

    inline std::vector<LabelInfo> labels = {
        {GUI_FIELD::GSE_TANK_PRESSURE, 0.09, 0.25},
        {GUI_FIELD::GSE_TANK_TEMPERATURE, 0.09, 0.315},
        {GUI_FIELD::GSE_FILLING_PRESSURE, 0.138, 0.576},
        {GUI_FIELD::CHAMBER_PRESSURE, 0.609595, 0.178905},
        {GUI_FIELD::HOPPER_N2O_PRESSURE, 0.447, 0.439},
        {GUI_FIELD::HOPPER_N2O_TEMP, 0.447, 0.502},
        {GUI_FIELD::HOPPER_ETH_PRESSURE, 0.893, 0.438}
    };

    inline QList<GUI_FIELD> gps = {
        HOPPER_GNSS_LON,
        HOPPER_GNSS_LAT,
        HOPPER_SAT_NBR,
    };
    inline QList<GUI_FIELD> imu = {
        HOPPER_GYRO_X,
        HOPPER_GYRO_Y,
        HOPPER_GYRO_Z,
        HOPPER_ACC_X,
        HOPPER_ACC_Y,
        HOPPER_ACC_Z,
    };
    inline QList<GUI_FIELD> position = {
        HOPPER_BARO,
        HOPPER_KALMAN_POS_X,
        HOPPER_KALMAN_POS_Y,
        HOPPER_KALMAN_POS_Z,
        HOPPER_KALMAN_YAW,
        HOPPER_KALMAN_PITCH,
        HOPPER_KALMAN_ROLL,
    };
    inline QList<GUI_FIELD> tbd = {
        HOPPER_GIMBAL_X,
        HOPPER_GIMBAL_Y,
        HOPPER_HV_VOLTAGE,
        HOPPER_AV_TEMP,
    };
    inline QMap<QString, QList<GUI_FIELD>> data_sections = {
    {"GPS", gps},
    {"Imu", imu},
    {"Vehicule Spatial Data", position},
    {"TBD", tbd}
    };

    inline QList<GUI_FIELD> gseDataFields = {
        GSE_FILLING_N2O,
        GSE_LOADCELL_1,
        GSE_LOADCELL_2,
        GSE_LOADCELL_3,
        GSE_LOADCELL_4,
    };
    inline QMap<QString, QList<GUI_FIELD>> gse_sections = {
        {"GSE", gseDataFields}
    };


    inline QFrame *middlePlaceholder;
    inline QFrame *leftPlaceholder;
    inline QFrame *rightPlaceholder;
    
    inline QString connectedBackgroundImage = ":/images/prop_icarus_connect.svg";
    inline QString disconnectedBackgroundImage = ":/images/prop_icarus_disconnect.svg";

    inline void init_views() {
        middlePlaceholder = new ValveControlView(valves, labels, connectedBackgroundImage, disconnectedBackgroundImage);
        leftPlaceholder = new TelemetryView(data_sections);
        QVBoxLayout* rightLayout = new QVBoxLayout;
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
}
#endif

#if FIREHORN_GUI
namespace ui_elements {
// Instantiate a QMap with std::string keys and std::vector<std::string> values
    inline QMap<std::string, std::vector<GUI_FIELD>> valvesMap{{"Engine Valves", 
        {
        GUI_FIELD::MAIN_LOX,
        GUI_FIELD::MAIN_FUEL,
        GUI_FIELD::VENT_LOX,
        GUI_FIELD::VENT_FUEL,
        GUI_FIELD::IGNITER_LOX,
        GUI_FIELD::IGNITER_FUEL,
        }}, 
        {"GSE Valves", {GUI_FIELD::GSE_VENT, GUI_FIELD::GSE_FILLING_N2O}}};

    inline QMap<std::string, std::vector<GUI_FIELD>> pushButtonMap{{"Command", {GUI_CMD_CALIBRATE, GUI_CMD_RECOVER,GUI_CMD_PRESSURIZE,
                                                        GUI_CMD_ARM, GUI_CMD_IGNITION, GUI_CMD_ABORT, GUI_CMD_MANUAL_DEPLOY}},
                                                                {"GSE Command", {GUI_CMD_DISCONNECT}}};
    inline QMap<std::string, QMap<std::string, std::vector<GUI_FIELD>>> controlMap{{"ValveControlButton", valvesMap},
                                                                            {"QPushButton", pushButtonMap}};
    inline std::vector<ValveInfo> valves = {
        {GUI_FIELD::GSE_VENT, {0.24, 0.668}, ValveButton::Orientation::Vertical},
        {GUI_FIELD::MAIN_LOX, {0.567, 0.74}, ValveButton::Orientation::Horizontal},
        {GUI_FIELD::MAIN_FUEL, {0.725, 0.68}, ValveButton::Orientation::Horizontal},
        // {GUI_FIELD::PRESSURI, {0.645736, 0.2}, ValveButton::Orientation::Horizontal},
        {GUI_FIELD::VENT_FUEL, {0.785838, 0.338}, ValveButton::Orientation::Horizontal},
        {GUI_FIELD::VENT_LOX, {0.51, 0.338}, ValveButton::Orientation::Vertical},
        {GUI_FIELD::IGNITER_LOX, {0.6, 0.611}, ValveButton::Orientation::Vertical},
        {GUI_FIELD::IGNITER_FUEL, {0.69, 0.611}, ValveButton::Orientation::Vertical}
    };

    inline std::vector<LabelInfo> labels = {
        {GUI_FIELD::GSE_TANK_PRESSURE, 0.09, 0.25},
        {GUI_FIELD::GSE_TANK_TEMPERATURE, 0.09, 0.315},
        {GUI_FIELD::GSE_FILLING_PRESSURE, 0.138, 0.576},

        {GUI_FIELD::N2_PRESSURE, 0.55, 0.19},
        {GUI_FIELD::N2_TEMP, 0.8, 0.19},

        {GUI_FIELD::LOX_PRESSURE, 0.447, 0.439},
        {GUI_FIELD::LOX_TEMP, 0.447, 0.505},
        {GUI_FIELD::LOX_LEVEL, 0.642, 0.502},

        {GUI_FIELD::FUEL_PRESSURE, 0.893, 0.438},
        {GUI_FIELD::FUEL_LEVEL, 0.761, 0.502},

        {GUI_FIELD::IGNITER_PRESSURE, 0.883, 0.7},
        {GUI_FIELD::FUEL_INJ_PRESSURE, 0.883, 0.781},

        {GUI_FIELD::LOX_INJ_PRESSURE, 0.455, 0.78},
        {GUI_FIELD::LOX_INJ_TEMP, 0.455, 0.86},

        {GUI_FIELD::CHAMBER_PRESSURE, 0.883, 0.86},

    };

    inline QList<GUI_FIELD> gps = {
        GNSS_LON,
        GNSS_LAT,
        GNSS_ALT,
        GNSS_VERTICAL_SPEED,
    };
    
    inline QList<GUI_FIELD> tbd = {
        AV_STATE,
        LPB_VOLTAGE,
        HPB_VOLTAGE,
        AV_FC_TEMP,
        AMBIENT_TEMP,
        CAM_REC,
    };
    inline QMap<QString, QList<GUI_FIELD>> data_sections = {
    {"GPS", gps},
    {"TBD", tbd}
    };

    inline QList<GUI_FIELD> gseDataFields = {
        GSE_FILLING_N2O,
        GSE_LOADCELL_1,
    };
    inline QMap<QString, QList<GUI_FIELD>> gse_sections = {
        {"GSE", gseDataFields}
    };


    inline QFrame *middlePlaceholder;
    inline QFrame *leftPlaceholder;
    inline QFrame *rightPlaceholder;
    
    inline QString connectedBackgroundImage = ":/images/prop_firehorn_connect.svg";
    inline QString disconnectedBackgroundImage = ":/images/prop_firehorn_disconnect.svg";

    inline void init_views() {
        middlePlaceholder = new ValveControlView(valves, labels, connectedBackgroundImage, disconnectedBackgroundImage);
        leftPlaceholder = new TelemetryView(data_sections);
        QVBoxLayout* rightLayout = new QVBoxLayout;
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
}
#endif


// ----------------------------- MainWindow setup ------------------------------
namespace mws
{
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
    inline void loadKeyFromFile(const QString& filePath) {
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            throw std::runtime_error("Failed to open the key file.");
        }

        QTextStream in(&file);
        key = in.readLine().trimmed(); // Read the first line and trim whitespace
        file.close();

        if (key.isEmpty()) {
            throw std::runtime_error("The key file is empty or invalid.");
        }
    }}

namespace network
{
    const QString serverIP = "127.0.0.1";
    const int serverPort = 12345;
}
// ----------------------------- Colour ----------------------------------------
namespace col
{
    inline QString backgroundColorCode = "#161A36";
    inline QString primary = "#B8C196";
    inline QString secondary = "#BDB979";
    inline QString accent = "#F5251A";
    inline QString complementary = "#457069";
    inline QString complementaryLighter = "#538078";

    inline QString background() { return "background-color: " + backgroundColorCode + ";"; }
    inline QString defaultCardStyle(QString id) { return QString(R"(
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

    inline QString labelStyle = QString("font-size: 14pt; color: %1;font-weight: 400;background: transparent;").arg(col::primary);
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
