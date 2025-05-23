#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include <QWidget>
#include <QMovie>
#include <QTimer>

#include "FieldUtil.h"
#include "Log.h"
#include "FileLocation.h"

enum State { Open, Close, Unknown };


class ToggleButton : public QWidget
{   
    Q_PROPERTY(qreal offset READ offset WRITE setOffset)
    Q_OBJECT
public:
    explicit ToggleButton(GUI_FIELD fieldSensitivity,QWidget *parent = nullptr);

    QSize sizeHint() const override;

    bool isChecked() const;
    void setChecked(bool checked);

    void animateToggle(int duration);

    void setUnknown();

    void updateState(const QString& res);
    void toggleCallback();


signals:
    void toggled(bool checked);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    QTimer *m_timer;
    State m_state;
    bool m_checked;
    qreal m_offset;
    qreal offset() const;
    void setOffset(qreal offsetValue);
    void updateAnim();
    QMovie *m_loadingMovie;
    GUI_FIELD m_fieldSensitivity;
    ModuleLog _logger = ModuleLog("ValveToggle", LOG_FILE_PATH);
};

#endif // TOGGLEBUTTON_H
