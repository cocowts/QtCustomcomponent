#include "genericdashboard.h"

#include <QFontMetrics>
#include <QPainter>
#include <QPointF>
#include <QPolygon>
#include <QRadialGradient>
#include <QtMath>

GenericDashBoard::GenericDashBoard(QWidget *parent) : QWidget(parent)
{
    m_animation.setTargetObject(this);
    m_animation.setPropertyName("curValue");
    m_animation.setEasingCurve(m_easingCurveType);

    connect(&m_animation, &QPropertyAnimation::valueChanged, [&](const QVariant &var){emit valueChanged(var.toInt());});
    connect(&m_animation, SIGNAL(finished()), this, SIGNAL(finished()));

    setAttribute(Qt::WA_TranslucentBackground, true);
}

GenericDashBoard::~GenericDashBoard()
{ }

void GenericDashBoard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing, true);
    // 移动坐标系原点到widget中心
    painter.translate(width()/2, height()/2);
    // 计算表盘半径
    m_radius = qMin(width(), height())/2;

    drawBackground(&painter);
    drawScale(&painter);
    drawScaleNum(&painter);
    drawPainterIndicator(&painter);
    drawText(&painter);

    return QWidget::paintEvent(event);
}

// 绘制表盘背景
void GenericDashBoard::drawBackground(QPainter *painter)
{
    double radius = m_radius;

    painter->save();

    // 绘制外表盘 【同比于手表金属外框】
    painter->setPen(Qt::NoPen);
    painter->setBrush(m_outerCircleColor);
    painter->drawEllipse(-radius , -radius, 2 * radius, 2 * radius);

    // 绘制内表盘 【同比于手表玻璃表盘】
    radius = m_radius * 0.9;
    painter->setBrush(m_innerCircleColor);
    painter->drawEllipse(-radius , -radius, 2 * radius, 2 * radius);

    painter->restore();
}

// 绘制表盘刻度
void GenericDashBoard::drawScale(QPainter *painter)
{
    const double radius       = m_radius * 0.89;
    const double shadowRadius = radius * 0.95;
    const double lineWidth    = m_radius * 0.009;
    const double penWidth     = m_radius * 0.1;
    const double scaleNum     = m_scaleMajor * m_scaleMinor;
    const double rotate       = (m_endAngle - m_startAngle) / scaleNum;

    painter->save();

    // 旋转坐标系，使每次绘制都停留在y轴上，即[x=0, y]
    painter->rotate(m_startAngle);

    // 绘制刻度阴影
    painter->setPen(QPen(QColor(255,255,255,20), penWidth));
    painter->setBrush(Qt::transparent);
    painter->drawArc(QRectF(-shadowRadius, -shadowRadius, shadowRadius*2, shadowRadius*2), 0 + 3*16, (m_endAngle - m_startAngle)*16 - 3*16*2);

    // 绘制刻度
    for (int i=0; i<=scaleNum; ++i)
    {
        QColor color(m_scaleStartColor);

        if (i > scaleNum * m_scalePercentage)
            color = m_scaleEndColor;

        if (i % m_scaleMinor == 0)
        {
            painter->setPen(QPen(color, 1.4*lineWidth));
            painter->drawLine(QPointF(0, radius), QPointF(0, radius / 1.16));
        }
        else
        {
            painter->setPen(QPen(color, 1.1*lineWidth));
            painter->drawLine(QPointF(0, radius), QPointF(0, radius / 1.1));
        }

        painter->rotate(rotate);
    }

    painter->restore();
}

