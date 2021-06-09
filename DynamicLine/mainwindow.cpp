#include "mainwindow.h"

#include <QVBoxLayout>
#include <QTimer>

#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    m_pLines = new DynamicLine(this);

    m_pLines->setChartTitle(QStringLiteral("历史温度曲线"));

    m_pLines->setTimeAxisXSpanSecs(10);
    m_pLines->setTimeAxisXFormat(QStringLiteral("HH:mm"));
    m_pLines->setTimeAxisXTickCount(11);

    m_pLines->setAxisYRange(0, 150);
    m_pLines->setpAxisYTickCount(6);
    m_pLines->setAxisYLabelFormat(QString::fromLatin1("%d℃"));

    QPen pen;
    pen.setWidth(2);
    pen.setColor(QColor(255,255,255));
    m_pLines->addSplineSeries(QStringLiteral("端点1"), pen, true);
    pen.setColor(QColor(112,197,166));
    m_pLines->addSplineSeries(QStringLiteral("端点2"), pen, true);
    pen.setColor(QColor(9,171,246));
    m_pLines->addSplineSeries(QStringLiteral("端点3"), pen, true);
    pen.setColor(QColor(252,184,199));
    m_pLines->addSplineSeries(QStringLiteral("端点4"), pen, true);
    pen.setColor(QColor(254,69,100));
    m_pLines->addSplineSeries(QStringLiteral("端点5"), pen, true);
    m_pLines->start(1000);

    QVBoxLayout *pLayout = new QVBoxLayout;
    pLayout->addWidget(m_pLines, 9);
    pLayout->setSpacing(0);
    pLayout->setMargin(0);
    setLayout(pLayout);

    setWindowTitle(QStringLiteral("动态曲线 Demo"));
    resize(700, 350);

    QTimer *pTimer = new QTimer(this);
    connect(pTimer, &QTimer::timeout, this, &MainWindow::update);
    pTimer->start(1000);
}

MainWindow::~MainWindow()
{
}

void MainWindow::update()
{
    m_pLines->setSeriesValues(0, QRandomGenerator::global()->bounded(120, 140));
    m_pLines->setSeriesValues(1, QRandomGenerator::global()->bounded(90, 120));
    m_pLines->setSeriesValues(2, QRandomGenerator::global()->bounded(75, 90));
    m_pLines->setSeriesValues(3, QRandomGenerator::global()->bounded(40, 70));
    m_pLines->setSeriesValues(4, QRandomGenerator::global()->bounded(10, 30));
}

