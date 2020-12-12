#include "colordashboard.h"

#include <QLinearGradient>
#include <QPainter>
#include <QPainterPath>
#include <QRectF>
#include <QtMath>

ColorDashboard::ColorDashboard(QWidget *parent) : QWidget(parent)
{
    m_animation.setTargetObject(this);
    m_animation.setPropertyName("curValue");
    m_animation.setEasingCurve(m_easingCurveType);

    connect(&m_animation, &QPropertyAnimation::valueChanged, [&](const QVariant &var){emit valueChanged(var.toInt());});
    connect(&m_animation, SIGNAL(finished()), this, SIGNAL(finished()));

    setAttribute(Qt::WA_TranslucentBackground, true);
}

ColorDashboard::~ColorDashboard()
{ }

void ColorDashboard::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);
    painter.translate(width()/2.0, height()/2.0);
    m_radius = qMin(width(), height()) /2;;

    drawOuterCircle(&painter);
    drawScaleCircle(&painter);
    drawPieCircle(&painter);
    drawInnerCircle(&painter);
    drawScaleNum(&painter);
    drawScale(&painter);
    drawPointerCircle(&painter);
    drawPointer(&painter);
    drawCenterCircel(&painter);
    drawText(&painter);
    if (m_isOverlayVisible) drawOverlay(&painter);

    return QWidget::paintEvent(e);
}

inline void ColorDashboard::drawGenericCircle(QPainter *painter, double radius, const QColor &color)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(color);
    painter->drawEllipse(QRectF(-radius, -radius, 2.0*radius, 2.0*radius));
    painter->restore();
}

void ColorDashboard::drawOuterCircle(QPainter *painter)
{
    double radius = m_radius;

    drawGenericCircle(painter, radius, m_outerCircleColor);
}

void ColorDashboard::drawScaleCircle(QPainter *painter)
{
    double radius = m_radius * 0.9;

    drawGenericCircle(painter, radius, m_scaleCircleColor);
}

void ColorDashboard::drawPieCircle(QPainter *painter)
{
    double radius = m_radius * 0.58;

    switch (m_pieStyle)
    {
        case PieStyle_Three:
            drawThreePieCircle(painter, radius);
            break;

        case PieStyle_Current:
          drawCurrentPieCircle(painter, radius);
            break;
    }
}

void ColorDashboard::drawThreePieCircle(QPainter *painter, double radius)
{
    double allAngle   = m_endAngle - m_startAngle;
    double angleStart = allAngle * 0.7;
    double angleMid   = allAngle * 0.15;
    double angleEnd   = allAngle * 0.15;
    QRectF rect(-radius, -radius, 2.0*radius, 2.0*radius);

    painter->save();
    painter->setPen(Qt::NoPen);

    // 绘制开始饼圆
    painter->setBrush(m_pieColorStart);
    painter->drawPie(rect, (270 - m_startAngle - angleStart) * 16, angleStart * 16);

    // 绘制中间饼圆
    painter->setBrush(m_pieColorMid);
    painter->drawPie(rect, (270 - m_startAngle - angleStart - angleMid) * 16, angleMid * 16);

    // 绘制结束饼圆
    painter->setBrush(m_pieColorEnd);
    painter->drawPie(rect, (270 - m_startAngle - angleStart - angleMid - angleEnd) * 16, angleEnd * 16);

    painter->restore();
}

void ColorDashboard::drawCurrentPieCircle(QPainter *painter, double radius)
{
    double allAngle   = m_endAngle - m_startAngle;
    double curAngle   = allAngle * (m_curValue - m_minValue) / (m_maxValue - m_minValue);
    double otherAngle = allAngle - curAngle;
    QRectF rect(-radius, -radius, 2.0*radius, 2.0*radius);

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_pieColorStart);
    painter->drawPie(rect, (270 - m_startAngle - curAngle) * 16, curAngle * 16);

    painter->setBrush(m_pieColorEnd);
    painter->drawPie(rect, (270 - m_startAngle - allAngle) * 16, otherAngle * 16);
    painter->restore();
}

void ColorDashboard::drawInnerCircle(QPainter *painter)
{
    double radius = m_radius * 0.48;

    drawGenericCircle(painter, radius, m_innerCircleColor);
}

