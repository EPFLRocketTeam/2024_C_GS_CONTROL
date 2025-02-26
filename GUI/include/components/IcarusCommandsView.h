/** 
    @authors M.Rochat
    @date 26-02-2025
    @brief QFrame displaying Icarus Spcific Commands
*/

#ifndef ICARUS_COMMAND_PANNEL_H
#define ICARUS_COMMAND_PANNEL_H

#include <memory>

#include <QLabel>
#include <QVBoxLayout>

#include "FieldUtil.h"

class IcarusCommandsView : public QFrame {
    Q_OBJECT

public:
    IcarusCommandsView(QWidget* parent = nullptr);
    
    virtual ~IcarusCommandsView() {};

private:

    QVBoxLayout* layout;
    std::unique_ptr<QLabel> altitudeLabel;
};

#endif /* ICARUS_COMMAND_PANNEL_H */
