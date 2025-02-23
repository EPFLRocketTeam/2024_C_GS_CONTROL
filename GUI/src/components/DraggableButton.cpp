#include "iostream"

#include "components/DraggableButton.h"


DraggableButton::DraggableButton(QWidget *parent) : QPushButton("test button",parent) {
    setIcon(QIcon(":/images/GS-valve-open.svg"));
    parent = parent;
}

void DraggableButton::mouseMoveEvent(QMouseEvent *event) {
    // Custom handling of mouse move event
    // Example: Print mouse position
    int yOffset = -12;
    int xOffset = 45;
    double w = 985.0;
    double h = 847.0;

    if(event->buttons() & Qt::LeftButton)
    {
        this->move(mapToParent(event->pos() - offset));
        std::cout << (pos().x()+xOffset)/w << " " << (pos().y()+yOffset)/h << std::endl;
    }
    // If you need to move the button based on mouse movement, you can adjust its position here
    // For example:
    // this->move(mapToParent(event->pos()));
}

void DraggableButton::mousePressEvent(QMouseEvent *event)
{
    offset = event->pos();
}
