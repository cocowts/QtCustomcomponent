#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include "wareprogressbar.h"

class MainWindow : public QLabel
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    WareProgressBar *createWareProgressBar1();
    WareProgressBar *createWareProgressBar2();
    WareProgressBar *createWareProgressBar3();
    WareProgressBar *createWareProgressBar4();
private:

};
#endif // MAINWINDOW_H
