#include "iostream"

#include "components/DraggableButton.h"


DraggableButton::DraggableButton(QWidget *parent) : QPushButton("test button",parent) {
    setIcon(QIcon(":/images/GS-valve-open.svg"));
    parent = parent;
    width = 1737;
    height = 847;

}

/*DraggableButton::DraggableButton(QWidget *parent, int pWidth, int pHeight) : QPushButton("test button",parent) {*/
/*    setIcon(QIcon(":/images/GS-valve-open.svg"));*/
/*    parent = parent;*/
/*    width = pWidth;*/
/*    height = pHeight;*/
/*}*/

void DraggableButton::mouseMoveEvent(QMouseEvent *event) {
    // Custom handling of mouse move event
    // Example: Print mouse position
    int yOffset = 0;
    int xOffset = -168;
    double w = width;
    double h = height;

    if(event->buttons() & Qt::LeftButton)
    {
        this->move(mapToParent(event->pos() - offset));
        std::cout << "x =" <<  (pos().x()) << ", " << "y =" << (pos().y()) << std::endl;
        std::cout << (pos().x()+xOffset)/w << ", " << (pos().y()+yOffset)/h << std::endl;
    }
    // If you need to move the button based on mouse movement, you can adjust its position here
    // For example:
    // this->move(mapToParent(event->pos()));
}

void DraggableButton::mousePressEvent(QMouseEvent *event)
{
    offset = event->pos();
}
