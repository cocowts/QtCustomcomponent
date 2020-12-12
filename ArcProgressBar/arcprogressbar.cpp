#include "arcprogressbar.h"

#include <QPainter>
#include <QtMath>
#include <QRectF>

ArcProgressBar::ArcProgressBar(QWidget *parent) : QWidget(parent)
{
    m_animation.setTargetObject(this);
    m_animation.setPropertyName("curValue");
    m_animation.setEasingCurve(m_easingCurveType);
}

ArcProgressBar::~ArcProgressBar()
{ }

void ArcProgressBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);
    painter.translate(width()/2.0, height()/2.0);
    m_radius = qMin(width(), height()) / 2.0;

    drawArc(&painter);
    drawValue(&painter);
    drawTitle(&painter);

    return QWidget::paintEvent(event);
}

void ArcProgressBar::drawArc(QPainter *painter)
{
    double pen_width = m_radius * 0.17;
    double radius    = m_radius - pen_width;
    QRectF rect(-radius, -radius, 2.0*radius, 2.0*radius);

    painter->save();

    QPen pen;
    pen.setWidthF(pen_width);
    pen.setCapStyle(Qt::RoundCap);

    painter->setBrush(Qt::NoBrush);

    double allAngle   = m_endAngle - m_startAngle;
    double curAngle   = allAngle * (m_curValue - m_minValue) / (m_maxValue - m_minValue);
    double otherAngle = allAngle - curAngle;

    pen.setColor(m_baseColor);
    painter->setPen(pen);
    painter->drawArc(rect, (270 - m_startAngle-curAngle)*16, -otherAngle * 16);

    pen.setColor(m_arcColor);
    painter->setPen(pen);
    painter->drawArc(rect, (270 - m_startAngle) * 16, -curAngle*16);

    painter->restore();
}

void ArcProgressBar::drawValue(QPainter *painter)
{
    double radius = m_radius * 0.7;

    painter->save();

    painter->setPen(m_textColor);
    painter->setFont(QFont("Consolas", m_radius * 0.25, QFont::Bold));
    double temp = 1.0 * m_curValue / (m_maxValue - m_minValue) * 100;
    painter->drawText(QRect(-radius, -radius*0.1, radius * 2, radius), Qt::AlignCenter, QString("%1%").arg(temp));

    painter->restore();
}

void ArcProgressBar::drawTitle(QPainter *painter)
{
    double radius = m_radius * 0.7;

    painter->save();

    painter->setPen(m_titleColor);
    painter->setFont(QFont("Consolas", m_radius * 0.12, QFont::Bold));
    painter->drawText(QRect(-radius, -radius*0.8, radius * 2, radius), Qt::AlignCenter, m_title);

    painter->restore();
}

void ArcProgressBar::updateValue(int value)
{
    m_curValue = value;
    update();
}

int ArcProgressBar::getCurValue() const
{
    return m_curValue;
}

void ArcProgressBar::setValue(int value)
{
    if (value < m_minValue)
            value = m_minValue;
    else if (value > m_maxValue)
        value = m_minValue;

    m_value = value;

    if (m_animationStepTime == 0)
    {
        updateValue(m_value);
    }
    else if (m_curValue != m_value)
    {
        m_animation.stop();
        m_animation.setKeyValueAt(0, m_curValue);
        m_animation.setKeyValueAt(1, m_value);

        m_animation.setDuration(m_animationStepTime * (m_value - m_minValue));
        m_animation.start();
    }
}

void ArcProgressBar::setArcColor(const QColor &color)
{
    m_arcColor = color;
    update();
}

void ArcProgressBar::setBaseColor(const QColor &color)
{
    m_baseColor = color;
    update();
}

void ArcProgressBar::setTextColor(const QColor &color)
{
    m_textColor = color;
    update();
}

void ArcProgressBar::setTitleColor(const QColor &color)
{
    m_titleColor = color;
    update();
}

void ArcProgressBar::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void ArcProgressBar::setRange(int minValue, int maxValue)
{
    m_minValue = minValue;
    m_maxValue = maxValue;
    update();
}

void ArcProgressBar::setMinValue(int minValue)
{
    m_minValue = minValue;
    update();
}

void ArcProgressBar::setMaxValue(int maxValue)
{
    m_maxValue = maxValue;
    update();
}

void ArcProgressBar::setAngleRange(int startAngle, int endAngle)
{
    m_startAngle = startAngle;
    m_endAngle   = endAngle;
    update();
}

void ArcProgressBar::setStartAngle(int startAngle)
{
    m_startAngle = startAngle;
    update();
}

void ArcProgressBar::setEndAngle(int endAngle)
{
    m_endAngle   = endAngle;
    update();
}

void ArcProgressBar::setAnimationStepTime(int msec)
{
    m_animationStepTime = msec;
    update();
}

QColor ArcProgressBar::getArcColor() const
{
    return m_arcColor;
}

QColor ArcProgressBar::getBaseColor() const
{
    return m_baseColor;
}

QColor ArcProgressBar::getTextColor() const
{
    return m_textColor;
}

QColor ArcProgressBar::getTitleColor() const
{
    return m_title;
}

QString ArcProgressBar::getTitle() const
{
    return m_title;
}

int ArcProgressBar::getMinValue() const
{
    return m_minValue;
}

int ArcProgressBar::getMaxValue() const
{
    return m_maxValue;
}

int ArcProgressBar::getValue() const
{
    return m_value;
}

int ArcProgressBar::getStartAngle() const
{
    return m_startAngle;
}

int ArcProgressBar::getEndAngle() const
{
    return m_endAngle;
}

int ArcProgressBar::getAnimationStepTime() const
{
    return m_animationStepTime;
}