void ColorDashboard::drawPointerCircle(QPainter *painter)
{
    double radius = m_radius * 0.19;

    drawGenericCircle(painter, radius, m_pointerColor);
}

void ColorDashboard::drawCenterCircel(QPainter *painter)
{
    double radius = m_radius * 0.15;

    drawGenericCircle(painter, radius, m_centerCirleColor);
}

void ColorDashboard::drawScaleNum(QPainter *painter)
{
    const double radius = m_radius * 0.81;
    const double rotate = (m_endAngle - m_startAngle) / m_scaleMajor * 1.0;

    painter->save();
    painter->setFont(QFont("Consolas", m_radius * 0.065, QFont::Bold));
    painter->setPen(m_scaleNumColor);
    QFontMetrics fm = QFontMetrics(painter->font());

    for (int i=0; i<=m_scaleMajor; ++i)
    {
        // 计算每次写text的painter位置
        int angle = 90 + m_startAngle + rotate * i;
        double angleArc =( angle % 360) * M_PI  / 180.0;
        double x = (radius)*cos(angleArc);
        double y = (radius)*sin(angleArc);
        double value = i * ((m_maxValue - m_minValue) / m_scaleMajor) + m_minValue * 1.0;
        QString strValue = QString("%1").arg(value, 0, 'f', 0);
        double w = fm.horizontalAdvance(strValue);
        double h = fm.height();
        x = x - w/2.0;
        y = y + h/4.0;

        painter->drawText(QPointF(x, y),strValue);
    }

    painter->restore();
}

void ColorDashboard::drawScale(QPainter *painter)
{
    const double radius       = m_radius * 0.71;
    const double lineWidth    = m_radius * 0.009;
    const double scaleNum     = m_scaleMajor * m_scaleMinor;
    const double rotate       = (m_endAngle - m_startAngle) / scaleNum;
    QPen pen(painter->pen());

    painter->save();
    pen.setColor(m_scaleColor);
    pen.setCapStyle(Qt::RoundCap);

    // 旋转坐标系，使每次绘制都停留在y轴上，即[x=0, y]
    painter->rotate(m_startAngle);

    for (int i=0; i<=scaleNum; ++i)
    {
        if (i % m_scaleMinor == 0)
        {
            pen.setWidthF(1.4*lineWidth);
            painter->setPen(pen);
            painter->drawLine(QPointF(0, radius), QPointF(0, radius / 1.17));
        }
        else
        {
            pen.setWidthF(1.1*lineWidth);
            painter->setPen(pen);
            painter->drawLine(QPointF(0, radius), QPointF(0, radius / 1.12));
        }

        painter->rotate(rotate);
    }

    painter->restore();
}

void ColorDashboard::drawOverlay(QPainter *painter)
{
    QPainterPath circel_1;
    QPainterPath circle_2;

    painter->save();
    painter->setPen(Qt::NoPen);
    double radius = m_radius*1.6;
    circel_1.addEllipse(QRectF(-radius+radius*0.8, -radius, 2*radius, 2*radius));
    radius = m_radius * 0.9;
    circle_2.addEllipse(QRectF(-radius, -radius, 2*radius, 2*radius));

    QPainterPath hightCircle = circle_2 - circel_1;
    QLinearGradient gradient(-radius/2, 0, 0, 0);
    m_overlayColor.setAlpha(100);
    gradient.setColorAt(0.0, m_overlayColor);
    m_overlayColor.setAlpha(30);
    gradient.setColorAt(1.0, m_overlayColor);

    painter->setBrush(gradient);
    painter->rotate(65);
    painter->drawPath(hightCircle);
    painter->restore();
}

void ColorDashboard::drawPointer(QPainter *painter)
{
    switch (m_pointerStyle)
    {
        case PointerStyle_Ball:
            drawPointerBall(painter);
            break;
        case PointerStyle_Indicator:
            drawPointerIndicator(painter);
            break;
        case PointerStyle_IndicatorR:
            drawPointerIndicatorR(painter);
            break;
        case PointerStyle_Triangle:
            drawPointerTriangle(painter);
            break;
    }
}

void ColorDashboard::drawPointerBall(QPainter *painter)
{
    double radius = m_radius * 0.36;
    double rotate = m_startAngle + 1.0 * (m_endAngle - m_startAngle) / (m_maxValue - m_minValue) * m_curValue;

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_pointerColor);
    painter->rotate(rotate);
    painter->drawEllipse(QRectF(-radius * 0.25 * 0.5, radius, radius * 0.25, radius * 0.25));
    painter->restore();
}

