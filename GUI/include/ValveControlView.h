/**
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief QFrame displaying plumbing diagram and valve control
**/

#ifndef VALVECONTROLVIEW_H
#define VALVECONTROLVIEW_H

#include <QFrame>
#include <memory>

#include <QMap>
#include <QtSvg/QSvgRenderer>
#include <qsvgrenderer.h>

#include "FieldUtil.h"
#include "Log.h"
#include "components/ValveButton.h"
#include "FileLocation.h"

struct Position {
        float x;
        float y;
    };

typedef struct  {
    GUI_FIELD f;
    Position p;
    ValveButton::Orientation o;
} ValveInfo;

typedef struct  {
    GUI_FIELD f;
    Position p;
} LabelInfo;

class ValveControlView : public QFrame {
    Q_OBJECT

public:
    ValveControlView(std::vector<ValveInfo> valves, std::vector<LabelInfo> labels, 
                     QString connectedBg, QString disconnectedBg, QWidget *parent = nullptr);
    virtual ~ValveControlView() {}
    
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void setSvgBackground(const QString& filePath);
    void addButtonIcon(GUI_FIELD field,float x, float y, ValveButton::Orientation orientation = ValveButton::Orientation::Vertical);
    void addDataLabel(const GUI_FIELD field, float x, float y);
    void placeValves();
    void placeDataLabels();
    void placeCommandButtons();
    void addComponent(QWidget* component,  float x, float y);
    void addCommandButton(const QString& label, float x, float y);

    QString connectedBgPath;
    QString disconnectedBgPath;
    std::vector<ValveInfo> _valves;
    std::vector<LabelInfo> _labels;
    ModuleLog _logger = ModuleLog("ValveControlView", LOG_FILE_PATH);
    std::unique_ptr<QSvgRenderer> svgRenderer;
    QMap<QWidget*, Position> componentsMap;
};

#endif // VALVECONTROLVIEW_H
