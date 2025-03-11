#include "components/DataLabel.h"
#include "MainWindow.h"
#include "../Setup.h"
#include <QFont>
#include <qnamespace.h>

#include "FieldUtil.h"

DataLabel::DataLabel(const GUI_FIELD field, QWidget *parent) : QLabel(parent), field(field)
{
    setObjectName(QString("dataLabel-%1").arg(field));
    setAlignment(Qt::AlignLeft);
    setMinimumWidth(120);

    
    setStyleSheet(QString("background: transparent; color: %1;").arg(col::primary));

    QFont curFont = font();
    curFont.setPointSize(15);
    curFont.setBold(false);
    setFont(curFont);

    
    setText("-");
    MainWindow::clientManager->subscribe(field, [this](const QString newText){setText(newText);});
    
}
