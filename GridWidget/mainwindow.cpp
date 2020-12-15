#include "mainwindow.h"

#include <QLabel>
#include <QStringLiteral>
#include <QVBoxLayout>
#include <QPixmap>
#include <QMovie>

#include "gridwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    //============================================================
    GridWidget *pGridWidget = new GridWidget(this);

    pGridWidget->resize(800, 600);                             // 设置父组件大小
    pGridWidget->setSpacing(QSize(40, 40));                    // 设置子组件行、列间距
    pGridWidget->setMargins(QMargins(15, 20, 15, 15));         // 设置子组件与父组件的边距
    pGridWidget->setSize(QSize(60, 60));                       // 设置子组件大小
    pGridWidget->setAnimationEnabled(true);                    // 开启鼠标点击动画

    QMovie *movie = new QMovie(":/image/src/background.gif");  // 设置父组件背景（支持静态图和动态图）
    movie->start();
    pGridWidget->setMovie(movie);

    for (int i=0; i<10; ++i)
    {
        QLabel *label = new QLabel(pGridWidget);
        label->setPixmap(QPixmap(QString(":/image/src/%1.png").arg(i, 2, 10, QLatin1Char('0'))));
        label->setScaledContents(true);

        pGridWidget->append(label);
    }

    QLabel *label = new QLabel(pGridWidget);
    label->setFixedSize(60, 60);
    label->setScaledContents(true);

    QMovie *movie1 = new QMovie(":/image/src/icon.gif");
    movie1->start();
    label->setMovie(movie1);

    pGridWidget->insert(0, label);

    //============================================================

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(pGridWidget);
    setLayout(pLayout);
}

MainWindow::~MainWindow()
{ }

