#ifndef DRAGGABLEBUTTON_H
#define DRAGGABLEBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <qwidget.h>

class DraggableButton : public QPushButton {
    Q_OBJECT
public:
    DraggableButton(QWidget *parent = nullptr);

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    
private:    
    QPoint offset;
    QWidget* parent;
    int width;
    int height;
};

#endif // DRAGGABLEBUTTON_H
