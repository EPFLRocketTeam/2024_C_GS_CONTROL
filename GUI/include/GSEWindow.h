#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class GSEWindow : public QMainWindow
{
    Q_OBJECT

public:
    GSEWindow(QWidget *parent = nullptr);
    virtual ~GSEWindow() {};

private:
    void setupUI();
};

#endif // NEWWINDOW_H
