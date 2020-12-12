#include "mainwindow.h"

#include <QGridLayout>
#include <QSlider>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QLabel(parent)
{
    QGridLayout *pLayout               = new QGridLayout;
    QSlider *slider                    = new QSlider(this);

    WareProgressBar *pWareProgressBar1 = createWareProgressBar1();
    WareProgressBar *pWareProgressBar2 = createWareProgressBar2();
    WareProgressBar *pWareProgressBar3 = createWareProgressBar3();
    WareProgressBar *pWareProgressBar4 = createWareProgressBar4();

    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 100);

    connect(slider, &QSlider::valueChanged, pWareProgressBar1, &WareProgressBar::setValue);
    connect(slider, &QSlider::valueChanged, pWareProgressBar2, &WareProgressBar::setValue);
    connect(slider, &QSlider::valueChanged, pWareProgressBar3, &WareProgressBar::setValue);
    connect(slider, &QSlider::valueChanged, pWareProgressBar4, &WareProgressBar::setValue);

    pLayout->addWidget(pWareProgressBar1, 0, 0);
    pLayout->addWidget(pWareProgressBar2, 0, 1);
    pLayout->addWidget(pWareProgressBar3, 1, 0);
    pLayout->addWidget(pWareProgressBar4, 1, 1);
    pLayout->addWidget(slider, 2, 0, 1, 2);

    setLayout(pLayout);

    setMinimumSize(500, 500);
    setPixmap(QPixmap(":/image/background.png"));
    setScaledContents(true);
}

WareProgressBar *MainWindow::createWareProgressBar1()
{
    WareProgressBar *ret = new WareProgressBar(this);

    ret->setMinValue(0);                            // 设置最小值
    ret->setMaxValue(100);                          // 设置最大值

    return ret;
}

WareProgressBar *MainWindow::createWareProgressBar2()
{
    WareProgressBar *ret = new WareProgressBar(this);

    ret->setWaveHeight(0);                          // 设置水波高度
    ret->setPieCircleColor(QColor(255,107,107));    // 设置饼行进度条圆颜色
    ret->setwaterColor(QColor(255,107,107));        // 设置水颜色
    ret->setPeiIsForwardOrientation(false);         // 设置饼型进度条圆前进方向

    return ret;
}

WareProgressBar *MainWindow::createWareProgressBar3()
{
    WareProgressBar *ret = new WareProgressBar(this);

    ret->setTextColor(Qt::yellow);                  // 设置文本颜色
    ret->setSuffixText("HZ");                       // 设置后缀文本
    ret->setWaveDensity(2);                         // 设置水波密度
    ret->setPeiCircleIsvisible(false);              // 设置饼型进度圆是否可见
    ret->setwaterColor(QColor(24,189,155));         // 设置水颜色

    return ret;
}

WareProgressBar *MainWindow::createWareProgressBar4()
{
   WareProgressBar *ret = new WareProgressBar(this);

   ret->setPieStartAngle(90);                       // 设置饼型进度条圆起始角度
   ret->setWaterIsvisible(false);                   // 设置水是否可见
   ret->setSuffixText("V");                         // 设置后缀文本

   return ret;
}

MainWindow::~MainWindow()
{
}

