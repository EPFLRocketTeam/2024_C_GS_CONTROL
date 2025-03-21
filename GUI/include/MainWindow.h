/**
    @authors M.Rochat & E.Dutruy (Co-TL GS 23-24)
    @date 06-02-2024
    @brief Main window of the application
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFrame>
#include <QPushButton>

#include "components/DraggableButton.h"
#include "ClientManager.h"
#include "ControlPannelView.h"
#include "RequestBuilder.h"
#include <Log.h>


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() {};
    inline static std::unique_ptr<ClientManager> clientManager;

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void replacePannelButton();
    void buttonGrabbed(QEvent *event);

    ModuleLog _logger = ModuleLog("MainWindow", LOG_FILE_PATH);
    DraggableButton *pannelButton; 
    ControlPannelView *pannelSection;
    QHBoxLayout* createSectionsLayout();
};




#endif /* MAINWINDOW_H */
