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

struct field_section {
    QString name;
    QList<GUI_FIELD> fields;
};

class TelemetryView : public QFrame {
    Q_OBJECT

public:
    TelemetryView(QList<field_section> section_map, QWidget* parent = nullptr);
    
    
    virtual ~TelemetryView() override = default;

private:
    void addField(GUI_FIELD f, QLayout* layout);
    void createSection(QString title, QList<GUI_FIELD> *fields);
    QVBoxLayout* layout;

    std::unique_ptr<QLabel> altitudeLabel;
};

#endif /* TELEMETRYVIEW_H */
