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
#include <qframe.h>
#include <qobject.h>
#include <QFile>
#include <QTextStream>
#include <QException>
#include <stdexcept>
// ----------------------------- Setup Views -----------------------------------
using LeftView = TelemetryView;
using MiddleView = ValveControlView;
using RightView = GSManagerView;

namespace ui_elements {
// Instantiate a QMap with std::string keys and std::vector<std::string> values
    inline QMap<std::string, std::vector<GUI_FIELD>> valvesMap{{"Engine Valves", {GUI_FIELD::HOPPER_N2O_VENT, GUI_FIELD::HOPPER_ETH_VENT, 
        GUI_FIELD::HOPPER_N2O_MAIN, GUI_FIELD::HOPPER_ETH_MAIN}}, 
        {"GSE Valves", {GUI_FIELD::GSE_VENT}}};

    inline QMap<std::string, std::vector<GUI_FIELD>> pushButtonMap{{"Command", {GUI_CMD_CALIBRATE, GUI_CMD_PRESSURIZE,
                                                        GUI_CMD_ARM, GUI_CMD_LAUNCH, GUI_CMD_ABORT}}};
    inline QMap<std::string, QMap<std::string, std::vector<GUI_FIELD>>> controlMap{{"ValveControlButton", valvesMap},
                                                                            {"QPushButton", pushButtonMap}};
    inline QFrame *middlePlaceholder;
    inline QFrame *leftPlaceholder;
    inline QFrame *rightPlaceholder;
    inline void init_views() {
        middlePlaceholder = new ValveControlView();
        leftPlaceholder = new TelemetryView();
        rightPlaceholder = new GSManagerView();
    }
}



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
                                                      .arg(id); }
} // namespace col

#endif /* SETUP_H */
