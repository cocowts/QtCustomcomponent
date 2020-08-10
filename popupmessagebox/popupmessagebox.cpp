#include "popupmessagebox.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QHBoxLayout>
#include <QPropertyAnimation>
#include <QScreen>
#include <QStringLiteral>
#include <QTimer>
#include <QVBoxLayout>
#include <QMovie>

PopupMessageBox::PopupMessageBox(QWidget *parent) : QWidget(parent)
{
    initUI();
    initControl();

    setTitle(QStringLiteral("温馨提示 ^_^ ~"));
    setText(QStringLiteral("可加载图片/文字/动画\n 按钮可隐藏"));
    setButtonVisible(false);
    hide();
}

PopupMessageBox::~PopupMessageBox()
{ }

void PopupMessageBox::initUI()
{
    m_iconLbl.setObjectName(QStringLiteral("iconLbl"));
    m_titleLbl.setObjectName(QStringLiteral("titleLbl"));
    m_closeBtn.setObjectName(QStringLiteral("closeBtn"));
    m_mainLbl.setObjectName(QStringLiteral("mainLbl"));
    m_okBtn.setObjectName(QStringLiteral("okBtn"));
    m_cancleBtn.setObjectName(QStringLiteral("cancleBtn"));

    //==================== 组件属性设置
    m_iconLbl.setScaledContents(true);
    m_iconLbl.setFixedSize(20, 20);
    m_titleLbl.setFixedHeight(24);

    m_closeBtn.setFlat(true);
    m_closeBtn.setFixedSize(14, 14);
    m_closeBtn.setToolTip(QStringLiteral("关闭"));

    m_mainLbl.setAlignment(Qt::AlignCenter);
    //m_mainLbl.setScaledContents(true);
    m_mainLbl.setWordWrap(true);

    m_okBtn.setFixedSize(150, 35);
    m_cancleBtn.setFixedSize(150, 35);
    m_okBtn.setText(QStringLiteral("确定(&O)"));
    m_cancleBtn.setText(QStringLiteral("取消(&C)"));
    m_okBtn.setCursor(Qt::PointingHandCursor);
    m_cancleBtn.setCursor(Qt::PointingHandCursor);

    //================ 组件布局
    QWidget *titleWidget = new QWidget(this);
    QHBoxLayout *pLayout1 = new QHBoxLayout;
    titleWidget->setObjectName(QStringLiteral("titleWidget"));
    pLayout1->addSpacing(5);
    pLayout1->addWidget(&m_iconLbl);
    pLayout1->addSpacing(5);
    pLayout1->addWidget(&m_titleLbl);
    pLayout1->addStretch();
    pLayout1->addWidget(&m_closeBtn);
    pLayout1->addSpacing(5);
    pLayout1->setContentsMargins(0, 0, 0, 0);
    titleWidget->setLayout(pLayout1);
    titleWidget->setFixedHeight(30);

    mpBtnLayout = new QHBoxLayout;
    mpBtnLayout->addWidget(&m_okBtn);
    mpBtnLayout->addWidget(&m_cancleBtn);
    mpBtnLayout->setContentsMargins(0, 10, 0, 0);

    QVBoxLayout *pMainLayout = new QVBoxLayout;
    pMainLayout->addWidget(titleWidget);
    pMainLayout->addWidget(&m_mainLbl);
    pMainLayout->addLayout(mpBtnLayout);
    pMainLayout->setSpacing(0);
    pMainLayout->setContentsMargins(0, 0, 0, 0);

    //================ // 样式表设置
    QFile file(QStringLiteral(":/popumessagebox/popupmessagebox/black.qss"));
    file.open(QFile::ReadOnly);
    this->setStyleSheet(file.readAll());
    file.close();

    //================
    setLayout(pMainLayout);
    setWindowFlag(Qt::FramelessWindowHint);
    setFixedSize(400, 300);
}

