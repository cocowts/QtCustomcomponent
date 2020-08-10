#include "mainwindow.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>

#include "iconhelper.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    QListWidget *pListWidget = new QListWidget(this);

    pListWidget->setMovement(QListView::Snap);
    pListWidget->setViewMode(QListView::IconMode);
    pListWidget->setResizeMode(QListView::Adjust);
    pListWidget->setSpacing(10);

    for (int index=0; index<500; ++index)
    {
        QListWidgetItem *item = new QListWidgetItem(pListWidget);
        IconHelper::setIcon(item, QChar(index + 0xf031), 16);   // 注意这里
        pListWidget->addItem(item);
    }

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(pListWidget);
    setLayout(pLayout);
}

MainWindow::~MainWindow()
{
}

