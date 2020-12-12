#ifndef ARCPROGRESSBAR_H
#define ARCPROGRESSBAR_H

#include <QWidget>
#include <QColor>
#include <QString>
#include <QPropertyAnimation>

/* 圆弧进度条自定义控件 实现的功能
 * 1. 可设置范围
 * 2. 可设置开始旋转角度、结束旋转角度
 * 3. 可设置过度动画时常
 * 4. 可设置仪表盘标题
 * 5. 可设置背景、进度条、值、标题颜色
 * 6. 自适应窗体拉伸，文字自动缩放
 */

class ArcProgressBar : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor arcColor        READ getArcColor          WRITE setArcColor)
    Q_PROPERTY(QColor baseColor       READ getBaseColor         WRITE setBaseColor)
    Q_PROPERTY(QColor textColor       READ getTextColor         WRITE setTextColor)
    Q_PROPERTY(QColor titleColor      READ getTitleColor        WRITE setTitleColor)
    Q_PROPERTY(QString title          READ getTitle             WRITE setTitle)
    Q_PROPERTY(int minValue           READ getMinValue          WRITE setMinValue)
    Q_PROPERTY(int maxValue           READ getMaxValue          WRITE setMaxValue)
    Q_PROPERTY(int value              READ getValue             WRITE setValue)
    Q_PROPERTY(int animationStepTime  READ getAnimationStepTime WRITE setAnimationStepTime)
    Q_PROPERTY(int startAngle         READ getStartAngle        WRITE setStartAngle)
    Q_PROPERTY(int endAngle           READ getEndAngle          WRITE setEndAngle)
    Q_PROPERTY(int curValue           READ getCurValue          WRITE updateValue)

public:
    explicit ArcProgressBar(QWidget *parent = nullptr);
    ~ArcProgressBar();

    QColor getArcColor()       const;
    QColor getBaseColor()      const;
    QColor getTextColor()      const;
    QColor getTitleColor()     const;
    QString getTitle()         const;
    int getMinValue()          const;
    int getMaxValue()          const;
    int getValue()             const;
    int getStartAngle()        const;
    int getEndAngle()          const;
    int getAnimationStepTime() const;

public slots:
    void setArcColor(const QColor &color);              // 设置圆弧颜色
    void setBaseColor(const QColor &color);             // 设置基础颜色
    void setTextColor(const QColor &color);             // 设置文本颜色
    void setTitleColor(const QColor &color);            // 设置标题颜色

    void setTitle(const QString &title);                // 设置Title

    void setRange(int minValue, int maxValue);          // 设置范围值
    void setMinValue(int minValue);                     // 设置最小值
    void setMaxValue(int maxValue);                     // 设置最大值
    void setValue(int value);                           // 设置目标值

    void setAngleRange(int startAngle, int endAngle);   // 设备旋转角度范围
    void setStartAngle(int startAngle);                 // 设置开始旋转角度
    void setEndAngle(int endAngle);                     // 设置结束旋转角度

    void setAnimationStepTime(int msec);                // 设置每刻度动画持续时间

protected:
    void paintEvent(QPaintEvent *event) override;

    void drawArc(QPainter *painter);
    void drawValue(QPainter *painter);
    void drawTitle(QPainter *painter);

    void updateValue(int value);
    int getCurValue() const;

signals:

private:
    double m_radius = 0.0;

    QColor m_arcColor   = QColor(34,163,169);
    QColor m_baseColor  = QColor(180,180,180);
    QColor m_textColor  = QColor(34,163,169);
    QColor m_titleColor =  QColor(Qt::black);

    QString m_title     = "正确率";

    int m_startAngle    = 45;
    int m_endAngle      = 315;

    int m_maxValue      = 100;
    int m_minValue      = 0;
    int m_curValue      = 0;
    int m_value         = 0;

    QPropertyAnimation m_animation;
    int m_animationStepTime              = 5;
    QEasingCurve::Type m_easingCurveType = QEasingCurve::Linear;
};

#endif // ARCPROGRESSBAR_H