void PopupMessageBox::initControl()
{
    connect(&m_okBtn, &QPushButton::clicked, [=](){
        emit accepted();
        m_timer.stop();
        animationQuit();
    });

    connect(&m_cancleBtn, &QPushButton::clicked, [=](){
        emit rejected();
        m_timer.stop();
        animationQuit();
    });

    connect(&m_closeBtn, &QPushButton::clicked, [=](){
        m_timer.stop();
        animationQuit();
    });

    connect(&m_timer, &QTimer::timeout, [=](){
        animationQuit();
    });

    setAttribute(Qt::WA_DeleteOnClose);
}

void PopupMessageBox::setFixedHeight(int h)
{
    QWidget::setFixedHeight(h);
}

void PopupMessageBox::setFixedSize(const QSize &s)
{
    QWidget::setFixedSize(s);
}

void PopupMessageBox::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
}

void PopupMessageBox::setFixedWidth(int w)
{
    QWidget::setFixedWidth(w);
}

QPointer<PopupMessageBox> PopupMessageBox::createInstance(QWidget *parent)
{
    return (new PopupMessageBox(parent));
}

void PopupMessageBox::setTitle(const QString &text)
{
    m_titleLbl.setText(text);

}

QString PopupMessageBox::title() const
{
    return m_titleLbl.text();
}

void PopupMessageBox::setTitleIcon(const QPixmap &pixmap)
{
    m_iconLbl.setPixmap(pixmap);
}

QPixmap PopupMessageBox::titleIcon() const
{
    return m_iconLbl.pixmap(Qt::ReturnByValue);
}

void PopupMessageBox::setText(const QString &text)
{
    m_mainLbl.setText(text);
}

QString PopupMessageBox::text() const
{
    return m_mainLbl.text();
}

void PopupMessageBox::setTextColor(const QColor &color)
{
    QString c = QString::number(((color.red() << 16) | (color.green() << 8) | color.blue()), 16);

    m_mainLbl.setStyleSheet(QStringLiteral("QLabel{color:#%1;}").arg(c));
}

void PopupMessageBox::setIcon400x270_400x215(const QPixmap & pximap)
{
    m_mainLbl.setPixmap(pximap);
}

QPixmap PopupMessageBox::icon() const
{
    return m_mainLbl.pixmap(Qt::ReturnByValue);
}

void PopupMessageBox::setMovie400x270_400x215(const QString &fileName)
{
    QMovie *movie = new QMovie(fileName);
    movie->setParent(this);
    m_mainLbl.setMovie(movie);
    movie->start();
}

const QMovie *PopupMessageBox::movie() const
{
    return m_mainLbl.movie();
}

void PopupMessageBox::setButtonVisible(bool visible)
{
    m_okBtn.setVisible(visible);
    m_cancleBtn.setVisible(visible);

    if (visible)
        mpBtnLayout->setContentsMargins(0, 10, 0, 10);
    else
        mpBtnLayout->setContentsMargins(0, 0, 0, 0);
}

bool PopupMessageBox::isButtonVisible() const
{
    return m_okBtn.isVisible();
}

void PopupMessageBox::animationDrop(int msecs)
{
    QPropertyAnimation *pAnimation = new QPropertyAnimation(this, "pos");
    QRect rect = QGuiApplication::screens().at(0)->availableGeometry();

    pAnimation->setDuration(msecs);
    pAnimation->setStartValue(QPoint(rect.width()-width(), 0));
    pAnimation->setEndValue(QPoint(rect.width()-width(), rect.height()-height()));
    pAnimation->setEasingCurve(QEasingCurve::OutElastic);
    pAnimation->start(QPropertyAnimation::DeleteWhenStopped);
}

