#include "sliderWidget.h"

#include <QScroller>
#include <QScrollerProperties>
#include <QScrollBar>
#include <QCursor>
#include <QLabel>
#include <QPixmap>

SliderWidget::SliderWidget(QWidget *parent, const QString &picture)
    : QWidget(parent)
{
    m_background = picture;

    initUi();
    initCtrl();
}

SliderWidget::~SliderWidget()
{
}

void SliderWidget::initUi()
{
    this->setAttribute(Qt::WA_TranslucentBackground);

    m_pMainWidget = new QWidget(this);
    m_pMainWidget->setStyleSheet("background: transparent");

    m_pScrollArea = new QScrollArea(this);
    m_pScrollArea->setAlignment(Qt::AlignCenter);
    m_pScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_pScrollArea->setWidget(m_pMainWidget);
    if (m_background.isEmpty())
        m_pScrollArea->setStyleSheet("background: transparent");
    else
        m_pScrollArea->setStyleSheet(QString("background-image:url(%1);").arg(m_background));

    auto scroller = QScroller::scroller(m_pScrollArea);
    scroller->grabGesture(m_pScrollArea, QScroller::LeftMouseButtonGesture);

    auto properties = scroller->scrollerProperties();
    properties.setScrollMetric(QScrollerProperties::SnapTime, 0.5);
    properties.setScrollMetric(QScrollerProperties::MinimumVelocity, 10);
    scroller->setScrollerProperties(properties);

    m_pMainLayout = new QHBoxLayout;
    m_pMainLayout->setMargin(0);
    m_pMainLayout->setSpacing(0);
    m_pMainWidget->setLayout(m_pMainLayout);

    m_pIndicatorWidget = new QWidget(this);
    m_pIndicatorLayout = new QHBoxLayout;
    m_pIndicatorLayout->setAlignment(Qt::AlignCenter);
    m_pIndicatorLayout->setMargin(0);
    m_pIndicatorWidget->setLayout(m_pIndicatorLayout);
}

void SliderWidget::initCtrl()
{
    m_pAnimation = new QPropertyAnimation(m_pScrollArea->horizontalScrollBar(), "value", this);
    m_pAnimation->setDuration(200);
    m_pAnimation->setEasingCurve(QEasingCurve::OutCurve);

    m_pSlidingTimer = new QTimer(this);
    m_pSlidingTimer->setInterval(300);

    connect(QScroller::scroller(m_pScrollArea), &QScroller::stateChanged, this, &SliderWidget::onScrollerStateChanged);
    connect(m_pSlidingTimer, &QTimer::timeout, this, &SliderWidget::onSliderTimerTimeout);
}

void SliderWidget::updateIndicator(int index)
{
    const QObjectList &childs = m_pIndicatorWidget->children();
    for (int i=1; i<childs.count(); ++i)
    {
        auto pIndicatorLbl = qobject_cast<QLabel*>(childs[i]);

        if (i-1 == index)
            pIndicatorLbl->setPixmap(QPixmap(":/resource/icons/indicatorActive.png"));
        else
             pIndicatorLbl->setPixmap(QPixmap(":/resource/icons/indicatorInactive.png"));
   }
}

int SliderWidget::addWidget(QWidget *page)
{
    if (!m_background.isEmpty())
        page->setStyleSheet("background: transparent");

    m_pMainLayout->addWidget(page);

    auto pIndicatorLbl = new QLabel;
    m_pIndicatorLayout->addWidget(pIndicatorLbl);

    if (count() == 1)
        pIndicatorLbl->setPixmap(QPixmap(":/resource/icons/indicatorActive.png"));
    else
       pIndicatorLbl->setPixmap(QPixmap(":/resource/icons/indicatorInactive.png"));

    return count() - 1;
}

int SliderWidget::count() const
{
    return m_pMainLayout->count();
}

int SliderWidget::currentIndex() const
{
    return m_curPageIndex;
}

