#include "bannerlabel.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QStringLiteral>
#include <QStringLiteral>
#include <QTextOption>
#include <QVBoxLayout>

BannerLabel::BannerLabel(QWidget *parent) : QWidget(parent)
{
    initUI();
    initControl();
}

BannerLabel::~BannerLabel()
{ }

void BannerLabel::initUI()
{
    // 放置指示器的窗体载体
    m_pIndicatorWidget = new QWidget(this);

    // 给指示器加上左右布局
    m_pIndicatorLayout = new QHBoxLayout(m_pIndicatorWidget);
    m_pIndicatorLayout->setSpacing(BtnShrinkWidth);

    // 主布局、上下布局
    m_pIndicatorLayout->setAlignment(Qt::AlignLeft);
}

void BannerLabel::initControl()
{
    initAnimation();

    connect(m_pTimer, &QTimer::timeout, this, &BannerLabel::onImageTimerOut);
    connect(m_pBtnGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::idClicked), this, &BannerLabel::onButtonClicked);

    m_pTimer->setInterval(interval);
    m_pTimer->start();
}

void BannerLabel::initAnimation()
{
    // 定制主动画组
    m_pAnimationGroup = new QParallelAnimationGroup(this);

    // 定义动画切换图片
    m_pImageAnimation = new QPropertyAnimation(m_pAnimationGroup, "");
    m_pImageAnimation->setEasingCurve(QEasingCurve::OutCirc);
    m_pImageAnimation->setDuration(800);
    connect(m_pImageAnimation, &QPropertyAnimation::valueChanged, this, &BannerLabel::onImageValueChange);

    // 按钮串行动画组
    QSequentialAnimationGroup *sequentialGroup = new QSequentialAnimationGroup(m_pAnimationGroup);

    m_pExtendBtnAnimation = new QPropertyAnimation(sequentialGroup, "");
    m_pExtendBtnAnimation->setEasingCurve(QEasingCurve::OutCirc);
    m_pExtendBtnAnimation->setDuration(400);
    connect(m_pExtendBtnAnimation, &QPropertyAnimation::valueChanged, this, &BannerLabel::onExtendBtnValueChange);

    m_pShrinkBtnAnimation = new QPropertyAnimation(sequentialGroup, "");
    m_pShrinkBtnAnimation->setEasingCurve(QEasingCurve::OutCirc);
    m_pShrinkBtnAnimation->setDuration(400);
    connect(m_pShrinkBtnAnimation, &QPropertyAnimation::valueChanged, this, &BannerLabel::onShrinkBtnValueChange);

    sequentialGroup->addAnimation(m_pExtendBtnAnimation);
    sequentialGroup->addAnimation(m_pShrinkBtnAnimation);

    // 图片切换与按钮切换并行运行
    m_pAnimationGroup->addAnimation(m_pImageAnimation);
    m_pAnimationGroup->addAnimation(sequentialGroup);
}

void BannerLabel::addImage(const QList<std::pair<QString, QString>> &imagePairList)
{
    QString style = QStringLiteral("QToolButton {border-image:url(:/image/point.png); border-width:%1px;}").arg(BtnShrinkWidth/2-1);  // 再看看！！！

    for (int index=0; index<imagePairList.count(); ++index)
    {
        QToolButton *btn = new QToolButton(m_pIndicatorWidget);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setFixedSize(BtnShrinkWidth, BtnShrinkWidth);
        btn->setStyleSheet(style);
        m_pBtnGroup->addButton(btn, index+m_imagePairList.count());
        m_pIndicatorLayout->addWidget(btn);
    }

    m_imagePairList.append(imagePairList);
    m_pBtnGroup->button(m_currIndex)->setFixedWidth(BtnExtendWidth);
    update();
}

const QList<std::pair<QString, QString>> &BannerLabel::getImages() const
{
    return m_imagePairList;
}

void BannerLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    if (m_imagePairList.count() == 0)
        return;

    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    QTextOption textOption(Qt::AlignLeft | Qt::AlignVCenter);
    painter.setPen(textColor);

    QFont font;
    font.setFamily("微软雅黑");
    font.setPixelSize(20);
    font.setBold(true);
    painter.setFont(font);

    // 取出上张图片 + 当前图片，并平滑缩放
    int width = this->width();
    int height = this->height();
    QPixmap previousPix(m_imagePairList.at(m_preIndex).first);
    QPixmap currentPix(m_imagePairList.at(m_currIndex).first);
    previousPix = previousPix.scaled(width, height, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    currentPix = currentPix.scaled(width, height, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    int widthOffset = m_preIndex < m_currIndex ? width + m_offset : m_offset - width;

    painter.drawPixmap(m_offset, 0, previousPix);
    painter.drawPixmap(widthOffset, 0, currentPix);

    painter.drawText(QRect(m_offset+BtnShrinkWidth, height-(BtnShrinkWidth*5), width-BtnShrinkWidth, BtnShrinkWidth*2), m_imagePairList.at(m_currIndex).second, textOption);
    painter.drawText(QRect(widthOffset+BtnShrinkWidth, height-(BtnShrinkWidth*5), width-BtnShrinkWidth, BtnShrinkWidth*2), m_imagePairList.at(m_currIndex).second, textOption);
}

void BannerLabel::resizeEvent(QResizeEvent *event)
{
    m_pIndicatorWidget->setGeometry(QRect(BtnShrinkWidth, height()-(BtnShrinkWidth*4), width()-BtnShrinkWidth, BtnShrinkWidth*5));

    QWidget::resizeEvent(event);
}

void BannerLabel::enterEvent(QEvent *event)
{
    if (hoverStop)
        m_pTimer->stop();

    QWidget::enterEvent(event);
}

void BannerLabel::leaveEvent(QEvent *event)
{
    if (hoverStop)
        m_pTimer->start();

    QWidget::leaveEvent(event);
}

void BannerLabel::mousePressEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton) && (!m_pIndicatorWidget->geometry().contains(event->pos())))
    {
        emit imageclicked(m_currIndex, m_imagePairList.at(m_currIndex).first, m_imagePairList.at(m_currIndex).second);
    }

    QWidget::mousePressEvent(event);
}

void BannerLabel::setFixedSize(const QSize &s)
{
    QWidget::setFixedSize(s);
}

void BannerLabel::setFixedSize(int w, int h)
{
    QWidget::setFixedSize(w, h);
}

int BannerLabel::getInterval() const
{
    return interval;
}

void BannerLabel::setInterval(int _interval)
{
    interval = _interval;

    m_pTimer->setInterval(_interval);
}

void BannerLabel::setHoverStop(bool isHoverStop)
{
    hoverStop = isHoverStop;
}

bool BannerLabel::getHoverStop() const
{
    return hoverStop;
}

void BannerLabel::setTextColor(const QColor &_textColor)
{
    textColor = _textColor;
}

QColor BannerLabel::getTextColor() const
{
    return textColor;
}

void BannerLabel::setIndicatorPosition(IndicatorPosition _indicatorPosition)
{
    indicatorPosition = _indicatorPosition;

    m_pIndicatorLayout->setAlignment(static_cast<Qt::Alignment>(_indicatorPosition));
}

BannerLabel::IndicatorPosition BannerLabel::getIndicatorPosition() const
{
    return indicatorPosition;
}

void BannerLabel::onImageValueChange(const QVariant &variant)
{
    m_offset = variant.toInt();
    update();
}

void BannerLabel::onExtendBtnValueChange(const QVariant &varient)
{
    m_pBtnGroup->button(m_currIndex)->setFixedWidth(varient.toInt());
}

void BannerLabel::onShrinkBtnValueChange(const QVariant &varient)
{
    for (int index=0; index<m_imagePairList.count(); ++index)
        if ((m_currIndex != index) && (m_pBtnGroup->button(index)->width() > BtnShrinkWidth))
            m_pBtnGroup->button(index)->setFixedWidth(varient.toInt());
}

void BannerLabel::onButtonClicked(int index)
{
    if (m_imagePairList.count() == 0)
        return;

    if (index < 0)
        index = m_imagePairList.size() - 1;

    if (index >= m_imagePairList.size())
        index = 0;

    m_preIndex = m_currIndex;
    m_currIndex = index;

    m_pAnimationGroup->stop();

    if (m_preIndex < m_currIndex)
    {
        m_pImageAnimation->setStartValue(0);
        m_pImageAnimation->setEndValue(0 - width());
    }
    else
    {
        m_pImageAnimation->setStartValue(0);
        m_pImageAnimation->setEndValue(width());
    }

    m_pShrinkBtnAnimation->setStartValue(BtnExtendWidth);
    m_pShrinkBtnAnimation->setEndValue(BtnShrinkWidth);

    m_pExtendBtnAnimation->setStartValue(BtnShrinkWidth);
    m_pExtendBtnAnimation->setEndValue(BtnExtendWidth);

    emit imageChange(m_currIndex, m_imagePairList.at(m_currIndex).first, m_imagePairList.at(m_currIndex).second);

    m_pAnimationGroup->start();

    if (!hoverStop)
        m_pTimer->start();
}

void BannerLabel::onImageTimerOut()
{
    onButtonClicked(m_currIndex+1);
}

void BannerLabel::imageClear()
{
    qDeleteAll(m_pBtnGroup->buttons());
    m_imagePairList.clear();
    m_preIndex = 0;
    m_currIndex = 0;
}