void ColorDashboard::drawPointerIndicator(QPainter *painter)
{
    int radius    = m_radius * 0.72;
    int width     = m_radius * 0.06;;
    double rotate = m_startAngle + 1.0 * (m_endAngle - m_startAngle) / (m_maxValue - m_minValue) * m_curValue;

    painter->save();
    painter->setOpacity(0.8);
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_pointerColor);
    painter->rotate(rotate);

    QPolygon polygon;
    polygon.setPoints(3, -width, 0, width, 0, 0, radius);

    painter->drawConvexPolygon(polygon);
    painter->restore();
}

void ColorDashboard::drawPointerIndicatorR(QPainter *painter)
{
    int radius    = m_radius * 0.72;
    int width     = m_radius * 0.06;;
    double rotate = m_startAngle + 1.0 * (m_endAngle - m_startAngle) / (m_maxValue - m_minValue) * m_curValue;

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_pointerColor);
    painter->rotate(rotate);

    QPolygon polygon;
    polygon.setPoints(3, -width, 0, width, 0, 0, radius);
    painter->drawConvexPolygon(polygon);

    QPen pen;
    pen.setColor(m_pointerColor);
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidthF(m_radius*0.035);
    painter->setPen(pen);
    painter->drawLine(0, 0, 0, radius);
    painter->restore();
}

void ColorDashboard::drawPointerTriangle(QPainter *painter)
{
    int radius = m_radius * 0.45;
    int width  = m_radius * 0.06;;
    int offset = m_radius * 0.35;
    double rotate = m_startAngle + 1.0 * (m_endAngle - m_startAngle) / (m_maxValue - m_minValue) * m_curValue;

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_pointerColor);
    painter->rotate(rotate);

    QPolygon polygon;
    polygon.setPoints(3, -width, offset, width, offset, 0, radius);
    painter->drawConvexPolygon(polygon);
    painter->restore();
}

void ColorDashboard::drawText(QPainter *painter)
{
    painter->save();
    painter->setPen(m_textColor);
    painter->setFont(QFont("Consolas", m_radius * 0.1, QFont::Bold));

    QFontMetrics fm = QFontMetrics(painter->font());
    QString strVal  = QString::number(m_curValue, 10);
    int w = fm.horizontalAdvance(strVal);
    int h = fm.height();
    painter->drawText(-w/2, -h/2, w, h, Qt::AlignCenter, strVal);
    painter->restore();
}

void ColorDashboard::setRange(int minValue, int maxValue)
{
    m_minValue = minValue;
    m_maxValue = maxValue;
    update();
}

void ColorDashboard::setMinValue(int minValue)
{
    m_minValue = minValue;
    update();
}

void ColorDashboard::setMaxValue(int maxValue)
{
    m_maxValue = maxValue;
    update();
}

void ColorDashboard::setValue(int value)
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
        m_animation.setDuration(m_animationStepTime * (m_scaleMajor + m_scaleMinor));
        m_animation.start();
    }
}

void ColorDashboard::setScaleMajor(int scaleMajor)
{
    m_scaleMajor = scaleMajor;
    update();
}

void ColorDashboard::setScaleMinor(int scaleMinor)
{
    m_scaleMinor = scaleMinor;
    update();
}

void ColorDashboard::setAngleRange(int startAngle, int endAngle)
{
    m_startAngle = startAngle;
    m_endAngle   = endAngle;
    update();
}

void ColorDashboard::setStartAngle(int startAngle)
{
    m_startAngle = startAngle;
    update();
}

void ColorDashboard::setEndAngle(int endAngle)
{
    m_endAngle = endAngle;
    update();
}

void ColorDashboard::setAnimationStepTime(int msec)
{
    m_animationStepTime = msec;
}

void ColorDashboard::setOuterCircleColor(const QColor &outerCircleColor)
{
    m_outerCircleColor = outerCircleColor;
    update();
}

void ColorDashboard::setInnerCircleColor(const QColor &innerCircleColor)
{
    m_innerCircleColor = innerCircleColor;
    update();
}

