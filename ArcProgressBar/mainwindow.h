#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include "arcprogressbar.h"

class MainWindow : public QLabel
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    ArcProgressBar *createArcProgressBar1();
    ArcProgressBar *createArcProgressBar2();
    ArcProgressBar *createArcProgressBar3();
};
#endif // MAINWINDOW_H
