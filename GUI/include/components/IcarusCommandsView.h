#ifndef ICARUSCOMMANDSVIEW_H
#define ICARUSCOMMANDSVIEW_H

#include "Log.h"
#include <QWidget>
#include <qframe.h>

#include "FileLocation.h"

class QLineEdit;
class QPushButton;

class IcarusCommandsView : public QFrame {
    Q_OBJECT
public:
    IcarusCommandsView(QWidget *parent = nullptr);
    ~IcarusCommandsView();

private slots:
    void onButtonClicked();

private:
    QLayout* setupCurrentValueLabels();
    QLayout* setupGimbalSection();
    QLayout* setupIDConfigSection();

    QLineEdit* m_gimbalXLineEdit;
    QLineEdit* m_gimbalYLineEdit;
    QLineEdit* m_configLineEdit;
    QPushButton* m_commandButton;
    QPushButton* m_sendConfig;

    ModuleLog _logger = ModuleLog("IcarusCommandsView", LOG_FILE_PATH);
};

#endif // ICARUSCOMMANDSVIEW_H

