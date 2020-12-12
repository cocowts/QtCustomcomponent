#ifndef COLORDASHBOARD_H
#define COLORDASHBOARD_H

#include <QColor>
#include <QPropertyAnimation>
#include <QWidget>

/* 多彩仪表盘自定义控件 实现的功能
 * 1. 可设置范围值
 * 2. 可设置大刻度数量，小刻度数量
 * 3. 可设置开始旋转角度、结束旋转角度
 * 4. 可设置是否启用动画、动画特效及每刻度动画持续时间
 * 5. 可设置外圆背景、内圆背景、提示指针、刻度尺、刻度值、提示文字颜色
 * 6. 可设置三色刻度占用比例
 * 7. 可设置圆环样式，三色圆环、当前圆环
 * 8. 可设置指示器样式，球形、指针形、圆角指针、三角形指示器
 * 8. 自适应窗体拉伸、刻度尺和文字自动缩放
 */

class ColorDashboard : public QWidget
{
    Q_OBJECT
    Q_ENUMS(PieStyle)
    Q_ENUMS(PointerStyle)

    Q_PROPERTY(int minValue               READ getMinValue               WRITE setMinValue)
    Q_PROPERTY(int maxValue               READ getMaxValue               WRITE setMaxValue)
    Q_PROPERTY(int scaleMajor             READ getScaleMajor             WRITE setScaleMajor)
    Q_PROPERTY(int scaleMinor             READ getScaleMinor             WRITE setScaleMinor)
    Q_PROPERTY(int startAngle             READ getStartAngle             WRITE setStartAngle)
    Q_PROPERTY(int endAngle               READ getEndAngle               WRITE setEndAngle)
    Q_PROPERTY(int animationStepTime      READ getAnimationStepTime      WRITE setAnimationStepTime)
    Q_PROPERTY(QColor outerCircleColor    READ getOuterCircleColor       WRITE setOuterCircleColor)
    Q_PROPERTY(QColor innerCircleColor    READ getInnerCircleColor       WRITE setInnerCircleColor)
    Q_PROPERTY(QColor m_pieColorStart     READ getPieStartColor          WRITE setPieStartColor)
    Q_PROPERTY(QColor m_pieColorMid       READ getPieMidColor            WRITE setPieMidColor)
    Q_PROPERTY(QColor m_pieColorEnd       READ getPieEndColor            WRITE setPieEndColor)
    Q_PROPERTY(QColor scaleColor          READ getScaleColor             WRITE setScaleColor)
    Q_PROPERTY(QColor scaleNumColor       READ getScaleNumColor          WRITE setScaleNumColor)
    Q_PROPERTY(QColor pointerColor        READ getPointerColor           WRITE setPointerColor)
    Q_PROPERTY(QColor centerCirleColor    READ getCenterCircleColor      WRITE setCenterCircleColor)
    Q_PROPERTY(QColor textColor           READ getTextColor              WRITE setTextColor)
    Q_PROPERTY(bool   isOverlayVisible    READ isOverlayVisible          WRITE setOverlayVisible)
    Q_PROPERTY(QColor overlayColor        READ getOverlayColor           WRITE setOverlayColor)
    Q_PROPERTY(PieStyle     pieStyle      READ getPieStyle               WRITE setPieStyle)
    Q_PROPERTY(PointerStyle pointerStyle  READ getPointerStyle           WRITE setPointerStyle)
    Q_PROPERTY(QEasingCurve::Type easingCurveType READ getEasingCurve    WRITE setEasingCurve)
    Q_PROPERTY(int curValue               READ getCurValue               WRITE updateValue)

public:
    enum PieStyle {
        PieStyle_Three = 0u,                // 三色圆环
        PieStyle_Current                    // 当前圆环
    };

    enum PointerStyle {
        PointerStyle_Ball = 0u,             // 圆指示器
        PointerStyle_Indicator,             // 指针指示器
        PointerStyle_IndicatorR,            // 圆角指针指示器
        PointerStyle_Triangle               // 三角指示器
    };

    explicit ColorDashboard(QWidget *parent = nullptr);
    ~ColorDashboard();

    int getMinValue()                      const;
    int getMaxValue()                      const;

    int getValue()                         const;

    int getScaleMajor()                    const;
    int getScaleMinor()                    const;

    int getStartAngle()                    const;
    int getEndAngle()                      const;

    int getAnimationStepTime()             const;

    QColor getOuterCircleColor()           const;
    QColor getInnerCircleColor()           const;

    QColor getPieStartColor()              const;
    QColor getPieMidColor()                const;
    QColor getPieEndColor()                const;

    QColor getScaleColor()                 const;
    QColor getScalCircleColor()            const;
    QColor getScaleNumColor()              const;
    QColor getPointerColor()               const;
    QColor getCenterCircleColor()          const;
    QColor getTextColor()                  const;

    bool isOverlayVisible()                const;
    QColor getOverlayColor()               const;

    PieStyle getPieStyle()                 const;
    PointerStyle getPointerStyle()         const;

    QEasingCurve::Type getEasingCurve()    const;

public slots:
    void setRange(int minValue, int maxValue);                  // 设置范围值
    void setMinValue(int minValue);                             // 设置最小值
    void setMaxValue(int maxValue);                             // 设置最大值