QWidget *SliderWidget::currentWidget() const
{
    if (count() == 0)
        return nullptr;

    return qobject_cast<QWidget*>(m_pMainWidget->children()[m_curPageIndex+1]);
}

int SliderWidget::indexOf(QWidget *widget) const
{
    auto index = m_pMainWidget->children().indexOf(widget);

    return index == -1 ? -1 : index -1;
}

void SliderWidget::setCurrentIndex(int index)
{
    if (index < 0 || index >= count())
        return;

    m_pAnimation->setStartValue(m_pScrollArea->horizontalScrollBar()->value());
    m_pAnimation->setEndValue(index * this->width());
    m_pAnimation->start();

    m_curPageIndex = index;
    updateIndicator(m_curPageIndex);
    emit currentChanged(m_curPageIndex);
}

void SliderWidget::setCurrentWidget(QWidget *widget)
{
    auto index = m_pMainWidget->children().indexOf(widget);

    if (index == -1)
        return;

    setCurrentIndex(index - 1);
}

void SliderWidget::removeWidget(int index)
{
    if (index < 0 || index >= count() || count() <= 1)
        return;

    auto widget = qobject_cast<QWidget*>(m_pMainWidget->children()[index+1]);
    auto indicator = qobject_cast<QWidget*>(m_pIndicatorWidget->children()[index+1]);

    m_pMainLayout->removeWidget(widget);
    m_pIndicatorLayout->removeWidget(indicator);

    delete widget;
    delete indicator;

    setCurrentIndex(0);
}

void SliderWidget::removeWidget(QWidget *widget)
{
    removeWidget(indexOf(widget));
}

void SliderWidget::resizeEvent(QResizeEvent *ev)
{
    Q_UNUSED(ev)

    m_pScrollArea->resize(this->size());
    m_pMainWidget->resize(this->width() * m_pMainLayout->count(), this->height() - 2);
    m_pIndicatorWidget->setGeometry(0, this->height() - 20, this->width(), 20);

    QWidget::resizeEvent(ev);
}

void SliderWidget::onScrollerStateChanged(QScroller::State state)
{
    if (count() == 0)
        return;

    if (state == QScroller::State::Inactive)
    {
        m_pSlidingTimer->stop();
        m_sliderReleaseValue = QCursor::pos().x();
        if (m_sliderReleaseValue == m_sliderPressedValue)
            return;

        if (!m_sliderFlag)
        {
            if (m_sliderPressedValue - m_sliderReleaseValue > 20)
                ++m_nextPageIndex;
            else if (m_sliderReleaseValue - m_sliderPressedValue > 20)
                --m_nextPageIndex;

            if (m_nextPageIndex > count())
                m_nextPageIndex = count() - 1;
            else if (m_nextPageIndex < 0)
                m_nextPageIndex = 0;
        }
        else
        {
            int value = m_pScrollArea->horizontalScrollBar()->value();
            m_nextPageIndex = (value / this->width()) + (value % this->width() / (this->width() / 2));
        }

        if (m_nextPageIndex != m_curPageIndex)
        {
            m_curPageIndex = m_nextPageIndex;
            updateIndicator(m_curPageIndex);
            emit currentChanged(m_curPageIndex);
        }

        m_pAnimation->setStartValue(m_pScrollArea->horizontalScrollBar()->value());
        m_pAnimation->setEndValue(m_nextPageIndex * this->width());
        m_pAnimation->start();

        m_sliderReleaseValue = 0;
        m_sliderPressedValue = 0;
        m_sliderFlag = 0;
    }

    if (state == QScroller::State::Pressed)
    {
        m_nextPageIndex = m_curPageIndex = m_pScrollArea->horizontalScrollBar()->value() / this->width();
        m_sliderPressedValue = QCursor::pos().x();
        m_pSlidingTimer->start();
    }
}

void SliderWidget::onSliderTimerTimeout()
{
    m_pSlidingTimer->stop();
    m_sliderFlag = true;
}
