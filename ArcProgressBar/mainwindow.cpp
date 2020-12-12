#include "mainwindow.h"

#include <QHBoxLayout>
#include <QSlider>

MainWindow::MainWindow(QWidget *parent)
    : QLabel(parent)
{
    QSlider *pSlider = new QSlider(this);
    ArcProgressBar *bar1 = createArcProgressBar1();
    ArcProgressBar *bar2 = createArcProgressBar2();
    ArcProgressBar *bar3 = createArcProgressBar3();

    pSlider->setRange(0, 100);

    connect(pSlider, &QSlider::valueChanged, bar1, &ArcProgressBar::setValue);
    connect(pSlider, &QSlider::valueChanged, bar2, &ArcProgressBar::setValue);
    connect(pSlider, &QSlider::valueChanged, bar3, &ArcProgressBar::setValue);

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(bar1);
    pLayout->addWidget(bar2);
    pLayout->addWidget(bar3);
    pLayout->addSpacing(20);
    pLayout->addWidget(pSlider);

    setLayout(pLayout);

    setMinimumSize(1000, 500);
    setPixmap(QPixmap(":/image/background.png"));
    setScaledContents(true);
}

MainWindow::~MainWindow()
{
}

ArcProgressBar *MainWindow::createArcProgressBar1()
{
    ArcProgressBar *ret = new ArcProgressBar(this);

    ret->setArcColor(QColor(247,74,99));
    ret->setTextColor(QColor(247,74,99));
    ret->setTitleColor(Qt::black);
    ret->setAnimationStepTime(10);
    ret->setTitle("完成率");

    return ret;
}

ArcProgressBar *MainWindow::createArcProgressBar2()
{
    ArcProgressBar *ret = new ArcProgressBar(this);

    ret->setStartAngle(145);
    ret->setEndAngle(405);
    ret->setAnimationStepTime(0);
    ret->setTitle("及格率");

    return ret;
}

ArcProgressBar *MainWindow::createArcProgressBar3()
{
    ArcProgressBar *ret = new ArcProgressBar(this);

    ret->setStartAngle(0);
    ret->setEndAngle(360);

    return ret;
}

