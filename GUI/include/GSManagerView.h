#ifndef GSMANAGERVIEW_H
#define GSMANAGERVIEW_H

#include <QLabel>
#include <QVBoxLayout>

#include "ControlPannelView.h"
#include "Log.h"
#include "TimerView.h"

class GSManagerView : public QFrame {
    Q_OBJECT

public:
    GSManagerView(std::unique_ptr<QWidget> parent = nullptr);
    
    ~GSManagerView() override = default;

private:
    void setupUI(); 
    void setupInfoSection();
    void setupTimersSection();
    std::function<void(const QString)> setAltitude = [this](const QString newText) { 
        // setInfoChildrenStyle(packetNumber, 27, false);
        altitude->setText(newText); 
        };
    void setInfoChildrenStyle(QFrame *child, int fontSize, bool isBold);
    void setupConnections();
    void setupInfoLayout(QLabel *altitudeLabel);
    void setupSerialSection();

    ModuleLog _logger = ModuleLog("GSManagerView", LOG_FILE_PATH);

    QLabel *altitude;
    QVBoxLayout *layout;
    TimerView *timerViewAV;
    TimerView *timerViewGSE;
    QWidget *timersSection;
    QWidget *infoSection;
    QWidget *serialSection;
    QString childrenStyle = QString("background: transparent");

};

#endif // DATAWIDGET_H
