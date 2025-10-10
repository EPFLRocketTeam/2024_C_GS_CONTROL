

#include <QLabel>
#include <QtGlobal>

#include "FieldUtil.h"

/**
 * @brief The DataLabel class is a class that thisplay a QLbael as well as it connects to the field sepcified in constructor
 */
class DataLabel : public QLabel
{
    Q_OBJECT

public:
    DataLabel(const GUI_FIELD field, QWidget *parent = nullptr);
    virtual ~DataLabel() {};
private:
    GUI_FIELD field;
    bool m_hasPacketFrequencyBaseline{false};
    qint64 m_lastPacketFrequencyTimestampMs{0};
    int m_lastPacketNumber{0};

};
