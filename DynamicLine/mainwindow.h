#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "dynamicline.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void update();

private:
    DynamicLine *m_pLines = nullptr;
};
#endif // MAINWINDOW_H
