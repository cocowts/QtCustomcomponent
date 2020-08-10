#include "mainwindow.h"

#include "bannerlabel.h"

#include <QWidget>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    /* BannerLabel example begin */
    BannerLabel *bannerLabel = new BannerLabel(this);

    bannerLabel->setTextColor(Qt::green);
    bannerLabel->setIndicatorPosition(BannerLabel::IndicatorLeft);
    bannerLabel->setInterval(1000);
    bannerLabel->setHoverStop(true);
    bannerLabel->setFixedSize(800, 400);

    const QList<std::pair<QString, QString>>&& imagepairlst = { std::make_pair("C:/Users/DELL/Desktop/bannerLabel/banner-image/0", "离离原上草！"),
                                                                std::make_pair("C:/Users/DELL/Desktop/bannerLabel/banner-image/1", "一岁一枯荣！"),
                                                                std::make_pair("C:/Users/DELL/Desktop/bannerLabel/banner-image/2", "野火烧不尽！"),
                                                                std::make_pair("C:/Users/DELL/Desktop/bannerLabel/banner-image/3", "春风吹又生！"),
                                                                std::make_pair("C:/Users/DELL/Desktop/bannerLabel/banner-image/5", "Banner 横幅组件！"), };
    bannerLabel->addImage(imagepairlst);

    connect(bannerLabel, &BannerLabel::imageChange, [=](int index, const QString &imagePath, const QString &tipText){
        qDebug() << "imageChange: " << index << " " << imagePath << " " << tipText;
    });

    connect(bannerLabel, &BannerLabel::imageclicked, [=](int index, const QString &imagePath, const QString &tipText){
        qDebug() << "imageclicked: " << index << " " << imagePath << " " << tipText;
    });
    /* BannerLabel example end */

    QHBoxLayout *pLayout = new QHBoxLayout;
    pLayout->addWidget(bannerLabel);
    pLayout->setMargin(0);
    setLayout(pLayout);
}

MainWindow::~MainWindow()
{
}

