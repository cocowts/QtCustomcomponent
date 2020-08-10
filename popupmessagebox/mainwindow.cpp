#include "mainwindow.h"

#include <QDebug>

#include "popupmessagebox.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    // 抖动
    QPointer<PopupMessageBox> p = PopupMessageBox::createInstance();

    p->setTitle("这是一个测试工程提示");
    p->setTitleIcon(QPixmap(":/popumessagebox/popupmessagebox/home.png"));
    p->setButtonVisible(false);
    p->setText("作业写完了吗！！！");
    p->setTextColor(Qt::red);
    p->setButtonVisible(true);

    connect(p, &PopupMessageBox::quit, [=](){
        qDebug() << "quit";
    });
    connect(p, &PopupMessageBox::accepted, [=](){
        qDebug() << "accepted";
    });
    connect(p, &PopupMessageBox::rejected, [=](){
        qDebug() << "rejected";
    });

    p->start(PopupMessageBox::Animation_Shark, -1, 800);
}

MainWindow::~MainWindow()
{
}