void PopupMessageBox::animationShake(int msecs)
{
    QPropertyAnimation *pAnimation = new QPropertyAnimation(this, "pos");
    QRect rect = QGuiApplication::screens().at(0)->availableGeometry();
    QPoint endPoint(rect.width()-width(), rect.height()-height());

    pAnimation->setDuration(msecs);
    pAnimation->setLoopCount(1);
    pAnimation->setKeyValueAt(0, QPoint(endPoint.x() - 3, endPoint.y() - 3));
    pAnimation->setKeyValueAt(0.1, QPoint(endPoint.x() + 6, endPoint.y() + 6));
    pAnimation->setKeyValueAt(0.2, QPoint(endPoint.x() - 6, endPoint.y() + 6));
    pAnimation->setKeyValueAt(0.3, QPoint(endPoint.x() + 6, endPoint.y() - 6));
    pAnimation->setKeyValueAt(0.4, QPoint(endPoint.x() - 6, endPoint.y() - 6));
    pAnimation->setKeyValueAt(0.5, QPoint(endPoint.x() + 6, endPoint.y() + 6));
    pAnimation->setKeyValueAt(0.6, QPoint(endPoint.x() - 6, endPoint.y() + 6));
    pAnimation->setKeyValueAt(0.7, QPoint(endPoint.x() + 6, endPoint.y() - 6));
    pAnimation->setKeyValueAt(0.8, QPoint(endPoint.x() - 6, endPoint.y() - 6));
    pAnimation->setKeyValueAt(0.9, QPoint(endPoint.x() + 6, endPoint.y() + 6));
    pAnimation->setKeyValueAt(1, QPoint(endPoint.x(), endPoint.y()));
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void PopupMessageBox::animationOpacity(int msecs)
{
    QPropertyAnimation *pAnimation = new QPropertyAnimation(this, "windowOpacity");
    QRect rect = QGuiApplication::screens().at(0)->availableGeometry();

    move(rect.width()-width(), rect.height()-height());

    pAnimation->setDuration(msecs);
    pAnimation->setKeyValueAt(0, 0);
    pAnimation->setKeyValueAt(1, 1);
    pAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void PopupMessageBox::animationUp(int msecs)
{
    QPropertyAnimation *pAnimation = new QPropertyAnimation(this, "pos");
    QRect rect = QGuiApplication::screens().at(0)->availableGeometry();

    pAnimation->setDuration(msecs);
    pAnimation->setStartValue(QPoint(rect.width()-width(), rect.height()+height()));
    pAnimation->setEndValue(QPoint(rect.width()-width(), rect.height()-height()));
    pAnimation->setEasingCurve(QEasingCurve::Linear);
    pAnimation->start(QPropertyAnimation::DeleteWhenStopped);
}

void PopupMessageBox::animationQuit(int msecs)
{
    QPropertyAnimation *pAnimation = new QPropertyAnimation(this, "geometry");
    QRect rect = QGuiApplication::screens().at(0)->availableGeometry();

    connect(pAnimation, &QPropertyAnimation::finished, [=](){
        emit quit();
        close();
    });

    pAnimation->setDuration(msecs);
    pAnimation->setStartValue(QRect(rect.width()-width(), rect.height()-height(), width(), height()));
    pAnimation->setEndValue(QRect(rect.width(), rect.height()-height(), width(), height()));
    pAnimation->setEasingCurve(QEasingCurve::Linear);
    pAnimation->start(QPropertyAnimation::DeleteWhenStopped);
}

void PopupMessageBox::animationKeep(int msecs)
{
    m_timer.start(msecs);
}

void PopupMessageBox::start(AnimationType type, int keepTimeMs, int enterTimeMs)
{
    show();

    switch (type)
    {
    case Animation_Drop:
            animationDrop(enterTimeMs);
        break;
    case Animation_Shark:
            animationShake(enterTimeMs);
        break;
    case Animation_Opacity:
        animationOpacity(enterTimeMs);
        break;
    case Animation_Up:
        animationUp(enterTimeMs);
        break;
    }

    if (keepTimeMs >= 0)
        animationKeep(enterTimeMs + keepTimeMs);
}

void PopupMessageBox::terminate()
{
    close();
}





