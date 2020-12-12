#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "genericdashboard.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    GenericDashBoard *createGenericDashBoard1();
    GenericDashBoard *createGenericDashBoard2();
    GenericDashBoard *createGenericDashBoard3();
    GenericDashBoard *createGenericDashBoard4();
};
#endif // MAINWINDOW_H
