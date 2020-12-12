#include "mainwindow.h"

#include <QGridLayout>
#include <QSlider>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    QSlider *slder = new QSlider(this);

    GenericDashBoard *db1 = createGenericDashBoard1();
    GenericDashBoard *db2 = createGenericDashBoard2();
    GenericDashBoard *db3 = createGenericDashBoard3();
    GenericDashBoard *db4 = createGenericDashBoard4();

    connect(db1, &GenericDashBoard::valueChanged, [&](int val){qDebug() << val;});      // 打印指针移动时对应的表盘数值
    connect(db1, &GenericDashBoard::finished, [&](){qDebug() << "Pointer reached";});   // 打印指针到达目标数值

    connect(slder, &QSlider::valueChanged, db1, &GenericDashBoard::setValue);           // 链接仪表盘数值更新槽
    connect(slder, &QSlider::valueChanged, db2, &GenericDashBoard::setValue);           // 链接仪表盘数值更新槽
    connect(slder, &QSlider::valueChanged, db3, &GenericDashBoard::setValue);           // 链接仪表盘数值更新槽
    connect(slder, &QSlider::valueChanged, db4, &GenericDashBoard::setValue);           // 链接仪表盘数值更新槽

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

GenericDashBoard *MainWindow::createGenericDashBoard1()
{
    GenericDashBoard *ret = new GenericDashBoard(this);

    ret->setMinValue(0);                                        // 设置仪表盘最小值
    ret->setMaxValue(240);                                      // 设置仪表盘最大值
    ret->setScaleMajor(8);                                      // 设置主刻度数量
    ret->setScaleMinor(10);                                     // 设置小刻度数量
    ret->setStartAngle(45);                                     // 设置仪表盘起始角度
    ret->setEndAngle(315);                                      // 设置仪表盘结束角度
    ret->setScalePercentage(0.85);                              // 设置仪表盘刻度百分比 【非低电量告警特殊颜色等的正常色比例】
    ret->setEasingCurve(QEasingCurve::OutQuad);                 // 设置指针缓和曲线
    ret->setAnimationStepTime(80);                              // 设置每刻度动画持续时间

    return ret;
}

GenericDashBoard *MainWindow::createGenericDashBoard2()
{
    GenericDashBoard *ret = new GenericDashBoard(this);

    ret->setAnimationStepTime(0);                                // 关闭动画
    ret->setStartAngle(0);
    ret->setEndAngle(270);
    ret->setSuffixText("W");

    return ret;
}

GenericDashBoard *MainWindow::createGenericDashBoard3()
{
    GenericDashBoard *ret = new GenericDashBoard(this);

    ret->setEasingCurve(QEasingCurve::InOutCubic);
    ret->setSuffixText("HZ");

    return ret;
}

GenericDashBoard *MainWindow::createGenericDashBoard4()
{
    GenericDashBoard *ret = new GenericDashBoard(this);

    ret->setEasingCurve(QEasingCurve::OutQuint);
    ret->setOuterCircleColor(Qt::yellow);
    ret->setPointerColor(Qt::blue);
    ret->setInnerCircleColor(Qt::green);
    ret->setScaleStartColor(Qt::red);
    ret->setScaleEndColor(Qt::white);
    ret->setTextColor(Qt::black);
    ret->setScaleNumColor(Qt::darkCyan);

    return ret;
}

