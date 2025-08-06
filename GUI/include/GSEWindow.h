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
    explicit GSEWindow(QWidget *parent = nullptr);

private:
    void setupUI();
};

#endif // NEWWINDOW_H
