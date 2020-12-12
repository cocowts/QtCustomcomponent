#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "colordashboard.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    ColorDashboard *ColorDashboard1();
    ColorDashboard *ColorDashboard2();
    ColorDashboard *ColorDashboard3();
    ColorDashboard *ColorDashboard4();
};
#endif // MAINWINDOW_H