// 绘制表盘刻度值
void GenericDashBoard::drawScaleNum(QPainter *painter)
{
    const double radius = m_radius * 0.62;
    const double rotate = (m_endAngle - m_startAngle) / m_scaleMajor * 1.0;

    painter->save();
    painter->setFont(QFont("Consolas", m_radius * 0.06, QFont::Bold));
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

// 绘制指示指针
void GenericDashBoard::drawPainterIndicator(QPainter *painter)
{
    QPolygon polygon;
    int radius    = m_radius * 0.6;
    double rotate = m_startAngle + 1.0 * (m_endAngle - m_startAngle) / (m_maxValue - m_minValue) * m_curValue;
    int width     = radius * 0.08;

    painter->save();

    // 绘制指针
    polygon.setPoints(3, -width, 0, width, 0, 0, radius);
    painter->rotate(rotate);
    painter->setBrush(m_pointerColor);
    painter->setPen(m_pointerColor);
    painter->drawConvexPolygon(polygon);

    QPen pen(painter->pen());
    pen.setCapStyle(Qt::RoundCap);
    pen.setWidth(radius * 0.04);
    painter->setPen(pen);
    painter->drawLine(0, 0, 0, radius);

    // 绘制中心圆
    QRadialGradient gradient(0, 0, 14);
    gradient.setColorAt(0, QColor(255, 255, 255));
    gradient.setColorAt(1, m_pointerColor);
    painter->setBrush(gradient);
    painter->setPen(Qt::NoPen);
    radius = radius * 0.15;
    painter->drawEllipse(-radius, -radius, radius*2, radius*2);

    painter->restore();
}

// 绘制提示文本
void GenericDashBoard::drawText(QPainter *painter)
{
    painter->save();

    painter->setFont(QFont("Consolas", m_radius * 0.07, QFont::Bold));
    painter->setPen(m_textColor);
    QFontMetrics fm = QFontMetrics(painter->font());
    QString text = QString::number(m_curValue, 10) + m_suffixText;
    int w = fm.horizontalAdvance(text);
    painter->drawText(QPoint(-w/2, m_radius/2), text);

    painter->restore();
}

int GenericDashBoard::getMinValue() const
{
    return m_minValue;
}

int GenericDashBoard::getMaxValue() const
{
    return m_maxValue;
}

int GenericDashBoard::getValue() const
{
    return m_value;
}

int GenericDashBoard::getAnimationStepTime() const
{
    return m_animationStepTime;
}

int GenericDashBoard::getScaleMajor() const
{
    return m_scaleMajor;
}

int GenericDashBoard::getScaleMinor() const
{
    return m_scaleMinor;
}

int GenericDashBoard::getStartAngle() const
{
    return m_startAngle;
}

int GenericDashBoard::getEndAngle() const
{
    return m_endAngle;
}

QColor GenericDashBoard::getScaleNumColor() const
{
    return m_scaleNumColor;
}

QColor GenericDashBoard::getOuterCircleColor() const
{
    return m_outerCircleColor;
}

QColor GenericDashBoard::getInnerCircleColor() const
{
    return m_innerCircleColor;
}

QColor GenericDashBoard::getScaleStartColor() const
{
    return m_scaleStartColor;
}

QColor GenericDashBoard::getScaleEndColor() const
{
    return m_scaleEndColor;
}

QColor GenericDashBoard::getPointerColor() const
{
    return m_pointerColor;
}

QColor GenericDashBoard::getTextColor() const
{
    return m_textColor;
}

QString GenericDashBoard::getSuffixText() const
{
   return m_suffixText;
}

int GenericDashBoard::getCurValue() const
{
    return m_curValue;
}

QEasingCurve::Type GenericDashBoard::getEasingCurve() const
{
    return m_easingCurveType;
}

double GenericDashBoard::getScalePercentage() const
{
    return m_scalePercentage;
}

void GenericDashBoard::setRange(int minValue, int maxValue)
{
    m_minValue = minValue;
    m_maxValue = maxValue;
    update();
}

void GenericDashBoard::setMinValue(int minValue)
{
    m_minValue = minValue;
    update();
}

void GenericDashBoard::setMaxValue(int maxValue)
{
    m_maxValue = maxValue;
    update();
}

void GenericDashBoard::setValue(int value)
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

void GenericDashBoard::updateValue(int value)
{
    m_curValue = value;
    update();
}

void GenericDashBoard::setAnimationStepTime(int msec)
{
    m_animationStepTime = msec;
}

void GenericDashBoard::setScaleMajor(int scaleMajor)
{
    m_scaleMajor = scaleMajor;
    update();
}
void GenericDashBoard::setScaleMinor(int scaleMinor)
{
    m_scaleMinor = scaleMinor;
    update();
}

void GenericDashBoard::setAngleRange(int startAngle, int endAngle)
{
    m_startAngle = startAngle;
    m_endAngle   = endAngle;
    update();
}

void GenericDashBoard::setStartAngle(int startAngle)
{
    m_startAngle = startAngle;
    update();
}

void GenericDashBoard::setEndAngle(int endAngle)
{
    m_endAngle = endAngle;
    update();
}

void GenericDashBoard::setScaleNumColor(const QColor &scaleNumColor)
{
    m_scaleNumColor = scaleNumColor;
    update();
}

void GenericDashBoard::setOuterCircleColor(const QColor &outerCircleColor)
{
    m_outerCircleColor = outerCircleColor;
    update();
}

void GenericDashBoard::setInnerCircleColor(const QColor &innerCircleColor)
{
    m_innerCircleColor = innerCircleColor;
    update();
}

void GenericDashBoard::setScaleStartColor(const QColor &scaleStartColor)
{
    m_scaleStartColor = scaleStartColor;
    update();
}

void GenericDashBoard::setScaleEndColor(const QColor &scaleEndColor)
{
    m_scaleEndColor = scaleEndColor;
    update();
}

void GenericDashBoard::setPointerColor(const QColor &pointerColor)
{
    m_pointerColor = pointerColor;
    update();
}

void GenericDashBoard::setTextColor(const QColor & textColor)
{
    m_textColor = textColor;
    update();
}

void GenericDashBoard::setSuffixText(const QString &string)
{
    m_suffixText = string;
    update();
}

void GenericDashBoard::setEasingCurve(QEasingCurve::Type type)
{
    m_easingCurveType = type;
    m_animation.setEasingCurve(m_easingCurveType);
}

void GenericDashBoard::setScalePercentage(double percentage)
{
    if (percentage > 1.0)
        percentage = 1.0;
    else if (percentage < 0.0)
        percentage = 0.0;

    m_scalePercentage = percentage;
    update();
}

