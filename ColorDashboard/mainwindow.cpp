#include "mainwindow.h"

#include <QVBoxLayout>
#include <QSlider>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    QSlider *slder = new QSlider(this);

    ColorDashboard *db1 = ColorDashboard1();
    ColorDashboard *db2 = ColorDashboard2();
    ColorDashboard *db3 = ColorDashboard3();
    ColorDashboard *db4 = ColorDashboard4();

    connect(db1, &ColorDashboard::valueChanged, [&](int val){qDebug() << val;});      // 打印指针移动时对应的表盘数值
    connect(db1, &ColorDashboard::finished, [&](){qDebug() << "Pointer reached";});   // 打印指针到达目标数值

    connect(slder, &QSlider::valueChanged, db1, &ColorDashboard::setValue);           // 链接仪表盘数值更新槽
    connect(slder, &QSlider::valueChanged, db2, &ColorDashboard::setValue);           // 链接仪表盘数值更新槽
    connect(slder, &QSlider::valueChanged, db3, &ColorDashboard::setValue);           // 链接仪表盘数值更新槽
    connect(slder, &QSlider::valueChanged, db4, &ColorDashboard::setValue);           // 链接仪表盘数值更新槽

    slder->setOrientation(Qt::Horizontal);
    slder->setRange(db1->getMinValue(), db1->getMaxValue());
    slder->setValue(db1->getValue());

    QGridLayout *pLayout = new QGridLayout;
    pLayout->addWidget(db1, 0, 0);
    pLayout->addWidget(db2, 0, 1);
    pLayout->addWidget(db3, 1, 0);
    pLayout->addWidget(db4, 1, 1);
    pLayout->addWidget(slder, 2, 0, 1, 2);

    setLayout(pLayout);

    setMinimumSize(400, 400);
}

MainWindow::~MainWindow()
{
}

ColorDashboard *MainWindow::ColorDashboard1()
{
    ColorDashboard *ret = new ColorDashboard(this);

    ret->setMinValue(0);                                                             // 设置仪表盘最小值
    ret->setMaxValue(240);                                                           // 设置仪表盘最大值

    ret->setScaleMajor(8);                                                           // 设置主刻度数量
    ret->setScaleMinor(10);                                                          // 设置小刻度数量

    ret->setStartAngle(45);                                                          // 设置仪表盘起始角度
    ret->setEndAngle(315);                                                           // 设置仪表盘结束角度

    ret->setEasingCurve(QEasingCurve::OutQuad);                                      // 设置指针缓和曲线
    ret->setAnimationStepTime(80);                                                   // 设置每刻度动画持续时间

    ret->setPointerStyle(ColorDashboard::PointerStyle_IndicatorR);                   // 设置指示器样式
    ret->setPieStyle(ColorDashboard::PieStyle_Three);                                // 设置饼圆样式

    return ret;
}

ColorDashboard *MainWindow::ColorDashboard2()
{
    ColorDashboard *ret = new ColorDashboard(this);

    ret->setStartAngle(0);
    ret->setEndAngle(270);
    ret->setAnimationStepTime(0);
    ret->setPointerStyle(ColorDashboard::PointerStyle_Ball);

    return ret;
}

ColorDashboard *MainWindow::ColorDashboard3()
{
    ColorDashboard *ret = new ColorDashboard(this);

    ret->setPointerStyle(ColorDashboard::PointerStyle_Triangle);
    ret->setPieStyle(ColorDashboard::PieStyle_Current);

    return ret;
}

ColorDashboard *MainWindow::ColorDashboard4()
{
    ColorDashboard *ret = new ColorDashboard(this);

    ret->setPointerStyle(ColorDashboard::PointerStyle_Indicator);
    ret->setPieStyle(ColorDashboard::PieStyle_Three);
    ret->setOverlayVisible(false);

    return ret;
}


