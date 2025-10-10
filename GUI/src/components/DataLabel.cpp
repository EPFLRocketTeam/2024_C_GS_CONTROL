#include "components/DataLabel.h"
#include "MainWindow.h"
#include "../Setup.h"
#include <QDateTime>
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

    if (field == GUI_FIELD::AV_PACKET_FREQ) {
        MainWindow::clientManager->subscribe(GUI_FIELD::PACKET_NBR, [this](const QString &packetText) {
            bool ok = false;
            const int packetNumber = packetText.toInt(&ok);
            if (!ok) {
                setText("-");
                m_hasPacketFrequencyBaseline = false;
                return;
            }

            const qint64 nowMs = QDateTime::currentMSecsSinceEpoch();
            if (m_hasPacketFrequencyBaseline) {
                const qint64 deltaMs = nowMs - m_lastPacketFrequencyTimestampMs;
                const int deltaPackets = packetNumber - m_lastPacketNumber;
                if (deltaMs > 0 && deltaPackets > 0) {
                    const double frequencyHz = static_cast<double>(deltaPackets) * 1000.0 /
                                               static_cast<double>(deltaMs);
                    setText(QString::number(frequencyHz, 'f', 2));
                }
            }

            m_lastPacketNumber = packetNumber;
            m_lastPacketFrequencyTimestampMs = nowMs;
            m_hasPacketFrequencyBaseline = true;
        });
        return;
    }

    MainWindow::clientManager->subscribe(field, [this](const QString newText){setText(newText);});
    
}
