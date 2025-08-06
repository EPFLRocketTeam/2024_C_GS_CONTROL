#include "GSEWindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

GSEWindow::GSEWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUI();
}

void GSEWindow::setupUI()
{
    // Create a central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create a layout
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Add a label
    QLabel *label = new QLabel("This is a new window!", centralWidget);
    layout->addWidget(label);

    // Set window title
    setWindowTitle("New Window");
    
    // Set window size
    resize(300, 200);
}
