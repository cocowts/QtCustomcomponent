#include "sliderWidget.h"

#include <QApplication>
#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <QMovie>
#include <QIcon>

// 简单随意的测试布局
QWidget *createPage1()
{
    QWidget *ret = new QWidget;
    QLabel *apps[11];

    ret->setStyleSheet("background-image:url(:/resource/imgs/background3.png);");

    for (int i=0; i<6; ++i)
    {
        apps[i] = new QLabel(ret);
        apps[i]->setScaledContents(true);
        apps[i]->setFixedSize(100, 100);
        apps[i]->setScaledContents(true);
        apps[i]->setPixmap(QString(":/resource/imgs/(%1).jpg").arg(i));
        apps[i]->move(i * 130 + 25, 25);
        apps[i]->setToolTip("点我");
    }

    for (int i=6; i<11; ++i)
    {
        apps[i] = new QLabel(ret);
        apps[i]->setScaledContents(true);
        apps[i]->setFixedSize(100, 100);
        apps[i]->setScaledContents(true);
        apps[i]->setPixmap(QString(":/resource/imgs/(%1).jpg").arg(i));
        apps[i]->move((i-6) * 130 + 25, 150);
        apps[i]->setToolTip("点我");
    }

    return ret;
}

// 简单随意的测试布局
QWidget *createPage2()
{
    QWidget *ret = new QWidget;
    QLabel *apps[11];

    ret->setStyleSheet("background-image:url(:/resource/imgs/background1.png);");

    for (int i=11; i<17; ++i)
    {
        apps[i-11] = new QLabel(ret);
        apps[i-11]->setScaledContents(true);
        apps[i-11]->setFixedSize(100, 100);
        apps[i-11]->setScaledContents(true);
        apps[i-11]->setPixmap(QString(":/resource/imgs/(%1).jpg").arg(i));
        apps[i-11]->move((i-11) * 130 + 25, 25);
        apps[i-11]->setToolTip("点我");
    }

    for (int i=17; i<21; ++i)
    {
        apps[i-17] = new QLabel(ret);
        apps[i-17]->setScaledContents(true);
        apps[i-17]->setFixedSize(100, 100);
        apps[i-17]->setScaledContents(true);
        apps[i-17]->setPixmap(QString(":/resource/imgs/(%1).jpg").arg(i));
        apps[i-17]->move((i-17) * 130 + 25, 150);
        apps[i-17]->setToolTip("点我");
    }

    return ret;
}

// 简单随意的测试布局
QWidget *createPage3()
{
    QWidget *ret = new QWidget;
    QLabel *apps[11];

    ret->setStyleSheet("background-image:url(:/resource/imgs/background2.png);");

    for (int i=21; i<27; ++i)
    {
        apps[i-21] = new QLabel(ret);
        apps[i-21]->setScaledContents(true);
        apps[i-21]->setFixedSize(100, 100);
        apps[i-21]->setScaledContents(true);
        apps[i-21]->setPixmap(QString(":/resource/imgs/(%1).jpg").arg(i));
        apps[i-21]->move((i-21) * 130 + 25, 25);
        apps[i-21]->setToolTip("点我");
    }

    for (int i=27; i<30; ++i)
    {
        apps[i-27] = new QLabel(ret);
        apps[i-27]->setScaledContents(true);
        apps[i-27]->setFixedSize(100, 100);
        apps[i-27]->setScaledContents(true);
        apps[i-27]->setPixmap(QString(":/resource/imgs/(%1).jpg").arg(i));
        apps[i-27]->move((i-27) * 130 + 25, 150);
        apps[i-27]->setToolTip("点我");
    }

    QMovie *movie = new QMovie(":/resource/imgs/icon.gif");
    QLabel *p = new QLabel(ret);
    p->setScaledContents(true);
    p->setFixedSize(100, 100);
    p->setScaledContents(true);
    p->setMovie(movie);
    p->move((30-27) * 130 + 25, 150);
    movie->start();

    return ret;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SliderWidget w(nullptr);
    //SliderWidget w(nullptr, ":/resource/imgs/background2.png");

    w.addWidget(createPage1());
    w.addWidget(createPage2());
    w.addWidget(createPage3());

    w.setWindowIcon(QIcon(":/resource/imgs/logo.png"));
    w.setWindowTitle(" 自定义滑动组件");
    w.setFixedSize(800, 480);
    w.show();

    return a.exec();
}