void ColorDashboard::setPieStartColor(const QColor &scaleStartColor)
{
    m_pieColorStart = scaleStartColor;
    update();
}

void ColorDashboard::setPieMidColor(const QColor &scaleMidColor)
{
    m_pieColorMid = scaleMidColor;
    update();
}

void ColorDashboard::setPieEndColor(const QColor &scaleEndColor)
{
    m_pieColorEnd = scaleEndColor;
    update();
}

void ColorDashboard::setScaleColor(const QColor &scaleColor)
{
    m_scaleColor = scaleColor;
    update();
}

void ColorDashboard::setScalCircleColor(const QColor &scaleCircleColor)
{
    m_scaleCircleColor = scaleCircleColor;
    update();
}

void ColorDashboard::setScaleNumColor(const QColor &scaleNumColor)
{
    m_scaleNumColor = scaleNumColor;
    update();
}

void ColorDashboard::setPointerColor(const QColor &pointerColor)
{
    m_pointerColor = pointerColor;
    update();
}

void ColorDashboard::setCenterCircleColor(const QColor &centerCircleColor)
{
    m_centerCirleColor = centerCircleColor;
    update();
}

void ColorDashboard::setTextColor(const QColor &textColor)
{
    m_textColor = textColor;
    update();
}

void ColorDashboard::setOverlayVisible(bool overlay)
{
    m_isOverlayVisible = overlay;
    update();
}

void ColorDashboard::setOverlayColor(const QColor &overlayColor)
{
    m_overlayColor = overlayColor;
    update();
}

void ColorDashboard::setPieStyle(PieStyle pieStyle)
{
    m_pieStyle = pieStyle;
    update();
}

void ColorDashboard::setPointerStyle(PointerStyle pointerStyle)
{
    m_pointerStyle = pointerStyle;
    update();
}

void ColorDashboard::setEasingCurve(QEasingCurve::Type type)
{
    m_easingCurveType = type;
    update();
}

void ColorDashboard::updateValue(int value)
{
    m_curValue = value;
    update();
}

int ColorDashboard::getCurValue() const
{
    return m_curValue;
}

int ColorDashboard::getMinValue() const
{
    return m_minValue;
}

int ColorDashboard::getMaxValue() const
{
    return m_maxValue;
}

int ColorDashboard::getValue() const
{
    return m_value;
}

int ColorDashboard::getScaleMajor() const
{
    return m_scaleMajor;
}

int ColorDashboard::getScaleMinor() const
{
    return m_scaleMinor;
}

int ColorDashboard::getStartAngle() const
{
    return m_startAngle;
}

int ColorDashboard::getEndAngle() const
{
    return m_endAngle;
}

int ColorDashboard::getAnimationStepTime() const
{
    return m_animationStepTime;
}

QColor ColorDashboard::getOuterCircleColor() const
{
    return m_outerCircleColor;
}

QColor ColorDashboard::getInnerCircleColor() const
{
    return m_innerCircleColor;
}

QColor ColorDashboard::getPieStartColor() const
{
    return m_pieColorStart;
}

QColor ColorDashboard::getPieMidColor() const
{
    return m_pieColorMid;
}
QColor ColorDashboard::getPieEndColor() const
{
    return m_pieColorMid;
}

QColor ColorDashboard::getScaleColor() const
{
    return m_scaleColor;
}

QColor ColorDashboard::getScalCircleColor() const
{
    return m_scaleCircleColor;
}

QColor ColorDashboard::getScaleNumColor() const
{
    return m_scaleNumColor;
}

QColor ColorDashboard::getPointerColor() const
{
    return m_pointerColor;
}

QColor ColorDashboard::getCenterCircleColor() const
{
    return m_centerCirleColor;
}

QColor ColorDashboard::getTextColor() const
{
    return m_textColor;
}

bool ColorDashboard::isOverlayVisible() const
{
    return m_isOverlayVisible;
}

QColor ColorDashboard::getOverlayColor() const
{
    return m_overlayColor;
}

ColorDashboard::PieStyle ColorDashboard::getPieStyle() const
{
    return m_pieStyle;
}

ColorDashboard::PointerStyle ColorDashboard::getPointerStyle() const
{
    return m_pointerStyle;
}

QEasingCurve::Type ColorDashboard::getEasingCurve() const
{
    return m_easingCurveType;
}
