/** 
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief QFrame displaying telemetry data
*/

#ifndef TELEMETRYVIEW_H
#define TELEMETRYVIEW_H

#include <memory>

#include <QLabel>
#include <QVBoxLayout>

#include "FieldUtil.h"

class TelemetryView : public QFrame {
    Q_OBJECT

public:
    TelemetryView(QMap<QString, QList<GUI_FIELD>> section_map, QWidget* parent = nullptr);
    
    virtual ~TelemetryView() {};

private:
    void addField(GUI_FIELD f, QLayout* layout);
    void createSection(QString title, QList<GUI_FIELD> *fields);
    QVBoxLayout* layout;

    std::unique_ptr<QLabel> altitudeLabel;
};

#endif /* TELEMETRYVIEW_H */
