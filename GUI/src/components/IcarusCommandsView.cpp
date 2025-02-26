#include "components/IcarusCommandsView.h"
#include <qboxlayout.h>
#include <qframe.h>
#include <qlabel.h>
#include <qwidget.h>
#include "Setup.h"

IcarusCommandsView::IcarusCommandsView(QWidget *parent): QFrame(parent) {
    setObjectName("IcarusCommandsView");
    setStyleSheet(col::defaultCardStyle("IcarusCommandsView"));
    QLabel *l = new QLabel("Icarus Commands");
    l->setStyleSheet(QString("font-size: 14pt; color: %1;font-weight: 400;background: transparent;").arg(col::primary));
    layout = new QVBoxLayout;
    layout->addWidget(l);
    setLayout(layout);
}