    void setValue(int value);                                   // 设置目标值

    void setScaleMajor(int scaleMajor);                         // 设置主刻度数量
    void setScaleMinor(int scaleMinor);                         // 设置小刻度数量

    void setAngleRange(int startAngle, int endAngle);           // 设备旋转角度范围
    void setStartAngle(int startAngle);                         // 设置开始旋转角度
    void setEndAngle(int endAngle);                             // 设置结束旋转角度

    void setAnimationStepTime(int msec);                        // 设置每刻度动画持续时间

    void setOuterCircleColor(const QColor &outerCircleColor);   // 设置外圆颜色
    void setInnerCircleColor(const QColor &innerCircleColor);   // 设置内圆颜色

    // 设置饼圆三种颜色
    void setPieStartColor(const QColor &scaleStartColor);
    void setPieMidColor(const QColor &scaleMidColor);
    void setPieEndColor(const QColor &scaleEndColor);

    void setScaleColor(const QColor &scaleColor);               // 设置刻度颜色
    void setScalCircleColor(const QColor &scaleCircleColor);    // 设置刻度圆颜色
    void setScaleNumColor(const QColor &scaleNumColor);         // 设置刻度值颜色
    void setPointerColor(const QColor &pointerColor);           // 设置指针颜色
    void setCenterCircleColor(const QColor &centerCircleColor); // 设置中心圆颜色
    void setTextColor(const QColor &textColor);                 // 设置提示文本颜色

    void setOverlayVisible(bool overlay);                       // 设置是否显示遮蔽罩
    void setOverlayColor(const QColor &overlayColor);           // 设置遮蔽罩颜色

    void setPieStyle(PieStyle pieStyle);                        // 设置饼图央视
    void setPointerStyle(PointerStyle pointerStyle);            // 设置指针样式

    void setEasingCurve(QEasingCurve::Type type);               // 设置指针动画类型

protected:
    void paintEvent(QPaintEvent *e) override;

    inline void drawGenericCircle(QPainter *painter, double radius, const QColor &cokor);
    void drawOuterCircle(QPainter *painter);
    void drawScaleCircle(QPainter *painter);
    void drawPieCircle(QPainter *painter);
    void drawThreePieCircle(QPainter *painter, double radius);
    void drawCurrentPieCircle(QPainter *painter, double radius);
    void drawInnerCircle(QPainter *painter);
    void drawPointerCircle(QPainter *painter);
    void drawCenterCircel(QPainter *painter);
    void drawScaleNum(QPainter *painter);
    void drawScale(QPainter *painter);
    void drawOverlay(QPainter *painter);
    void drawPointer(QPainter *painter);
    void drawPointerBall(QPainter *painter);
    void drawPointerIndicator(QPainter *painter);
    void drawPointerIndicatorR(QPainter *painter);
    void drawPointerTriangle(QPainter *painter);
    void drawText(QPainter *painter);

    void updateValue(int value);
    int getCurValue() const;

signals:
    void valueChanged(int value);
    void finished();

private:
    double m_radius           = 0;                                  // 仪表盘半径

    QColor m_outerCircleColor = QColor(80, 80, 80);                 // 外圆颜色
    QColor m_scaleCircleColor = QColor(60, 60, 60);                 // 刻度圆颜色
    QColor m_innerCircleColor = QColor(100, 100, 100);              // 内圆颜色
    QColor m_centerCirleColor = QColor(250, 250, 250);              // 中心圆颜色
    QColor m_pointerColor     = QColor(223,105,105);                // 指针颜色
    QColor m_pieColorStart    = QColor(24,189,155);                 // 饼圆开始颜色
    QColor m_pieColorMid      = QColor(218,218,0);                  // 饼圆中间颜色
    QColor m_pieColorEnd      = QColor(255,107,107);                // 饼圆结束颜色
    QColor m_scaleNumColor    = QColor(255,255,255);                // 刻度字体颜色
    QColor m_scaleColor       = QColor(255,255,255);                // 刻度颜色
    QColor m_overlayColor     = QColor(255,255,255);                // 遮避罩颜色
    QColor m_textColor        = QColor(0,0,0);                      // 提示文本颜色

    PieStyle m_pieStyle         = PieStyle_Current;                 // 饼风格
    PointerStyle m_pointerStyle = PointerStyle_Triangle;            // 指示器样式

    int m_value               = 0;                                  // 表盘设定值
    int m_curValue            = 0;                                  // 表盘当前值
    int m_minValue            = 0;                                  // 表盘最小值
    int m_maxValue            = 240;                                // 表盘最大值
    int m_scaleMajor          = 10;                                 // 大刻度数量
    int m_scaleMinor          = 5;                                  // 小刻度数量        【如：总刻度数量 = scaleMajor * scaleMinor】
    int m_startAngle          = 45;                                 // 起始旋转角度
    int m_endAngle            = 315;                                // 结束旋转角度
    bool m_isOverlayVisible   = true;                               // 是否显示遮蔽罩

    QPropertyAnimation m_animation;                                 // 指针属性动画
    int m_animationStepTime   = 50;                                 // 每刻度动画持续时间
    QEasingCurve::Type m_easingCurveType = QEasingCurve::OutQuad;   // 指针动画缓和曲线类型
};

#endif // COLORDASHBOARD_H
