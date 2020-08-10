#include "mainwindow.h"

#include "simplemessagebox.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << SimpleMessageBox::infomationMessageBox("这是一个测试，今天心情好不啦,啦啦啦，嘿嘿嘿");
    qDebug() << SimpleMessageBox::errorMessageBox("祝你平安喜乐，万事胜意");
    qDebug() << SimpleMessageBox::questionMessageBox("bug修复了吗？");
}

MainWindow::~MainWindow()
{
}

