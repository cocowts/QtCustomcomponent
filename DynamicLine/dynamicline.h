#ifndef DYNAMICLINE_H
#define DYNAMICLINE_H

#include <QChart>
#include <QChartView>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QLineSeries>
#include <QTimer>
#include <QValueAxis>
#include <QVector>
#include <QWidget>

using namespace QtCharts;

/* 动态曲线
 * 1. 可设置 Y 轴范围、标签格式、tick 数量
 * 2. 可设置 X 轴时间跨度、时间格式、tick 数量
 * 3. 可显示任意多条曲线、设置对应曲线颜色及数值点是否显示
 * 4. 可设置曲线更新时间间隔
 * 5. 可选择隐藏曲线
 * 6. 自适应窗体拉伸，图表自动缩放
 */

class DynamicLine : public QWidget
{
    Q_OBJECT

public:
    explicit DynamicLine(QWidget *parent = nullptr);
    ~DynamicLine();

    void start(int intervalMsec = 1000);                        // 开始更新图表
    void stop();                                                // 停止更新图表

    int count() const;                                          // 曲线数量
    qint64 timeAxisXSpanSecs() const;                           // X 轴时间跨度
    void addSplineSeries(const QString &name = "", const QPen &pen = QPen(), bool pointsVisible = false);  // 添加一条新的曲线

public slots:
    void setSeriesValues(int index, int value);                 // 设置曲线值
    void setSeriesVisible(int index, bool visible);             // 设置曲线是否可见
    void setChartTitle(const QString &title);                   // 设置图表标题

    void setTimeAxisXSpanSecs(qint64 secs);                     // 设置 X 轴时间跨度
    void setTimeAxisXFormat(const QString &format = "HH:mm");   // 设置 X 轴时间格式
    void setTimeAxisXTickCount(int tickCount);                  // 设置 X 轴 tick 数量

    void setAxisYRange(qreal min, qreal max);                   // 设置 Y 轴范围
    void setpAxisYTickCount(int count);                         // 设置 Y 轴 tick 数量
    void setAxisYLabelFormat(const QString &format);            // 设置 Y 轴标签格式

private:
    void initUi();
    void initCtrl();

    QLineSeries *createSeries(QChart *chart, const QString &name, const QPen &pen, bool pointsVisible = false);

private slots:
    void updateChart();

private:
    QVector<QLineSeries*> m_series;
    QChartView*           m_pChartView = nullptr;
    QChart*               m_pChart     = nullptr;
    QDateTimeAxis*        m_pTimeAxisX = nullptr;
    QValueAxis*           m_pAxisY     = nullptr;

    qint64 m_spanSecs = 60;
    QVector<int> m_lastValues;
    bool m_isFirstTime = false;

    QTimer m_timer;
};

#endif // DYNAMICLINE_H
