/*
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief QFrame displaying global data
*/

#ifndef CONTROLPANNELVIEW_H
#define CONTROLPANNELVIEW_H

#include <memory>

#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>

#include "FieldUtil.h"
#include "Log.h"
#include "FileLocation.h"

class ControlPannelView : public QFrame {
    Q_OBJECT

public:
    ControlPannelView(QWidget *parent = nullptr, QMap<std::string, QList<std::vector<GUI_FIELD>>> *controls = nullptr);
    void resizeWidget();
    virtual ~ControlPannelView() override = default;


protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void expandClicked();
    std::unique_ptr<QLabel> displayText;
    int getHeightPos();
    void setupExpandButton();
    void setupContainerWidget();
    void createValveControlButtons(QGridLayout *gridLayout, const std::vector<GUI_FIELD> &strings, int maxColumns);
    void createValveLayouts(QHBoxLayout *mainLayout, QList<std::vector<GUI_FIELD>> *valves);
    void createPushButtonLayouts(QHBoxLayout *mainLayout, QList<std::vector<GUI_FIELD>> *buttons);
    QMessageBox::StandardButton showConfirmDialog(QWidget *parent, 
                                              const QString &title, 
                                              const QString &text);
    
    ModuleLog _logger = ModuleLog("ControlPannelView", LOG_FILE_PATH);
    QPixmap buttonPixMap;
    bool toggled;
    QWidget *controlContainerWidget;
    QPushButton *expandButton;
};

#endif // CONTROLPANNELVIEW_H

