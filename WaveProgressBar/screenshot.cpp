#include "wareprogressbar.h"

#include <QFontMetrics>
#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <QRectF>
#include <QtMath>

WareProgressBar::WareProgressBar(QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);

    connect(&m_timer, &QTimer::timeout, this, [&]()
    {
        if (m_waveForwardOrientation)
        {
            m_offset += 0.6;
            if (m_offset > m_radius)
                m_offset = -m_radius;
        }
        else
        {
            m_offset -= 0.6;
            if (m_offset < -m_radius)
                m_offset = m_radius;
        }
        update();
    });

    m_timer.start(80);
}

WareProgressBar::~WareProgressBar()
{ }

void WareProgressBar::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);
    painter.translate(width()/2.0, height()/2.0);
    m_radius = qMin(width(), height()) / 2.0;

    drawBackground(&painter);
    if (m_peiCircleIsvisible) drawPieCircel(&painter);
    if (m_waterIsvisible)     drawWater(&painter);
    drawText(&painter);

    return QWidget::paintEvent(event);
}

void WareProgressBar::drawPieCircel(QPainter *painter)
{
    double pen_width = m_radius * 0.1;
    double radius    = m_radius - pen_width;
    double angle     = 360.0 * (m_value - m_minValue) / (m_maxValue - m_minValue);

    painter->save();

    QPen pen;
    pen.setWidthF(pen_width);
    pen.setColor(m_peiCircleColor);
    pen.setCapStyle(Qt::SquareCap);
    painter->setPen(pen);
    painter->drawArc(-radius, -radius, radius*2, radius*2, m_pieStartAngle * 16, m_peiForwardOrientation ? -angle *16 : angle *16);

    painter->restore();
}

void WareProgressBar::drawBackground(QPainter *painter)
{
    double radius = m_radius * 0.81;

    painter->save();

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(50, 50, 50, 100));
    painter->drawEllipse(QPointF(0, 0), radius, radius);

    painter->restore();
}

void WareProgressBar::drawWater(QPainter *painter)
{
    double radius = m_radius * 0.8;
    double percent = 1.0 * (m_value - m_minValue) / (m_maxValue - m_minValue);

    double w = m_waveDensity * M_PI / radius;
    double A = radius * m_waveHeight / 40;
    double k = (0.5 - percent) * radius * 2;

    painter->save();

    QPainterPath wavePath1;
    QPainterPath wavePath2;
    wavePath1.moveTo(-radius, radius);
    wavePath2.moveTo(-radius, radius);

    for (double x=-radius; x<=radius; ++x)
    {
        double waterY1 = 0.0;
        double waterY2 = 0.0;

        if (m_value == m_minValue)
        {
            waterY2 = waterY1 = radius;
        }
        else if (m_value == m_maxValue)
        {
            waterY2 = waterY1 = -radius;
        }
        else
        {
            waterY1 = (double)(A * qSin(w * x + m_offset)) + k;
            waterY2 = (double)(A * qSin(w * x + m_offset + (radius / 2 * w))) + k;
        }

        wavePath1.lineTo(QPointF(x, waterY1));
        wavePath2.lineTo(QPointF(x, waterY2));
    }

    wavePath1.lineTo(radius, radius);
    wavePath2.lineTo(radius, radius);

    QPainterPath bigPath;
    bigPath.addEllipse(QPointF(0, 0), radius, radius);

    QColor watercolor1 = m_waterColor;
    QColor watercolor2 = m_waterColor;

    painter->setPen(Qt::NoPen);

    QPainterPath intersectedPath = bigPath.intersected(wavePath1);
    watercolor1.setAlpha(100);
    painter->setBrush(watercolor1);
    painter->drawPath(intersectedPath);

    intersectedPath = bigPath.intersected(wavePath2);
    watercolor2.setAlpha(180);
    painter->setBrush(watercolor2);
    painter->drawPath(intersectedPath);

    painter->restore();
}

void WareProgressBar::drawText(QPainter *painter)
{
    painter->save();

    painter->setFont(QFont("Consolas", m_radius * 0.25, QFont::Bold));
    painter->setPen(m_textColor);
    int temp = 1.0 * m_value / (m_maxValue - m_minValue) * 100;
    painter->drawText(QRect(-m_radius, -m_radius, m_radius*2, m_radius*2), Qt::AlignCenter, QString("%1%2").arg(temp).arg(m_suffixText));

    painter->restore();
}

void WareProgressBar::setPieCircleColor(const QColor &color)
{
    m_peiCircleColor = color;
    update();
}

void WareProgressBar::setwaterColor(const QColor &color)
{
    m_waterColor = color;
    update();
}

void WareProgressBar::setTextColor(const QColor &color)
{
    m_textColor = color;
    update();
}

void WareProgressBar::setPieStartAngle(int angle)
{
    m_pieStartAngle = angle;
    update();
}

void WareProgressBar::setValue(int value)
{
    if (value < m_minValue)
        value = m_minValue;
    else if (value > m_maxValue)
        value = m_maxValue;

    m_value = value;
    update();
}

void WareProgressBar::setRange(int minValue, int maxValue)
{
    m_minValue = minValue;
    m_maxValue = maxValue;
    update();
}

void WareProgressBar::setMaxValue(int value)
{
    m_maxValue = value;
    update();
}

void WareProgressBar::setMinValue(int value)
{
    m_minValue = value;
    update();
}

void WareProgressBar::setWaveDensity(int density)
{
    m_waveDensity = density;
    update();
}

void WareProgressBar::setWaveHeight(int height)
{
    m_waveHeight = height;
    update();
}

void WareProgressBar::setSuffixText(const QString &text)
{
    m_suffixText = text;
    update();
}

void WareProgressBar::setWaveIsForwardOrientation(bool Orientation)
{
    m_waveForwardOrientation = Orientation;
    update();
}

void WareProgressBar::setWaterIsvisible(bool visible)
{
    m_waterIsvisible = visible;
    update();
}

void WareProgressBar::setPeiCircleIsvisible(bool visible)
{
    m_peiCircleIsvisible = visible;
    update();
}

void WareProgressBar::setPeiIsForwardOrientation(bool Orientation)
{
    m_peiForwardOrientation = Orientation;
    update();
}

QColor WareProgressBar::getPieCircleColor() const
{
    return m_peiCircleColor;
}

QColor WareProgressBar::getwaterColor() const
{
    return m_waterColor;
}

QColor WareProgressBar::getTextColor() const
{
    return m_textColor;
}

int WareProgressBar::getPieStartAngle()  const
{
    return m_pieStartAngle;
}

int WareProgressBar::getValue() const
{
    return m_value;
}

int WareProgressBar::getMaxValue() const
{
    return m_maxValue;
}

int WareProgressBar::getMinValue() const
{
    return m_minValue;
}

int WareProgressBar::getWaveDensity() const
{
    return m_waveDensity;
}

int WareProgressBar::getWaveHeight() const
{
    return m_waveHeight;
}

QString WareProgressBar::getSuffixText() const
{
    return m_suffixText;
}

bool WareProgressBar::getWaveIsForwardOrientation() const
{
    return m_waveForwardOrientation;
}

bool WareProgressBar::getWaterIsvisible() const
{
    return m_waterIsvisible;
}

bool WareProgressBar::getPeiCircleIsvisible() const
{
    return m_peiCircleIsvisible;
}

bool WareProgressBar::getPeiIsForwardOrientation() const
{
    return m_peiForwardOrientation;
}
