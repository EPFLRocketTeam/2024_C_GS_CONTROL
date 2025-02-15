/** 
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief QFrame displaying telemetry data
*/

#include <memory>

#include <QLabel>
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "MainWindow.h"

#include "FieldUtil.h"
#include "components/DataLabel.h"
#include "../Setup.h"
#include "TelemetryView.h"

TelemetryView::TelemetryView(QWidget* parent) : QFrame(parent) {
    
    // Set up the appearance or behavior as needed
    //setStyleSheet("background-color: lightblue;");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    
    // addField(GUI_FIELD::DISCONNECT_ACTIVE);
    

    QList<GUI_FIELD> fields = {
        GUI_FIELD::HOPPER_N2O_PRESSURE,
        HOPPER_ETH_PRESSURE,
        HOPPER_N2O_VENT,
        HOPPER_ETH_VENT,
        HOPPER_N2O_TEMP,
        HOPPER_ETH_MAIN,
        HOPPER_GNSS_LON,
        HOPPER_GNSS_LAT,
        HOPPER_SAT_NBR,
        HOPPER_GYRO_X,
        HOPPER_GYRO_Y,
        HOPPER_GYRO_Z,
        HOPPER_ACC_X,
        HOPPER_ACC_Y,
        HOPPER_ACC_Z,
        HOPPER_BARO,
        HOPPER_KALMAN_POS_X,
        HOPPER_KALMAN_POS_Y,
        HOPPER_KALMAN_POS_Z,
        HOPPER_KALMAN_YAW,
        HOPPER_KALMAN_PITCH,
        HOPPER_KALMAN_ROLL,
        HOPPER_GIMBAL_X,
        HOPPER_GIMBAL_Y,
        HOPPER_HV_VOLTAGE,
        HOPPER_AV_TEMP,
    };
    createSection("AV State", &fields);
    /**/
    /*fields = {*/
    /*    GUI_FIELD::GNSS_CHOICE,*/
    /*    GUI_FIELD::GNSS_LON, */
    /*    GUI_FIELD::GNSS_LAT,*/
    /*    GUI_FIELD::GNSS_ALT,*/
    /*    GUI_FIELD::GNSS_LON_R, */
    /*    GUI_FIELD::GNSS_LAT_R,*/
    /*    GUI_FIELD::GNSS_ALT_R,*/
    /*    GUI_FIELD::GNSS_VERTICAL_SPEED*/
    /*};*/
    /*createSection("Positioning", &fields);*/
}

void TelemetryView::createSection(QString title, QList<GUI_FIELD> *fields) {
    QLabel* l = new QLabel(title);
    l->setStyleSheet(QString("font-size: 14pt; color: %1;font-weight: 400;background: transparent;").arg(col::primary));
    QWidget *wrapper = new QWidget;
    wrapper->setObjectName("wrapper");
    wrapper->setStyleSheet(col::defaultCardStyle("wrapper"));
    QVBoxLayout* layoutField = new QVBoxLayout(wrapper);
    layoutField->addWidget(l);
    for (int i=0; i<fields->size(); i++) {
        addField(fields->at(i), layoutField);
    }
    wrapper->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    layoutField->setAlignment(Qt::AlignTop);

    layout->addWidget(wrapper);
}

void TelemetryView::addField(GUI_FIELD f, QLayout* layout) {

    QStringList parts = fieldUtil::enumToFieldName(f).toLower().split(" ");
    for (int i=0; i<parts.size(); i++) {
        QString word = parts[i];
        parts[i] = word.at(0).toUpper()+word.mid(1);
        
    }
    QLabel* l = new QLabel(parts.join(" ")+" : ");
    l->setStyleSheet(QString("color: %1;background: transparent;").arg(col::primary));
    QWidget *wrapper = new QWidget;
    wrapper->setStyleSheet("background-color: transparent;");
    QHBoxLayout* layoutField = new QHBoxLayout(wrapper);
    layoutField->addWidget(l);
    layoutField->addWidget(new DataLabel(f));
    layout->addWidget(wrapper);
    
}
