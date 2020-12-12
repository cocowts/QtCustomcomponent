#ifndef WAREPROGRESSBAR_H
#define WAREPROGRESSBAR_H

#include <QColor>
#include <QString>
#include <QTimer>
#include <QWidget>

/* 多彩仪表盘自定义控件 实现的功能
 * 1. 可设置范围值
 * 2. 可设置起始角度
 * 3. 可设置水波密度，密度越大浪越大
 * 4. 可设置水波高度，高度越大浪越大
 * 5. 可设置是否显示进度条
 * 6. 可设置是否显示水纹
 * 7. 可设置进度色、水纹色、文字色
 * 8. 可设置进度条前进方向
 */

class WareProgressBar : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QColor  outerCircleColor       READ getPieCircleColor           WRITE setPieCircleColor)
    Q_PROPERTY(QColor  waterColor             READ getwaterColor               WRITE setwaterColor)
    Q_PROPERTY(QColor  textColor              READ getTextColor                WRITE setTextColor)
    Q_PROPERTY(int     pieStartAngle          READ getPieStartAngle            WRITE setPieStartAngle)
    Q_PROPERTY(int     value                  READ getValue                    WRITE setValue)
    Q_PROPERTY(int     maxValue               READ getMaxValue                 WRITE setMaxValue)
    Q_PROPERTY(int     minValue               READ getMinValue                 WRITE setMinValue)
    Q_PROPERTY(int     waveDensity            READ getWaveDensity              WRITE setWaveDensity)
    Q_PROPERTY(int     waveHeight             READ getWaveHeight               WRITE setWaveHeight)
    Q_PROPERTY(QString suffixText             READ getSuffixText               WRITE setSuffixText)
    Q_PROPERTY(bool    waveForwardOrientation READ getWaveIsForwardOrientation WRITE setWaveIsForwardOrientation)
    Q_PROPERTY(bool    waterIsvisible         READ getWaterIsvisible           WRITE setWaterIsvisible)
    Q_PROPERTY(bool    peiCircleIsvisible     READ getPeiCircleIsvisible       WRITE setPeiCircleIsvisible)
    Q_PROPERTY(bool    peiForwardOrientation  READ getPeiIsForwardOrientation  WRITE setPeiIsForwardOrientation)

public:
    explicit WareProgressBar(QWidget *parent = nullptr);
    ~WareProgressBar();

    QColor getPieCircleColor()               const;
    QColor getwaterColor()                   const;
    QColor getTextColor()                    const;
    int getPieStartAngle()                   const;
    int getValue()                           const;
    int getMaxValue()                        const;
    int getMinValue()                        const;
    int getWaveDensity()                     const;
    int getWaveHeight()                      const;
    QString getSuffixText()                  const;
    bool getWaveIsForwardOrientation()       const;
    bool getWaterIsvisible()                 const;
    bool getPeiCircleIsvisible()             const;
    bool getPeiIsForwardOrientation()        const;

public slots:

    void setPieCircleColor(const QColor &color);                // 设置外圆颜色
    void setwaterColor(const QColor &color);                    // 设置水颜色
    void setTextColor(const QColor &color);                     // 设置文本颜色
    void setPieStartAngle(int angle);                           // 设置饼图起始角度
    void setValue(int value);                                   // 设置开始值
    void setRange(int minValue, int maxValue);                  // 设置范围值
    void setMaxValue(int value);                                // 设置最大值
    void setMinValue(int value);                                // 设置最小值
    void setWaveDensity(int density);                           // 设置水波密度 [0 - 10]
    void setWaveHeight(int height);                             // 设置水波高度 [0 - 10]
    void setSuffixText(const QString &text);                    // 设置提示文本
    void setWaveIsForwardOrientation(bool Orientation);         // 设置水波方向 [true 正向； false 反向]
    void setWaterIsvisible(bool visible);                       // 设置水是否可见
    void setPeiCircleIsvisible(bool visible);                   // 设置圆饼否可见
    void setPeiIsForwardOrientation(bool Orientation);          // 设置圆饼前进方向

protected:
    void paintEvent(QPaintEvent *event) override;
    void drawPieCircel(QPainter *painter);
    void drawBackground(QPainter *painter);
    void drawWater(QPainter *painter);
    void drawText(QPainter *painter);

private:
    double m_radius               = 0.0;

    QColor m_peiCircleColor       = QColor(100,184,255);
    QColor m_waterColor           = QColor(100,184,255);
    QColor m_textColor            = QColor(250,250,250);
    int m_pieStartAngle           = 0;
    int m_value                   = 0;
    int m_maxValue                = 100;
    int m_minValue                = 0;
    int m_waveDensity             = 1;
    int m_waveHeight              = 5;
    bool m_waveForwardOrientation = true;
    QString m_suffixText          = "%";
    bool m_waterIsvisible         = true;
    bool m_peiCircleIsvisible     = true;
    bool m_peiForwardOrientation  = true;

    double m_offset               = 50;
    QTimer m_timer;
};

#endif // WAREPROGRESSBAR_H
