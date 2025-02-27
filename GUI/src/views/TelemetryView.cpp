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
#include <string>

#include "MainWindow.h"

#include "FieldUtil.h"
#include "components/DataLabel.h"
#include "../Setup.h"
#include "TelemetryView.h"

TelemetryView::TelemetryView(QMap<QString, QList<GUI_FIELD>> section_map, QWidget* parent) : QFrame(parent) {
    
    // Set up the appearance or behavior as needed
    //setStyleSheet("background-color: lightblue;");
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    layout->setContentsMargins(0, 0, 0, 0);
    for (auto key: section_map.keys()) {
        createSection(key, &section_map[key]);
    }
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
