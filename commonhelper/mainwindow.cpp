#include "mainwindow.h"

#include <QPushButton>
#include <QVBoxLayout>

#include "commonhelper.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    QPushButton *pBtn = new QPushButton("重启", this);

    connect(pBtn, &QPushButton::clicked, [=](){
        CommonHelper::reboot();
    });

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(pBtn);
    setLayout(pLayout);
}

MainWindow::~MainWindow()
{
}

